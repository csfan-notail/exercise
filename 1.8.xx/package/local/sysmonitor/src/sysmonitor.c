#include <stdio.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <stdbool.h>
#include <linux/watchdog.h>

/*
LOG_EMERG      system is unusable
LOG_ALERT      action must be taken immediately
LOG_CRIT       critical conditions
LOG_ERR        error conditions
LOG_WARNING    warning conditions
LOG_NOTICE     normal, but significant, condition
LOG_INFO       informational message
LOG_DEBUG      debug-level message
*/
#define sm_print(level, format, arg...) do { \
    if ((!gsm.daemonize) && (gsm.loglevel >= level)) {  \
        printf("%s:%d: "format, __func__, __LINE__, ##arg);    \
    }   \
    syslog(level,"[sysmonitor] "format, ##arg);	\
} while(0)

#define ETHDUMP_SYSTEM_MEM_CLEAN 35
#define PROC_SHORT_NAME_LEN 16
#define PROC_STATE_DEFAULT '-'
#define PROC_DEFAULT(_name, _fail_cnt_max, _rss_max, _cmdline) \
    { _name, -1, PROC_STATE_DEFAULT, 0, _fail_cnt_max, 0, _rss_max, 0, _cmdline, }

#define MOUNT_PACKET "mount |grep \"/dev/sda2\" |awk '{print$1}'"

struct sm_proc {
    char name[PROC_SHORT_NAME_LEN];
    int pid;
    char state;  /* process status, include R S D Z T X */
    unsigned char fail_cnt;
    unsigned char fail_cnt_max; /* reboot process, if fail_cnt > fail_cnt_max */
    unsigned long rss;  /*vmsize of proccess, unit is KB*/
    unsigned long rss_max;  /* reboot process, if rss > rss_max(if not 0). */
    unsigned int restart_cnt;  /* just for statictics */
    char cmd[1024];  /* the cmdline of proccess */
};

struct sm_globals {
    bool daemonize;
    int loglevel;
    int proc_cnt;
    struct sm_proc *procs;
};

struct sm_proc g_procs[] = {
    //PROC_DEFAULT("multicast",  	5, 0, "/etc/init.d/multi_cast restart"),
    //PROC_DEFAULT("wifidog",    	5, 0, "/etc/init.d/wifidog restart"),
    //PROC_DEFAULT("lighttpd",   	5, 0, "/etc/init.d/lighttpd restart"),
    //PROC_DEFAULT("aglink",     	5, 0, "/etc/init.d/aglink restart"),
    //PROC_DEFAULT("mini_snmpd", 	5, 0, "/sbin/mini_snmpd"),
    //PROC_DEFAULT("stam",       	5, 0, "/etc/init.d/stam restart"),
    PROC_DEFAULT("dnsmasq",    	5, 0, "/etc/init.d/dnsmasq restart"),
    PROC_DEFAULT("ltegetinfo", 	5, 0, "/sbin/ltegetinfo"),
    PROC_DEFAULT("lteauto", 	5, 0, "/sbin/lteauto"),
    PROC_DEFAULT("ledstatus", 	5, 0, "/sbin/ledstatus"),
    PROC_DEFAULT("simledstatus",   5, 0, "/sbin/simledstatus > /dev/null 2>&1"),
	PROC_DEFAULT("ltecomsev", 	5, 0, "/sbin/ltecomsev -m 15 -l 5"),
	//PROC_DEFAULT("ltecomsev2", 	5, 0, "/sbin/ltecomsev2 -m 15 -l 5"),
    //PROC_DEFAULT("tr069",   5, 0, "/sbin/tr069"),
    PROC_DEFAULT("logsave", 	5, 0, "/sbin/logsave"),		//gsm.procs[6]
    PROC_DEFAULT("ethdump", 	5, 0, ""),		//gsm.procs[7]
    PROC_DEFAULT("udpxy", 		5, 0, ""),		//gsm.procs[8]
    PROC_DEFAULT("pimd", 		5, 0, ""),		//gsm.procs[9]
    PROC_DEFAULT("ospfd", 		5, 0, ""),		//gsm.procs[10]
	//PROC_DEFAULT("tr069", 		5, 0, ""),		//gsm.procs[11]
	//PROC_DEFAULT("tr069_d", 		5, 0, ""),		//gsm.procs[12]
	PROC_DEFAULT("ltegetinfo2", 	5, 0, "/sbin/ltegetinfo2"),
	PROC_DEFAULT("lteauto2", 	5, 0, "/sbin/lteauto2"),
	PROC_DEFAULT("ledstatus2", 	5, 0, "/sbin/ledstatus2"),
	PROC_DEFAULT("ltecomsev2", 	5, 0, "/sbin/ltecomsev2 -m 15 -l 5"),
    
};

struct sm_globals gsm;


void sm_global_init(void)
{
    gsm.daemonize = false;
    gsm.loglevel = LOG_WARNING;

    gsm.proc_cnt = sizeof(g_procs)/sizeof(g_procs[0]);
    gsm.procs = g_procs;
    
    return;
}

int sm_parse_args(int argc, char *argv[])
{
    char op;

    while ((op = getopt(argc, argv, "Bdh")) != -1)
        switch (op) {
            case 'B':
                gsm.daemonize = true;
                break;
            case 'd':
                gsm.loglevel++;
                break;
            case 'h':
            default:
                return -1;
                //break;
        }

    return 0;
}

int sm_daemonize(void)
{
	pid_t pid;

	pid = fork();
	if (pid < 0){
		perror ("fork");
		exit(-1);
	}

    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
		exit(0);
	}

	/* Create a new SID for the child process */
	pid = setsid();
	if (pid < 0){
		perror ("setsid");
		exit(-1);
	}
	umask (0755);

	return 0;
}

