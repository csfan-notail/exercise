/*
 * Copyright (C) GBCOM Co.,Ltd. 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <string.h>
#include <syslog.h>
#include <netinet/in.h>
#include <netdb.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include<sys/file.h>

#include "appdefs.h"
#include "utils.h"
#include "event.h"
#include "informer.h"
#include "SOAPParser/CPEframework.h"
#include "webproto/protocol.h"
#include "SOAPParser/RPCState.h"
#include "informer.h"
#include "manageServerParams.h"
#include "ap_types.h"

/* for debug */
#include "../SOAPParser/xmlParserSM.h"
#include "../SOAPParser/xmlTables.h"
LTEInfoState lteinfostate;
int g_device_first_up;   /*add bu duanguiyuan 20160328 �豸�Ƿ񿪻�����tr69cd 1:�ǿ���   0:���ǿ���*/
int protect_connect_state_flag; /*add by duanguiyuan ����connect_state�����޸� 1:����  0:������*/
int tr69cTerm = 0;	//TR69C termination flag
int g_debug = 0;        //debug flag add by zhanghaozhong 2013-5-18
                        //default g_debug = 0 (debug off )
int g_log_level = 7;    //default simple log to syslog . add by zhanghaozhong 2013-10-25

#define LOCKMODE    (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)	/* lock file status */
#define RPC_UPLOAD //do by duanguiyuan 20150901
void sigIgnore(int sig)
{
    slog(TR69C_LOG_DEBUG, "Ignore sig %d",sig);
    return;
}

void tr69c_sigTermHandler(int sig)
{
	//printf("TR69C terminating......\n");
 	tr69cTerm = 1;

	//Set the timer to send out an inform to ACS.  The purpose is to
	//create an ACS disconnect event so that the tr69c termination
	//flag will be examined and acted on.
	resetPeriodicInform(1);
}
extern ACSState    acsState;
extern int uiCFMfd;
extern void cfmKickListener(void *handle);

/*
* Initialize all the various tasks 
��ʼ�����еĸ�������
 */
static int initTasks(int isSleep)
{	
	protect_connect_state_flag = 0;
    /* BEGIN:  PN:����ҳ���TR069���� */
    char *value=NULL;
    getCWMPEnable(&value);
    if ( value != NULL ) 
    {
		free(value);
		value=NULL;
		
	}
    slog(TR69C_LOG_DEBUG,"informEnable:%d",acsState.informEnable);
	printf("informEnable    is    ######################################  %d\n",acsState.informEnable);
	if(!acsState.informEnable)
	{	
		slog(TR69C_LOG_INFO,"TR069 disable!!!!!!!");
		return 0;
	}
    /* init cfm listener */    
#if 0
//	initCFMListener();
    if (!acsState.informEnable)
        slog(TR69C_LOG_INFO,"TR069 disable!!!!!!!");
        
    while(!acsState.informEnable)
    {
        FD_ZERO(&rfds);
        FD_SET(uiCFMfd, &rfds);
        maxfd = uiCFMfd + 1;
        tm.tv_sec = 5;
        res = select(maxfd, &rfds, NULL, NULL, &tm);
        if (res > 0 && FD_ISSET(uiCFMfd, &rfds))
        {
    	    FD_CLR(uiCFMfd, &rfds);
            cfmKickListener((void *)uiCFMfd);
	    }
        slog(TR69C_LOG_DEBUG,"informEnable:%d",acsState.informEnable);
		//getCWMPEnable(&value);
    }
#endif
    /* END:    PN:����ҳ���TR069���� */
    /* init gbcom library interface */
    initBCMLibIF();     /* ��ʼ��ʵ�� */
    /* BEGIN: Deleted by zhangjiasi, 2015/6/16   PN:��ʱ������ͬʱ�������� */
    /*if (!isSleep)
        sleep(iadRandomTime(45, 300));  
        */
    /* END:   Deleted by zhangjiasi, 2015/6/16   PN:��ʱ������ͬʱ�������� */
                                                                             
    /* Just booted so send initial Inform */
    initInformer();	
    /* init nat listener */
    //initNatUdpListener();
    	return 1;
}

static void daemonize(void) {

    slog(TR69C_LOG_DEBUG,"Daemonizing process ");
    if (fork()!=0) exit(0);     /* fork һ������ */
    setsid();                   /* �ӽ���Ϊ�Ự�鳤 */

}

