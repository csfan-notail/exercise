/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : commandline.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>

#include "includes.h"
#include "config.h"
#include "log.h"
#include "conf.h"
#include "util.h"



char ** restartargv = NULL;
extern  int g_debug_level;
/*
 * A flag to denote whether we were restarted via a parent wifidog, or started normally
 * 0 means normally, otherwise it will be populated by the PID of the parent
 */
pid_t restart_orig_pid = 0;
extern int g_daemon;

static void usage(void);


/**
 * @帮助信息显示
 */
static void usage(void)
{
    printf("Usage: wifidog [options]\n");
    printf("\n");
    printf("  -c [filename] Use this config file\n");
    printf("  -f            Run in foreground\n");
    printf("  -d <level>    Debug level\n");
    printf("  -s            Log to syslog\n");
    //printf("  -w <path>     Wdctl socket path\n");
    printf("  -h            Print usage\n");
    printf("  -v            Print version information\n");
    //printf("  -x pid        Used internally by WiFiDog when re-starting itself *DO NOT ISSUE THIS SWITCH MANUAlLY*\n");
    //printf("  -i <path>     Internal socket path used when re-starting self\n");
    printf("\n");
}

/**
 * @命令行处理函数
 */
void parse_commandline(int argc, char **argv)
{
    char c;
    int i;

    s_config *config;

    config = config_get_config();

    restartargv = safe_malloc((argc + 3) * sizeof(char*));
	i=0;
	restartargv[i++] = safe_strdup(argv[0]);
    while (-1 != (c = getopt(argc, argv, "c:hfd:sv"))) 
    {
		switch(c) 
		{
			case 'h':
				usage();
				sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at parse_commandline() h");
				exit(1);
				break;
			case 'c':
				if (optarg) {
					strncpy(config->configfile, optarg, sizeof(config->configfile));
				}
				break;
			case 'w':
				if (optarg) {
					free(config->wdctl_sock);
					config->wdctl_sock = safe_strdup(optarg);
				}
				break;
			case 'f':
				config->daemon = 0;
                g_daemon = 0;
				break;
			case 'd':
				if (optarg) {
					config->debuglevel = atoi(optarg);
					g_debug_level = atoi(optarg);
				}
				break;
			case 's':
				config->log_syslog = 1;
				break;
			case 'v':
				printf("This is WiFiDog version " VERSION "\n");
				sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at parse_commandline() v.");
				exit(1);
				break;
			case 'x':
				if (optarg) {
					restart_orig_pid = atoi(optarg);
				}
				else {
					printf("The expected PID to the -x switch was not supplied!");
					sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at parse_commandline() x.");
					exit(1);
				}
				break;
			case 'i':
				if (optarg) {
					free(config->internal_sock);
					config->internal_sock = safe_strdup(optarg);
				}
				break;
			default:
				usage();
				sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at parse_commandline() default.");
				exit(1);
				break;
		}
    }
    
    /* Finally, we should add  the -x, pid and NULL to restartargv
	 * HOWEVER we cannot do it here, since this is called before we fork to background
	 * so we'll leave this job to gateway.c after forking is completed
	 * so that the correct PID is assigned
	 *
	 * We add 3 nulls, and the first 2 will be overridden later
	 */
	restartargv[i++] = NULL;
	restartargv[i++] = NULL;
	restartargv[i++] = NULL;
}