void sm_feed_watchdog(void)
{
    static int fd = -1;
    static unsigned int wd_timeout = 60;    /* 60 seconds */

    if (fd < 0) {
        if ((fd = open("/dev/watchdog", O_WRONLY)) < 0) {
            perror("/dev/watchdog");
            exit(-1);
        }
        /*set close-on-exec flag to close fd in child process across execve*/
        fcntl(fd, F_SETFD, fcntl(fd,F_GETFD) | FD_CLOEXEC);
    }

    ioctl(fd, WDIOC_SETOPTIONS, (void*) &wd_timeout);

    write(fd, "", 1); /* write zero byte */
    
    return;
}

int sm_execute(const char *cmd)
{
    int ret;
    if(NULL == cmd){
    	return -1;	
    }
    if(0 == strcmp(cmd,"")){
		return -1;
	}
    sm_print(LOG_DEBUG, "excute \"%s\"\n", cmd);
    
    sm_feed_watchdog();
    
    ret = system(cmd);
    if (ret) {
        sm_print(LOG_ERR, "execute failed: %s\n", cmd);
    }

    return ret;
}

int sm_proc_get_cmdline(int pid, char *cmdline, int size) 
{
    FILE *fp;
    char path[32];
    int n;
    char *str;
    
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    
    if ((fp = fopen(path, "r")) == NULL)
    {
        sm_print(LOG_EMERG, "open %s error: %s\n", path, strerror(errno));
        return 1;
    }
    
    if ((n = fread(cmdline, 1, size - 1, fp)) > 0) 
    {
        if (cmdline[n-1] == '\n')
        {
            cmdline[--n] = 0;
        }
        str = cmdline;
        while (n) 
        {
            if(((unsigned char)*str) < ' ')
                *str = ' ';
            str++;
            n--;
        }
        *str = 0;
    }
    fclose(fp);

    return 0;

}    

void popen_ret(char *acCmd, char *retval)
{
	FILE *fp_p = NULL;
	char acBuff[32] = {0};
	fp_p = popen(acCmd,"r");
	if(fp_p){
		fgets(acBuff,32,fp_p);
		acBuff[strlen(acBuff)-1] = '\0';
		strcpy(retval,acBuff);
	}
	pclose(fp_p);
}