static int tr69cd_is_running(void)
{
    UINT32 tr69cd_fd;
	INT32 iRet;

    mkdirs(TR69_DIR);

	tr69cd_fd = open(TR69_PID_FILE, O_RDWR|O_CREAT, LOCKMODE);
	if(tr69cd_fd < 0)
	{
		slog(TR69C_LOG_ERR,"can't open file %s!\n", TR69_PID_FILE);
		return 1;
	}
    /* �����ļ� */
	iRet = flock(tr69cd_fd, LOCK_EX|LOCK_NB);
	if(0 != iRet){
		sleep(3);
		iRet = flock(tr69cd_fd, LOCK_EX|LOCK_NB);
        if(0 != iRet)
        {
            slog(TR69C_LOG_ERR,"lock file %s! failed", TR69_PID_FILE);
		    close(tr69cd_fd);
		    return 1;
        }
	}
	return 0;
    
}

void rpc_debug(char *file)
{
    eParseStatus    status;
    ParseHow    parseReq;

    fprintf(stderr, "Parse %s Start\n",file);
    parseReq.topLevel = envelopeDesc;
    parseReq.nameSpace = nameSpaces;
    status = parseGeneric(file,NULL,0,&parseReq);
    fprintf(stderr, "Parse status of %s is %s\n", file, status==NO_ERROR? "OK":
        "Bad");
    if (status==NO_ERROR) {
        fprintf(stderr, "runRPC Start\n");
        runRPC_test();
    }
}

static void help_show(void){
    puts("+=============================================================================+");
    puts("|  v  Log Print to Stdout                                                     |");
    puts("|  d  Proecss Run As a Daemon                                                 |");
    puts("|  D  Open Debug Mode                                                         |");
    puts("|  S  Start Tr69cd immediately                                                |");
    puts("|  l  Syslog Print level:Range{0~7},if 7,all log will be print                |");
    puts("|  f  debug file name                                                         |");
    puts("+=============================================================================+");
    exit(0);
}


/*****************************************************************************
 �� �� ��  : main
 ��������  : tr69c����������
 �������  : int argc     
             char** argv  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��5��16��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ɺ���

*****************************************************************************/
int main(int argc, char** argv)
{
	
    int verbose = 0;
    int no_daemonize=0;
    int iNoSleep = 0;
    char    c;
    char *file = NULL;
#ifdef RPC_UPLOAD    
 //printf("upload is running\n");
#endif   // RPC_UPLOAD 

    #ifdef WRITEPIDFILE
    if (0 != tr69cd_is_running() )
    {
        printf("tr69cd is running\n");
        slog(TR69C_LOG_ERR,"tr69cd is running");
        exit(0);
    }
    #endif
    /* -DΪ����ģʽ */
    while ((c=getopt(argc, argv, "hSdvDl:f:")) != -1) {
        switch (c) {
        case 'v':
            verbose = 1;        /* verboseΪ0ʱ��ӡ��syslog������ӡ����׼��� */
            break;
        case 'd':
            no_daemonize = 1;   /* no_daemonizeΪ0ʱ����Ϊ�ػ����̣�Ĭ������Ϊ�ػ����� */
            break;
        case 'D':
            g_debug = 1;   /* ����debug */
            break;
        case 'S':
            iNoSleep = 1;   /* ����debug */
            break;
        case 'l':
            g_log_level = atoi(optarg); /* ��d��Ϊ��ӡ��ϸ��־��syslog */            
            break;
        case 'f':
            file = optarg;
            break;
        case 'h':
            help_show();
            break;
        default:
            help_show();
            break;
        }
    }
    
    initLog(verbose);
    if (!no_daemonize)
        daemonize();    /* �ػ����� */

    /* set signal masks */
	signal(SIGPIPE, SIG_IGN); /* Ignore SIGPIPE signals */
	signal(SIGTERM, tr69c_sigTermHandler);      /* ����kill �źţ����ú���ĺ��� */
	signal(SIGTERM, tr69c_sigTermHandler);
    if(initTasks(iNoSleep)<1)
    {
		return 0;
	}
	
    if ( NULL != file )
    {
        rpc_debug(file);
      //  exit(0);
    }
    eventLoop();
    return 0;
}

