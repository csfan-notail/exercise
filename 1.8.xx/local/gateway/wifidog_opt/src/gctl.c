/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : gctl.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/12/11
  Last Modified :
  Description   : wifidog调试命令
  Function List :
  History       :
  1.Date        : 2013/12/11
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "gctl.h"

static int usage(int argc, char **argv);
static int gctl_start(int argc, char **argv);
static int gctl_stop(int argc, char **argv);
static int gctl_restart(int argc, char **argv);
static int gctl_status(int argc, char **argv);
/*add@2014.10.11*/
static int gctl_global(int argc, char **argv);
static int gctl_show_cnnt_sta(int argc, char **argv);
static int gctl_show_reg_ap(int argc, char **argv);
static int gctl_debug(int argc, char **argv);
static int gctl_extdebug(int argc, char **argv);
static int gctl_auth_ctrl(int argc, char **argv);
static int gctl_sta_auth(int argc, char **argv);
static int gctl_stat(int argc, char **argv);
static int gctl_up_wxl_whitelist(int argc, char **argv);
static int gctl_vap(int argc, char **argv);
static int gctl_running_status(int argc, char **argv);
static int gctl_how_maro_roam_sta(int argc, char **argv);
static int gctl_show_ios_sta(int argc, char **argv);
static int gctl_show_sta_acl(int argc, char **argv);
static int gctl_sta_http_record(int argc, char **argv);
static int gctl_span_ssid_auth(int argc, char **argv);
static int gctl_lan_info(int argc, char **argv);



gctl_config config[GCTL_MAX] = {
{GCTL_START, "start", "Start wifidog.", gctl_start},
{GCTL_STOP, "stop", "Stop wifidog.", gctl_stop},
{GCTL_RESTART, "restart", "restart wifidog.", gctl_restart},
{GCTL_STATUS, "status", "Display clients information.", gctl_status},
{GCTL_GLOBAL, "global", "Display global config.", gctl_global},    
{GCTL_SHOW_AP, "showap", "Display registered APs", gctl_show_reg_ap},
{GCTL_SHOW_CNNT_STA, "showsta", "Display wireless connected STAs.", gctl_show_cnnt_sta},
{GCTL_SETLOG_LEVEL, "debug", "Set WiFiDog debug level(1..7).", gctl_debug},
{GCTL_EXTDGB_LEVEL, "ext-debug", "Set WiFiDog extend module debug level(1..7).", gctl_extdebug},
{GCTL_AUTH_CTRL, "auth-ctrl", "Set Auth Control (x,x,x: ios-auto_portal,ios-rd-mode,ucweb-rd-mode)", gctl_auth_ctrl},
{GCTL_STA_AUTH, "sta-auth", "Set STA auth status (XX:XX:XX:XX:XX:XX 0|2)", gctl_sta_auth},
{GCTL_STAT, "stat", "Get statistics (start|stop [interval] [duration])", gctl_stat},
{GCTL_UP_WXL_WHITELIST, "up-wxl", "Update wxl whitelist.", gctl_up_wxl_whitelist},
{GCTL_SSID, "vap-update", "Update vap info", gctl_vap},
{GCTL_WDSTATUS, "run-status", "Get wifidog running status", gctl_running_status},
{GCTL_MARO_ROAM_STA,"roam-sta","Get maro hot roaming sta",gctl_how_maro_roam_sta},
{GCTL_IOS_STA,"ios-sta","Get ios sta",gctl_show_ios_sta},
{GCTL_STA_ACL,"sta-acl","Get sta acl", gctl_show_sta_acl},
{GCTL_SPAN_SSID_AUTH,"ssid-auth","set span ssid auth switch([enable|disable])",gctl_span_ssid_auth},
{GCTL_STA_HTTP_RECORD,"http-record","Sta http record ([switch] [wdura] [bdura] [maxcnt] [ckintvl] [ckstalimit])", gctl_sta_http_record},
{GCTL_LAN_INFO,"laninfo","Get|Set lan info([update])", gctl_lan_info},
{GCTL_HELP, "help", "Display this messages.", usage}
};

static int connect_gctl_server(void)
{
    int fd;
    struct sockaddr_un addr;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, DEFAULT_SOCK);

    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        printf("Message: The wifidog could not running\n");
        exit(1);
    }
    return fd;
}

static int send_request(int fd, char *req)
{
    int write_len, len;

    write_len = len = 0;

    while(write_len != strlen(req))
    {
        len = write(fd, req + write_len, strlen(req) - write_len);
        if(-1 == len)
        {
            printf("Write to wifidog failed: %s\n", strerror(errno));
            exit(1);
        }
        write_len += len;
    }

    return len;
}