int  ethdump_handle()
{
	char acPacketSw[32] = {0};
	char acLanIp[32] = {0};
	char acWanIp[32] = {0};
	char acPacketTime[32] = {0};
	char acPacketSize[32] = {0};
	char acInterface[32] = {0};
	char cmd[128] = {0};
	char acMount[32] ={0};
	int iPacketSw = 0;
	popen_ret("uci get config.packet.packet_sw",acPacketSw);
	popen_ret("uci get config.packet.lan_ip",acLanIp);
	popen_ret("uci get config.packet.wan_ip",acWanIp);
	popen_ret("uci get config.packet.packet_time",acPacketTime);
	popen_ret("uci get config.packet.packet_size",acPacketSize);
	popen_ret("uci get config.packet.packet_int",acInterface);
	popen_ret(MOUNT_PACKET,acMount);
	iPacketSw = atoi(acPacketSw);
	
	if( 1== iPacketSw )
	{
		snprintf(cmd,sizeof(cmd),"/sbin/ethdump -l %s -w %s -t %s -f %s -i %s > /dev/null 2>&1 ",acLanIp,acWanIp,acPacketTime,acPacketSize,acInterface);
	}
	
	if(strcmp(acMount,"/dev/sda2") == 0)
	{
		memcpy(gsm.procs[7].cmd,cmd,strlen(cmd)+1);
	}
	else
	{
		printf("[%s:%d] ethdump start check failed !\n",__func__,__LINE__);
		return -1;
	}
	return 0;
}

int  multicast_handle()
{
	char get_sw[32] = {0};
	char get_interface[32] = {0};
	char cmd[64] = {0};
	
	popen_ret("uci get config.muticast.muticast_sw",get_sw);
	popen_ret("uci get config.muticast.dr_interface",get_interface);
	
	if (strlen(get_sw) == 1)
	{
		sprintf(cmd,"/sbin/pimd ; udpxy -p 8080 -a %s",get_interface);
		memcpy(gsm.procs[8].cmd,cmd,strlen(cmd)+1);
	}
	else
	{
		printf("[%s:%d] muticast start switch off !\n",__func__,__LINE__);
		return -1;
	}
	return 0;
}

int  ospfd_handle()
{
	char get_sw[32] = {0};
	char get_interface[32] = {0};
	char cmd[128] = {0};
	
	popen_ret("uci get config.ospf.ospf_sw",get_sw);
	
	if (strlen(get_sw) == 1)
	{
		sprintf(cmd,"%s","/etc/init.d/quagga start;/usr/sbin/ospfd -d -f /etc/quagga/ospfd.conf");
		memcpy(gsm.procs[10].cmd,cmd,strlen(cmd)+1);
	}else{
		printf("[%s:%d] ospfd start switch off !\n",__func__,__LINE__);	
	}

	return 0;
}

int tau_process(char *pcProcess)
{
	char *pcTmp;
	int iRet = 0;
	
    char acProPimd[] = "pimd";
    char acProUdpxy[] = "udpxy";
	char acProEthdump[] = "ethdump";
	char acProOspfd[] = "ospfd";
	
	printf("[%s:%d] %s check\n",__func__,__LINE__,pcProcess);
	if(strstr(pcProcess,acProPimd) || strstr(pcProcess,acProUdpxy))
	{
		printf("[%s:%d] %s start\n",__func__,__LINE__,acProUdpxy);
		iRet = multicast_handle();
		if(iRet < 0)
		{
			return -1;
		}
	}else if(strstr(pcProcess,acProEthdump)){
		printf("[%s:%d] %s start\n",__func__,__LINE__,acProEthdump);
		iRet = ethdump_handle();
		if(iRet < 0)
		{
			return -1;
		}
	}else if(strstr(pcProcess,acProOspfd)){
		printf("[%s:%d] %s start\n",__func__,__LINE__,acProOspfd);
		iRet = ospfd_handle();
		if(iRet < 0)
		{
			return -1;
		}
	}
	
	return 0;
}

int sm_proc_start(struct sm_proc *proc, bool restart)
{
    const char *cmd = proc->cmd[0] ? proc->cmd : proc->cmd;
    char *buf = NULL;
    int ret = 1;
	if(NULL == cmd){
		
		return -1;
	}
    if(0 == strcmp(cmd,"")){
		
		return -1;
	}
	
	ret = tau_process(cmd);
	if (ret == -1)
	{
		return ret;
	}
    buf = calloc(sizeof(char), strlen(cmd) + 2);
    if (buf)
    {
        if (restart)
        {
            kill(proc->pid, SIGKILL);
        }
        
        sprintf(buf, "%s&", cmd);        
        ret = sm_execute(buf);
        free(buf);
        
        if (ret == 0)
        {
            proc->restart_cnt++;
            proc->fail_cnt = 0;
            sm_print(LOG_NOTICE, "start %s %d times\n", proc->name, proc->restart_cnt);
        }
    }
    else
    {
        sm_print(LOG_ERR, "malloc failed\n");
    }

    return ret;
}

void sm_proc_check_vmsize(struct sm_proc *proc)
{
    if ((proc->rss_max == 0) || (proc->rss < proc->rss_max)) {
        return ;  /* vmsize is normal,do nothing */
    }
    
    sm_print(LOG_WARNING, "%s occupies vm(%lu KB), larger than %lu, restart...", 
        proc->name, proc->rss, proc->rss_max);

    sm_proc_start(proc, true);
    return ;
}

/*
linux-2.6.31
	"R (running)",
	"S (sleeping)",
	"D (disk sleep)",
	"T (stopped)",
	"T (tracing stop)",
	"Z (zombie)",
	"X (dead)"

linux-3.3.8
    "R (running)",
    "S (sleeping)",
    "D (disk sleep)",
    "T (stopped)",
    "t (tracing stop)",
    "Z (zombie)",
    "X (dead)",
    "x (dead)",
    "K (wakekill)",
    "W (waking)",
*/
void sm_proc_check(struct sm_proc *proc)
{  
    //sm_print(LOG_DEBUG, "%-16s %-5d %c %-8lu %s\n", proc->name, proc->pid, 
    //    proc->state, proc->rss, proc->cmd);

    switch (proc->state)
    {
        case 'S' : /* TASK_RUNNING */
        case 'R' : /* TASK_INTERRUPTIBLE */
        case 't' : /* TASK_TRACED */    
            if (proc->fail_cnt > 0)
            {
                proc->fail_cnt = 0;
            }
            sm_proc_check_vmsize(proc);
            break;
        case 'D' : /* TASK_UNINTERRUPTIBLE */
        case 'T' : /* TASK_STOPPED or TASK_TRACED */
        case 'X' : /* TASK_DEAD - EXIT_DEAD */
            break; /* do nothing now */
        case 'Z' : /* TASK_DEAD - EXIT_ZOMBIE */
            /* fall through */
        default:
            /* the process quit, run it again */
            proc->fail_cnt++;
            sm_print(LOG_NOTICE, "check %s failed for %d times\n", 
                proc->name, proc->fail_cnt);
             
            if (proc->fail_cnt >= proc->fail_cnt_max)
            {
                sm_proc_start(proc, false);
            }
    }

    return;
}