static int usage(int argc, char **argv)
{
    int i, j, gap;
	char printbuf[128];

    printf("Usage: gctl option []\n");
    printf("\n");
    
    for(i = 0; i < GCTL_MAX; i++)
    {
    	sprintf(printbuf, "  %s", config[i].options);

		gap = 16 - strlen(printbuf);
		if(gap > 0){
			for(j=strlen(printbuf); j<16; ++j)
				printbuf[j] = ' ';
			printbuf[16] = '\0';
		}

		sprintf(printbuf + strlen(printbuf), "%s\n", config[i].msg);
        printf("%s", printbuf);
    }

    printf("\n");
    return 0;
}

static int gctl_start(int argc, char **argv)
{

    if(0 == system("pidof wifidog 1>/dev/null 2>&1"))    
        printf("Message: the wifidog is already running.\n");
    else
        system("wifidog -s 1>/dev/null 2>&1 &");

    return 0;
}

static int gctl_stop(int argc, char **argv)
{
    int fd;
    char req[32];
    
    fd = connect_gctl_server();

    memset(req, 0, sizeof(req));
    sprintf(req, "stop\r\n");
    send_request(fd, req);
    close(fd);
    return 0;
}

static int gctl_restart(int argc, char **argv)
{
    // 关闭WiFiDog
    gctl_stop(argc, argv);
    gctl_start(argc, argv);
    return 0;
}

static int gctl_status(int argc, char **argv)
{
    int fd;
    int done;
    int len;
    char req[128];
    char resp[4096];

    fd = connect_gctl_server();
    sprintf(req, "status\r\n");
    send_request(fd, req);

    done = 0;
    len = 0;
    while(!done)
    {
        memset(resp, 0, sizeof(resp));
        len = read(fd, resp, sizeof(resp));
        if(len > 0)
        {
            printf("%s", resp);
            len = 0;
        }
        else
        {
            done = 1;
        }
    }

    close(fd);
    return 0;
}
/*begin: add@2014.10.11*/
static int gctl_global(int argc, char **argv)
{
    int fd;
    int done;
    int len;
    char req[128];
    char resp[2048];

    fd = connect_gctl_server();
    sprintf(req, "global\r\n");
    send_request(fd, req);

    done = 0;
    len = 0;
    while(!done)
    {
        memset(resp, 0, sizeof(resp));
        len = read(fd, resp, sizeof(resp));
        if(len > 0)
        {
            printf("%s", resp);
            len = 0;
        }
        else
        {
            done = 1;
        }
    }

    close(fd);
    return 0;
}
/*end: add@2014.10.11*/

static int gctl_show_reg_ap(int argc, char **argv)
{
	int fd;
	int done;
	int len;
	char req[128];
	char resp[4096];

	fd = connect_gctl_server();
	sprintf(req, "showap\r\n");
	send_request(fd, req);

	done = 0;
	len = 0;
	while(!done)
	{
		memset(resp, 0, sizeof(resp));
		len = read(fd, resp, sizeof(resp));
		if(len > 0)
		{
			printf("%s", resp);
			len = 0;
		}
		else
		{
			done = 1;
		}
	}

	close(fd);
	return 0;
}



static int gctl_span_ssid_auth(int argc, char **argv)
{
	int fd;
	int done;
	int len;
	char req[128];
	char resp[4096];

       char aucParam[32]={0};
	if(3 == argc)
	{
	    if(0 == strcmp(argv[2],"enable") || 0 == strcmp(argv[2],"disable"))
	    {
	        sprintf(aucParam,argv[2]);
	    }
	    else
	    {
	        printf("Use: gctl ssid-auth [enable|disable]");
	        return 0;
	    }
	}

	fd = connect_gctl_server();
	sprintf(req, "ssid-auth %s\r\n",aucParam);
	printf("********send req:%s\n",req);
	send_request(fd, req);

	done = 0;
	len = 0;
	while(!done)
	{
		memset(resp, 0, sizeof(resp));
		len = read(fd, resp, sizeof(resp));
		if(len > 0)
		{
			printf("%s", resp);
			len = 0;
		}
		else
		{
			done = 1;
		}
	}

	close(fd);
	return 0;
}