/*
Table 1-4: Contents of the stat files (as of 2.6.30-rc7) -- proc/%d/stat
..............................................................................
index Field          Content
1     pid           process id
2     tcomm         filename of the executable
3     state         state (R is running, S is sleeping, D is sleeping in an uninterruptible wait, Z is zombie, T is traced or stopped)
4     ppid          process id of the parent process
5     pgrp          pgrp of the process
6     sid           session id
7     tty_nr        tty the process uses
8     tty_pgrp      pgrp of the tty
9     flags         task flags
10    min_flt       number of minor faults
11    cmin_flt      number of minor faults with child's
12    maj_flt       number of major faults
13    cmaj_flt      number of major faults with child's
14    utime         user mode jiffies
15    stime         kernel mode jiffies
16    cutime        user mode jiffies with child's
17    cstime        kernel mode jiffies with child's
18    priority      priority level
19    nice          nice level
20    num_threads   number of threads
21    it_real_value	(obsolete, always 0)
22    start_time    time the process started after system boot
23    vsize         virtual memory size
24    rss           resident set memory size
25    rsslim        current limit in bytes on the rss
26    start_code    address above which program text can run
27    end_code      address below which program text can run
28    start_stack   address of the start of the stack
29    esp           current value of ESP
30    eip           current value of EIP
40    pending       bitmap of pending signals
41    blocked       bitmap of blocked signals
42    sigign        bitmap of ignored signals
43    sigcatch      bitmap of catched signals
44    wchan         address where process went to sleep
45    0             (place holder)
46    0             (place holder)
47    exit_signal   signal to send to parent thread on exit
48    task_cpu      which CPU the task is scheduled on
49    rt_priority   realtime priority
50    policy        scheduling policy (man sched_setscheduler)
51    blkio_ticks   time spent waiting for block IO
52    gtime         guest time of the task in jiffies
53    cgtime        guest time of the task children in jiffies
..............................................................................

e.g.
2095 (nfsd) S 2 0 0 0 -1 2150629440 0 0 0 0 0 0 0 0 20 0 1 0 5853 0 0 18446744073709551615 0 0 0 0 0 0 0 2147483384 263 18446744073709551615 0 0 17 3 0 0 0 0 0
*/
void sm_check_process(void)
{
    struct sm_proc *proc;
    struct dirent *entry;
    DIR *dir;
    FILE *fp;
    char *str;
    char proc_name[PROC_SHORT_NAME_LEN];
    char path[32];
    char buf[1024];
    int pid;
    int i;

    dir = opendir("/proc");
    if (!dir) {
        perror("/proc");
        exit(-1);        
    }

    while ((entry = readdir(dir)) != NULL) {
        str = entry->d_name;

        while ((*str >= '0') && (*str <= '9'))
            str++;
        if (*str != '\0')
            continue;

        if ((pid = atoi(entry->d_name)) == 0)
            continue;

        snprintf(path, sizeof(path), "/proc/%d/stat", pid);
        if ((fp = fopen(path, "r")) == NULL)
        {
            sm_print(LOG_WARNING, "Can't open %s\n", path);
            continue;
        }

        memset(buf, 0, sizeof(buf));
        str = fgets(buf, sizeof(buf), fp);
        fclose(fp);
        
        if (str == NULL) {
            continue;
        }
        
        str = strrchr(buf, ')'); /* split into "PID (cmd" and "<rest>" */
        if (str == NULL || str[1] != ' ') {
            continue;
        }
        *str = 0;
        str += 2;
        
        memset(proc_name, 0, sizeof(proc_name));
        sscanf(buf, "%*s (%15c", proc_name);

        proc = NULL;
        for(i = 0; i < gsm.proc_cnt; i++) {
            if (0 == strncmp(proc_name, gsm.procs[i].name, sizeof(proc_name)))
            {
                proc = &gsm.procs[i];
                break;
            }
        }
        
        if (i == gsm.proc_cnt)
        {
            continue; /* didn't match any proccess we monitor */
        }

        if (2 != sscanf(str, "%c %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s "
                        "%*s %*s %*s %*s %*s %*s %*s %*s %*s %ld",
                        &proc->state, &proc->rss))
        {
            continue;
        }

#ifdef PAGE_SHIFT
        proc->rss <<= (PAGE_SHIFT - 10);       /* 2**10 = 1kb */
#else
        proc->rss *= (getpagesize() >> 10);    /* 2**10 = 1kb */
#endif
        proc->pid = pid;

        if (proc->cmd[0] == 0) {
            sm_proc_get_cmdline(pid, proc->cmd, sizeof(proc->cmd));
        }
    }
    closedir(dir);

    for (i = 0; i < gsm.proc_cnt; i++) {
        sm_proc_check(&gsm.procs[i]);
        
        gsm.procs[i].pid = -1;
        gsm.procs[i].state = PROC_STATE_DEFAULT;
        gsm.procs[i].rss = 0;
    }

    return;
}

void sm_check_system(void)
{

    return;
}

void sm_usage()
{
    printf(
        "Usage: sysmonitor [-Bdh]\n"
        "\n"
        "Options:\n"
        "   -B      run daemon in the background\n"
        "   -d      show more debug messages (-dd for even more)\n"
        "   -h      show this usage\n"
    );

    return;
}

int ethdump_check_memory_size()
{
 struct sysinfo s_info;
 int error;
 error = sysinfo(&s_info);
 
   if(s_info.freeram/1024/1024 < ETHDUMP_SYSTEM_MEM_CLEAN)
   {
    printf("\n\ncode error=%d\n",error);  
    printf("[%s:%d] total:%d free:%d\n",__func__,__LINE__,s_info.totalram/1024/1024, s_info.freeram/1024/1024);
    return -1;
   }
   
 return 0;
}


#define CPU_ALARM_LIMIT 70
static char g_cpu_stat[128] ={0};
static int iCpuCount=0;
int get_proc_stat(char* buf,int len)
{
	FILE *fp;
 	fp = fopen("/proc/stat","r");  
    if(fp == NULL)  
    {  
       return -1; 
    }  
	fgets(buf,len,fp);  
	fclose(fp);
	return 0;
}

/*	 ??：：?CPU：o1：???：o	*/
int get_cpu_usage(void)
{	
	int cpu_use=0;
	char acTmp[128] ={0};
	char acCmd[32] ={0};
//get_proc_stat
 	long int iUser,nice,sys,idle,iowait,irq,softirq; 
  	long int all1,all2,idle1,idle2;  
    float usage;  
	char cpu[32];

	if(get_proc_stat(acTmp,sizeof(acTmp))==0)
	{
		//printf("cpu 2 [%s]\n",acTmp);
		sscanf(acTmp,"%s%d%d%d%d%d%d%d",cpu,&iUser,&nice,&sys,&idle,&iowait,&irq,&softirq);  
		all2 = iUser+nice+sys+idle+iowait+irq+softirq;  
        idle2 = idle;  

		//printf("cpu 1 [%s]\n",g_cpu_stat);
		sscanf(g_cpu_stat,"%s%d%d%d%d%d%d%d",cpu,&iUser,&nice,&sys,&idle,&iowait,&irq,&softirq);  
		all1 = iUser+nice+sys+idle+iowait+irq+softirq;  
        idle1 = idle;

		usage = (float)(all2-all1-(idle2-idle1)) / (all2-all1)*100 ;  
        cpu_use=(int)usage;  
		if( cpu_use > CPU_ALARM_LIMIT )
		{
			if(iCpuCount > 3)
			{
				sprintf(acCmd,"echo %d > /tmp/cpuinfo",cpu_use);
				system(acCmd);
				iCpuCount = 0;
				memset(g_cpu_stat,0,sizeof(g_cpu_stat));
				sprintf(g_cpu_stat,"%s",acTmp);
				return 0;
				
			}
			iCpuCount++;				
		}
		else
		{
				sprintf(acCmd,"echo %d > /tmp/cpuinfo",cpu_use);
				system(acCmd);
				iCpuCount = 0;
				memset(g_cpu_stat,0,sizeof(g_cpu_stat));
				sprintf(g_cpu_stat,"%s",acTmp);
				return 0;
			
		}
	}
	
} 
int main(int argc, char *argv[])
{
    int iRet;
    sm_global_init();
        
    if (sm_parse_args(argc, argv)) {
        sm_usage();
        return -1;
    }

    if (gsm.daemonize) {
        sm_daemonize();
    }
	get_proc_stat(g_cpu_stat,sizeof(g_cpu_stat));
    for(;;) {
    	printf("[%s:%d] process check !\n",__func__,__LINE__);	
        sm_feed_watchdog();
		sleep(1);
        sm_check_process();

		sleep(1);
        iRet = ethdump_check_memory_size();
        if(0!=iRet)
        {
            system("sync");
            system("echo 3 > /proc/sys/vm/drop_caches ");
            syslog(LOG_INFO,"[%s:%d] system memory clean !\n", __func__, __LINE__);
        }
		sleep(1);
		get_cpu_usage();
		sleep(7);
    } /* for (;;) */
    /* not reached */
    return 0;
}