static int gctl_show_cnnt_sta(int argc, char **argv)
{
	int fd;
	int done;
	int len;
	char req[128];
	char resp[4096];

	fd = connect_gctl_server();
	sprintf(req, "showsta\r\n");
	send_request(fd, req);

	done = 0;
	len = 0;
	while(!done)
	{
		memset(resp, 0, sizeof(resp));
		len = read(fd, resp, sizeof(resp));
		if(len > 0)
		{
			printf("%s", resp);
			len = 0;
		}
		else
		{
			done = 1;
		}
	}

	close(fd);
	return 0;
}


static int gctl_show_ios_sta(int argc, char **argv)
{
	int fd;
	int done;
	int len;
	char req[128];
	char resp[4096];

	fd = connect_gctl_server();
	sprintf(req, "showiossta\r\n");
	send_request(fd, req);

	done = 0;
	len = 0;
	while(!done)
	{
		memset(resp, 0, sizeof(resp));
		len = read(fd, resp, sizeof(resp));
		if(len > 0)
		{
			printf("%s", resp);
			len = 0;
		}
		else
		{
			done = 1;
		}
	}

	close(fd);
	return 0;
}

static int gctl_show_sta_acl(int argc, char **argv)
{
	int fd;
	int done;
	int len;
	char req[128];
	char resp[4096];

	fd = connect_gctl_server();
	sprintf(req, "showaclsta\r\n");
	send_request(fd, req);

	done = 0;
	len = 0;
	while(!done)
	{
		memset(resp, 0, sizeof(resp));
		len = read(fd, resp, sizeof(resp));
		if(len > 0)
		{
			printf("%s", resp);
			len = 0;
		}
		else
		{
			done = 1;
		}
	}

	close(fd);
	return 0;
}


static int gctl_sta_http_record(int argc, char **argv)
{
	int fd;
	int done;
	int len;
	char req[128] = {0};
	char resp[4096];
	int i;

	fd = connect_gctl_server();
	if(argc == 2)
		sprintf(req, "httprecord\r\n");
	else if(argc > 2){
		sprintf(req, "httprecord ");
		for(i=2; i<argc; ++i){
			len = strlen(req);
			snprintf(req+len, sizeof(req)-len, "%s ", argv[i]);
		}
		len = strlen(req);
		snprintf(req+len, sizeof(req)-len, "\r\n");
	}
	else
		return usage(argc, argv);
	
	send_request(fd, req);

	if(argc == 2){
		done = 0;
		len = 0;
		while(!done)
		{
			memset(resp, 0, sizeof(resp));
			len = read(fd, resp, sizeof(resp));
			if(len > 0)
			{
				printf("%s", resp);
				len = 0;
			}
			else
			{
				done = 1;
			}
		}
	}

	close(fd);
	return 0;
}

static int gctl_lan_info(int argc, char **argv)
{
	int fd;
	int done;
	int len;
	char req[128] = {0};
	char resp[4096];
	int i;

	fd = connect_gctl_server();
	if(argc == 2)
		sprintf(req, "laninfo\r\n");
	else if(argc > 2){
		sprintf(req, "laninfo ");
		for(i=2; i<argc; ++i){
			len = strlen(req);
			snprintf(req+len, sizeof(req)-len, "%s ", argv[i]);
		}
		len = strlen(req);
		snprintf(req+len, sizeof(req)-len, "\r\n");
	}
	else
		return usage(argc, argv);
	
	send_request(fd, req);

	if(argc == 2){
		done = 0;
		len = 0;
		while(!done)
		{
			memset(resp, 0, sizeof(resp));
			len = read(fd, resp, sizeof(resp));
			if(len > 0)
			{
				printf("%s", resp);
				len = 0;
			}
			else
			{
				done = 1;
			}
		}
	}

	close(fd);
	return 0;
}


static int gctl_debug(int argc, char **argv)
{
    char req[32];
    int fd;

    if(argc != 3)
        return usage(argc, argv);

    // 检测最后一位是否是正确的值
    if(strlen(argv[2]) != 1
       || 0 == isdigit(argv[2][0]))
    {
        printf("The level is not legitimate\n");
        return 1;
    }
    memset(req, 0, sizeof(req));
    sprintf(req, "debug %c\r\n", argv[2][0]);
    fd = connect_gctl_server();
    send_request(fd, req);
    close(fd);
    return 0;
}

static int gctl_extdebug(int argc, char **argv)
{
    char req[32];
    int fd;

    if(argc != 3)
        return usage(argc, argv);

    // 检测最后一位是否是正确的值
    if(strlen(argv[2]) != 1
       || 0 == isdigit(argv[2][0]))
    {
        printf("The level is not legitimate\n");
        return 1;
    }
    memset(req, 0, sizeof(req));
    sprintf(req, "ext-debug %c\r\n", argv[2][0]);
    fd = connect_gctl_server();
    send_request(fd, req);
    close(fd);
    return 0;
}

static int gctl_auth_ctrl(int argc, char **argv)
{
    char req[32];
    int fd;

    if(argc != 3)
        return usage(argc, argv);

    memset(req, 0, sizeof(req));
    sprintf(req, "auth-ctrl %s\r\n", argv[2]);
    fd = connect_gctl_server();
    send_request(fd, req);
    close(fd);
    return 0;
}

static int gctl_sta_auth(int argc, char **argv)
{
    char req[64];
    int fd;

    if(argc != 4)
        return usage(argc, argv);

    memset(req, 0, sizeof(req));
    sprintf(req, "sta-auth %s %s\r\n", argv[2], argv[3]);
    fd = connect_gctl_server();
    send_request(fd, req);
    close(fd);

	return 0;
}

static int gctl_stat(int argc, char **argv)
{
#define DEF_DURATION	0 	/*hour*/
#define DEF_INTERVAL	5  	/*second*/

    char req[64];
    int fd;
	int duration = 0, interval = 0;

    if(argc < 3)
        return usage(argc, argv);

    memset(req, 0, sizeof(req));
	if(!strncmp(argv[2], "start", 5)){
		if(argc == 3){
			interval = DEF_INTERVAL;
			duration = DEF_DURATION;
		}
		else if(argc == 4){
			interval = atoi(argv[3]);
			duration = DEF_DURATION;
		}
		else if(argc == 5){
			interval = atoi(argv[3]);
			duration = atoi(argv[4]);
		}
		else
			return usage(argc, argv);

		if(interval < 1)
			return usage(argc, argv);

		if(0 == duration){
			printf("Start to stat, interval is %d second(s), duration is forever\n",
				interval);
		}
		else{
			printf("Start to stat, interval is %d second(s), duration is %d hour(s)\n",
				interval, duration);
		}

		sprintf(req, "stat-start %d %d\r\n", interval, duration);
	}
	else if(!strncmp(argv[2], "stop", 4)){
		if(argc > 3)
        	return usage(argc, argv);
		sprintf(req, "stat-stop\r\n");
	}
	else{
		return usage(argc, argv);
	}
	
    fd = connect_gctl_server();
    send_request(fd, req);
    close(fd);

	return 0;
}

static int gctl_up_wxl_whitelist(int argc, char **argv)
{
    char req[64];
    int fd;

    if(argc != 2)
        return usage(argc, argv);

    memset(req, 0, sizeof(req));
    sprintf(req, "up-wxl-whitelist\r\n");
    fd = connect_gctl_server();
    send_request(fd, req);
    close(fd);

	return 0;
}

static int gctl_vap(int argc, char **argv)
{
    char req[64];
    int fd;

    if(argc < 2)
        return usage(argc, argv);

	sprintf(req, "vap-update\r\n");
	
    fd = connect_gctl_server();
    send_request(fd, req);
    close(fd);

	return 0;
}

static int gctl_running_status(int argc, char **argv)
{
    int fd;
    int done;
    int len;
    char req[128];
    char resp[4096];

    fd = connect_gctl_server();
    sprintf(req, "run-status\r\n");
    send_request(fd, req);

    done = 0;
    len = 0;
    while(!done)
    {
        memset(resp, 0, sizeof(resp));
        len = read(fd, resp, sizeof(resp));
        if(len > 0)
        {
            printf("%s\n", resp);
            len = 0;
        }
        else
        {
            done = 1;
        }
    }

    close(fd);
    return 0;
}



static int gctl_how_maro_roam_sta(int argc, char **argv)
{
        int fd;
        int done;
        int len;
        char req[128];
        char resp[4096];
    
        fd = connect_gctl_server();
        sprintf(req, "roam-sta\r\n");
        send_request(fd, req);
    
        done = 0;
        len = 0;
        while(!done)
        {
            memset(resp, 0, sizeof(resp));
            len = read(fd, resp, sizeof(resp));
            if(len > 0)
            {
                printf("%s", resp);
                len = 0;
            }
            else
            {
                done = 1;
            }
        }
    
        close(fd);
        return 0;
}




int main(int argc, char **argv)
{
    int loop;

    if(argc < 2)
    {
		config[GCTL_HELP].function(argc, argv);
		return 1;
    }

    for(loop = 0; loop < GCTL_MAX; loop++)
    {
        if(0 == strcmp(config[loop].options, argv[1]))
            return config[loop].function(argc, argv);
    }
    config[GCTL_HELP].function(argc, argv);
    return 1;
}
