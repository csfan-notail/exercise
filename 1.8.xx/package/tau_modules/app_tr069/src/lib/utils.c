/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <time.h>
#include <utime.h>
#include <signal.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <linux/if.h>
#include <syslog.h>

#include "appdefs.h"
#include "utils.h"
#include "md5.h"
/*
//#define DEBUG
if (g_debug)
#define DBGPRINT(X) fprintf X
#else
#define DBGPRINT(X)
#endif
*/

#ifdef DMALLOC
#include "dmalloc.h"
#endif
/* forwards */
static void generateBasicAuth( SessionAuth *sa, char *user, char* pwd);
static void generateCnonce(char **cnonceBuf);

static int  verbose;

size_t b64_encode(const char *inp, size_t insize, char **outptr);

#if 0
void slog(int level, const char* fmt, ...)
{
	va_list ap;
	va_start(ap,fmt);
	if (verbose) {
        if (g_debug)
        {
        		vfprintf(stderr, fmt, ap);
        		fprintf(stderr, "\n");
        }
	}
	vsyslog(level, fmt, ap);
	va_end(ap);
}
#endif

#include <stdarg.h>
void mBUG(char *format,...){
 va_list args;
    FILE *fp;
 
    fp = fopen("/var/log/tr69log","a+");
    if(!fp){
        return;
    }
    va_start(args,format);
	//time(&timep);
   // p=localtime(&timep);
    
 //	fprintf(fp,"%2d:%2d:%2d",p->tm_hour,p->tm_min,p->tm_sec);

    vfprintf(fp,format,args);
    va_end(args);
    fprintf(fp,"\n");
    fflush(fp);
    fclose(fp);
}

 #define mBUG(arg...)	mBUG("[tr69c][%s][%d]%s", __func__,__LINE__,##arg);

void vlog(int level, const char* fmt, va_list ap)
{
	if (verbose) {
		vfprintf(stderr, fmt, ap);
	}
	vsyslog(level, fmt, ap);
}

void initLog(int flag)
{
	verbose = flag;
//	openlog( "user", LOG_CONS,LOG_DAEMON);
	//Option LOG_CONS will cause syslog to redirect messages
	//to system console when syslog is disable.
	openlog( "tr69c",LOG_CONS, LOG_DAEMON);
}
extern int g_log_level;
/*
 * dns lookup that uses gethostbyname -- this is blocking call
 * return should always be true.
 */
int dns_lookup(const char *name, tIpAddr *res)
{
	struct hostent *netent;

	if ( (netent = gethostbyname( name ))) {
		*res = ntohl( (int)*(int *)*netent->h_addr_list);
	} else{
		*res =0;
        if (g_log_level)
            slog(TR69C_LOG_ERR, "dns_lookup ERR......\n");
    }
    slog(TR69C_LOG_DEBUG, "dns_lookup(%s) = %s\n", name, writeIp(*res));
	return 1;
}


const u_char zeroMac[6] = {0, 0, 0, 0, 0, 0};

static int read_if(char *interface, u_int32_t *addr)
{
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *our_ip;

	memset(&ifr, 0, sizeof(struct ifreq));
	if((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		ifr.ifr_addr.sa_family = AF_INET;
		strcpy(ifr.ifr_name, interface);
		if (addr) { 
			if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
				our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
				*addr = ntohl(our_ip->sin_addr.s_addr);
				/*slog(TR69C_LOG_DEBUG, "%s (our ip) = %s", 
				    ifr.ifr_name, inet_ntoa(our_ip->sin_addr));*/
			} else {
				slog(LOG_ERROR, "SIOCGIFADDR failed, interface %s: %s",ifr.ifr_name, 
						strerror(errno));
				return -1;
			}
		}
	} else {
		slog(LOG_ERROR, "interface %s socket failed!: %s",ifr.ifr_name, strerror(errno));
		return -1;
	}
	close(fd);
	return 0;
}

int getWanInfo( WanState *wanState )
{
	FILE *f;
	char  line[256];
	size_t   size;
	char  iface[30];
	char  dest[30];
	unsigned  gw;
	unsigned flags, refcnt, use, metric, mask, window, mtu, irtt;
	char      *lineptr = line;

	size = sizeof(line);
	if (wanState->name) {
		free(wanState->name);
		wanState->name=NULL;
		wanState->name = NULL;
	}
	
	if ((f=fopen("/proc/net/route", "r"))) {
		
		while ( (getline( &lineptr, &size, f)>=0) ) {
			/* iface destination gateway flags refcnt use metric mask mtu window irtt*/
			if ( sscanf(lineptr, "%s %s %x %x %x %x %x %x %x %x %x",
						iface, dest, &gw, &flags, &refcnt, &use, &metric, &mask, &mtu, &window, &irtt) 
				 == 11 ) {
				 
				if (streq(dest,"00000000")) {
					
					wanState->gw = gw;
					wanState->name = strdup(iface);
					wanState->mask = mask;

					read_if(wanState->name, &wanState->ip);
					//slog(TR69C_LOG_DEBUG, "WanIF up %s %s", wanState->name, writeIp(wanState->ip));
					break;
				}
			}
		}
		fclose(f);
	}
	
	return wanState->name!=NULL;
}
/**********************************************************************
 * Files
 **********************************************************************/
/*--------------------
 * returns 1 if files differ 0 otherwise
 */
int hasChanged(const char* new, const char* old)
{
	int ret = 0;
	FILE* fn = fopen(new, "r");
	FILE* fo = fopen(old, "r");
	char bufn[1024];
	char bufo[1024];

	if (!fn) {
		ret = 0;
	} else if (!fo) {
		ret = 1;
	} else {
		for (;;) {
			int nn = fread(bufn, 1, sizeof bufn, fn);
			int no = fread(bufo, 1, sizeof bufo, fo);
			if (nn != no) {
				ret = 1;
				break;
			}

			if (nn == 0) {
				break;
			}

			if (memcmp(bufn, bufo, nn)) {
				ret = 1;
				break;
			}
		}
	}
	if (fn) {
		fclose(fn);
	}
	if (fo) {
		fclose(fo);
	}
	return ret;
}

/*--------------------*/
int mkdirs(const char *path)
{
	struct stat sb;
	char buf[256];
	char* p;

	if (stat(path, &sb) && errno == ENOENT) {
		strcpy(buf, path);
		p = buf + strlen(buf) - 1;
		while (p > buf && *p=='/') p--;
		while (p >= buf && *p!='/')	p--;
		while (p > buf && *p=='/') p--;

		if (p < buf) {
			p = buf;
			*p = '.';
		}
		p[1] = 0;

		if (mkdirs(buf))
			return -1;

		if (mkdir(path, 0777))
			return -1;
	}
	return 0;
}

/*--------------------*/
static void rmrf1(char** p, int* max, int n)
{
	struct stat sb;

	(*p)[n] = 0;
	if (lstat(*p,&sb))
		return;

	if (S_ISDIR(sb.st_mode)) {
		DIR *dp = opendir(*p);
		if (dp) {
			struct dirent *de;
			int k = n;
			if (strcmp(*p,"/")) {
				(*p)[n] = '/';
				k = n + 1;
			}
			while ((de = readdir(dp))) {
				char* f = de->d_name;
				if (strcmp(f, ".") && strcmp(f, "..")) {
					int m = k + strlen(f);
					while (*max <= m) {
						*max *= 2;
						*p = realloc(*p, *max);
					}
					strcpy(*p + k, f);
					rmrf1(p, max, m);
				}
			}
			closedir(dp);
		}
		(*p)[n] = 0;
		rmdir(*p);
	} else {
		unlink(*p);
	}
}

/*--------------------*/
void rmrf(const char* path)
{
	int n = strlen(path);
	int len = n > 254 ? 2*n : 256;
	char* buf=malloc(len);

	strcpy(buf, path);
	while ( n > 1 && buf[n-1] == '/') {
		n--;
	}
	rmrf1(&buf, &len, n);
	free(buf);
	buf=NULL;
}

/**********************************************************************
 * Time
 **********************************************************************/
/*--------------------*/
static int DSTflag;
static int  timeoffset;

time_t getCurrentTime()
{
	time_t now;
	struct tm *t;
	now = time(NULL);
	t=localtime(&now);
	DSTflag = t->tm_isdst;
	timeoffset=timezone;
	return now-timeoffset+(DSTflag==1?3600:0); 
}

char *getXSIdateTime(time_t *tp)
{
	static char    buf[30];
	int     c;
	struct tm   *tmp;
	tmp = localtime(tp);
	c = strftime(buf,sizeof(buf),"%Y-%m-%dT%H:%M:%S%z",tmp );
	/* fix missing : in time-zone offset-- change -500 to -5:00 */
	memmove(&buf[c-1],&buf[c-2],3);
	buf[c-2]=':';
	return buf;
}

/*--------------------*/
int cmpTime(struct timeval* t1, struct timeval* t2)
{
	int n = t1->tv_sec - t2->tv_sec;
	if (n) {
		return n;
	}
	return t1->tv_usec - t2->tv_usec;
}

/*--------------------*/
/* add Milliseconds   */
void addMs(struct timeval* t1, struct timeval* t2, int ms)
{
	t1->tv_sec  = t2->tv_sec  + ms/1000;
	t1->tv_usec = t2->tv_usec + 1000*(ms%1000);
	if (t1->tv_usec >= 1000000) {
		t1->tv_sec  += 1;
		t1->tv_usec -= 1000000;
	}
}

/*--------------------*/
void subTime(struct timeval* t1, struct timeval* t2)
{
	t1->tv_sec  = t2->tv_sec  - t1->tv_sec;
	t1->tv_usec = t2->tv_usec - t1->tv_usec;
	if (t1->tv_usec < 0) {
		t1->tv_sec  -= 1;
		t1->tv_usec += 1000000;
	}
}

/**********************************************************************
 * hex
 **********************************************************************/
static char hex[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

const char *util_StringToHex(const char *s)
{
	static char buf[256];
	char *p;

	if (strlen(s) > 254) return NULL;
	p = buf;
	while (*s) {
		*p++ = hex[((*s & 0xF0) >> 4)];
		*p++ = hex[(*s & 0x0F)];
		s++;
	}
	*p = '\0';
	return buf;
}

/**********************************************************************
 * Addresses
 **********************************************************************/
/*--------------------*/
static int hexChar(int c)
{
	if (c >= 'A') {
		if (c >= 'a') {
			return c <= 'f' ? c - 'a' + 10 : -1;
		} else {
			return c <= 'F' ? c - 'A' + 10 : -1;
		}
	} else {
		return c >= '0' && c <= '9' ? c - '0' : -1;
	}
}

/*--------------------*/
void readMac(u_char* mac, const char* val)
{
	u_char* emac = mac + 6;
	int flag = 0;
	int b = 0;

	while (*val && mac < emac) {
		int c = hexChar(*val++);
		if (c >= 0) {
			b = (b << 4) | c;
			if (flag) {
				*mac++ = b;
			}
			flag = !flag;
		}
	}
}

/*--------------------*/
int readIp(const char* ip)
{
	int n = 0;
	int res = 0;

	while (n < 4 && *ip) {
		if (isdigit(*ip)) {
			res = (res << 8) | atoi(ip);
			n++;
			while (isdigit(*ip)) {
				ip++;
			}
		} else {
			ip++;
		}
	}
	return res;
}

/*--------------------*/
int readProto(const char* val)
{
	if (!strcmp(val,"udp") || !strcmp(val, "UDP")) {
		return IPPROTO_UDP;
	}

	if (!strcmp(val,"icmp") || !strcmp(val, "ICMP")) {
		return IPPROTO_ICMP;
	}

	return IPPROTO_TCP;
}

/*--------------------*/
int readMask(const char *mask)
{
	int ip = readIp(mask);
	int count = 1;
	int bit = 1;

	while ((bit = (ip << 1))) {
		if (bit) count++;
		ip = (ip << 1);
	}
	return count;
}

/*--------------------*/
char* writeMac(const u_char* mac)
{
	static char buf[24];

	sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",
			mac[0], mac[1], mac[2] ,mac[3], mac[4], mac[5]);
	return buf;
}

/*--------------------
 * writes MAC in canonical form (mosquito definition),
 * 12 characters denoting macaddress, no colon or other delimiters, all lower case
 */
char* writeCanonicalMac(const u_char* mac)
{
	static char buf[24];
	char *p;

	sprintf(buf,"%02x%02x%02x%02x%02x%02x",
			mac[0], mac[1], mac[2] ,mac[3], mac[4], mac[5]);
	p = buf;
	/* while ((*p = tolower(*p++)));  The %x is lower case abcdef This is causing an error with BRCMSW */

	return buf;
}

/*--------------------*/
char* writeQMac(const u_char* mac)
{
	static char buf[24];

	sprintf(buf,"%02x\\:%02x\\:%02x\\:%02x\\:%02x\\:%02x",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return buf;
}

/*--------------------*/
void writeIp_b(int ip, char *buf)
{
	sprintf(buf,"%d.%d.%d.%d",
			0xff&(ip>>24), 0xff&(ip>>16), 0xff&(ip>>8), 0xff&ip);
}

/*--------------------*/
char* writeIp(int ip)
{
	static char buf[24];
	writeIp_b(ip, buf);

	//sprintf(buf,"172.16.0.1");
	return buf;
}

/*--------------------*/
char* writeNet(int ip, int bits)
{
	static char buf[24];

	ip &= INADDR_BROADCAST << (32 - bits);
	sprintf(buf,"%d.%d.%d.%d",
			0xff&(ip>>24), 0xff&(ip>>16), 0xff&(ip>>8), 0xff&ip);
	return buf;
}

/*--------------------*/
char* writeBcast(int ip, int bits)
{
	static char buf[24];

	ip |= INADDR_BROADCAST >> bits;
	sprintf(buf,"%d.%d.%d.%d",
			0xff&(ip>>24), 0xff&(ip>>16), 0xff&(ip>>8), 0xff&ip);
	return buf;
}

/*--------------------*/
char* writeMask(int bits)
{
	static char buf[24];
	int ip = INADDR_BROADCAST << (32 - bits);

	sprintf(buf,"%d.%d.%d.%d",
			0xff&(ip>>24), 0xff&(ip>>16), 0xff&(ip>>8), 0xff&ip);
	return buf;
}

/*--------------------*/
char* writeRevNet(int ip, int bits)
{
	static char buf[24];
	char* p = buf;

	if (bits % 8) {
		slog(LOG_ERROR,"Cannot split FQDN %s/%d", writeIp(ip), bits);
	}

	ip >>= 32 - bits;
	while (bits > 0) {
		if (p > buf) {
			*p++ = '.';
		}
		sprintf(p, "%d", 0xff&ip);
		while (*p) {
			p++;
		}
		ip >>= 8;
		bits -= 8;
	}
	return buf;
}

/*--------------------*/
char* writeRevHost(int ip, int bits)
{
	static char buf[24];
	char* p = buf;

	if (bits % 8) {
		slog(LOG_ERROR ,"Cannot split FQDN %s/%d", writeIp(ip), bits);
	}

	bits = 32 - bits;
	while (bits > 0) {
		if (p > buf) {
			*p++ = '.';
		}
		sprintf(p, "%d", 0xff&ip);
		while (*p) p++;
		ip >>= 8;
		bits -= 8;
	}
	return buf;
}

/*--------------------*/
char* writeProto(int proto)
{
	switch (proto) {
	case IPPROTO_UDP:
		return "udp";
	case IPPROTO_ICMP:
		return "icmp";
	case IPPROTO_TCP:
	default:
		return "tcp";
	}
}

/**********************************************************************
 * Text formatting
 *********************************************************************/
/*--------------------*/
void readHash(u_char* hash, const char* val)
{
	u_char* ehash = hash + MD5_DIGEST_LEN;
	int flag = 0;
	int b = 0;

	while (*val && hash < ehash) {
		int c = hexChar(*val++);
		if (c >= 0) {
			b = (b << 4) | c;
			if (flag) {
				*hash++ = b;
			}
			flag = !flag;
		}
	}
}

/*--------------------*/
char* writeQHash(const u_char* hash)
{
	int i;
	static char buf[128];
	char* p = buf;

	for (i=0; i < MD5_DIGEST_LEN; i++) {
		if (i > 0) {
			*p++ = '\\';
			*p++ = ':';
		}
		sprintf(p, "%02x", hash[i]);
		p += 2;
	}
	return buf;
}

/*--------------------*/
char* unquoteText(const char* t)
{
	int len = 0;
	char* t1;
	const char* p;
	char* p1;

	for (p = t; *p; p++) {
		if (*p != '\\') {
			len++;
		}
	}
	t1 = malloc(len + 1);

	for (p = t, p1 = t1; *p; p++) {
		if (*p != '\\') {
			*p1++ = *p;
		}
	}
	*p1 = 0;

	return t1;
}

/*--------------------*/
char* quoteText(const char* t)
{
	int len = 0;
	char* t1;
	const char* p;
	char* p1;

	for (p = t; *p; p++) {
		len += *p == ':' ? 2 : 1;
	}
	t1 = malloc(len + 1);

	for (p = t, p1 = t1; *p; p++) {
		if (*p == ':') {
			*p1++ = '\\';
		}
		*p1++ = *p;
	}
	*p1 = 0;

	return t1;
}

/**********************************************************************
 * Processes
 **********************************************************************/
void do_cmd(int logon, const char *cmd, char *fmt, ...)
{
	va_list ap;
	char msg[1024];
	char *p;

	strcpy(msg, cmd);
	p = msg + strlen(cmd);
	*p++ = ' ';

	va_start(ap, fmt);
	vsnprintf(p, sizeof msg, fmt, ap);
	va_end(ap);
	if (logon)
		slog(TR69C_LOG_INFO, "system(\"%s\")", msg);
	system(msg);
}

/*--------------------*/
void runScript(const char* name)
{
	char buf[1024];
	sprintf(buf, "%s %s", SHELL, name);
#ifdef BOX_TBOXxxxx
	printf("TBOX: system(\"%s\"\n", buf);
#else
	system(buf);
#endif
	slog(TR69C_LOG_DEBUG, "runScript %s", name);
}

/*--------------------*
 * returns
 *   <pid> for str
 *   -1    if process not found
 */
int findProc(const char* str)
{
	DIR* d;
	struct dirent *de;
	char path[64];
	char cmd[256];
	int n;

	if ((d = opendir("/proc")) != NULL) {
		while ((de = readdir(d))) {
			if (! isdigit(*de->d_name)) {
				continue;
			}
			sprintf(path, "/proc/%s/exe", de->d_name);
			n = readlink(path, cmd, 256);
			cmd[n] = '\0';
/*if (g_debug)
	  slog(TR69C_LOG_DEBUG, "findProc() pid=%s cmd=\"%s\" str=\"%s\" match=%d", 
		  de->d_name, cmd, str, strcmp(cmd,str) == 0);
#endif */
			if (strcmp(cmd, str) == 0) {
				int pid = atoi(de->d_name);
				closedir(d);
            if (g_debug)
				slog(TR69C_LOG_DEBUG, "findProc(%s) pid=%d", str, pid);
				return pid;
			}
		}
		closedir(d);
	}
if (g_debug)
	slog(TR69C_LOG_DEBUG, "findProc(%s) not found", str);
	return -1;
}

/* caseless strcmp */
int stricmp( const char *s1, const char *s2 )
{
	for (;(*s1 && *s2) && (tolower(*s1))==(tolower(*s2)); ++s1, ++s2);
	return(tolower(*s1))-(tolower(*s2));
}
/* ÏàµÈ·µ»Ø1 by zhanghaozhong*/
int streq(const char *s0, const char *s1)
{
	if (s0 == NULL && s1 == NULL)
		return 1;
	else if (s0 == NULL || s1 == NULL)
		return 0;
	else
		return strcmp(s0, s1) == 0;
}

const char *itoa(int i)
{
	static char buf[256];
	sprintf(buf, "%d", i);
	return buf;
}


int testBoolean(const char *s)
{
	if (strcasecmp(s,"true")!=0)
		/* != true */
		if (strcasecmp(s,"false")==0)
			return 0;
		return strcmp(s,"0");
	return 1;
}

/*
* digest authentication routines
*/


void  resetSessionAuth( SessionAuth *s)
{
	free(s->nc);
	s->nc=NULL;
	free(s->nonce);
	s->nonce=NULL;
	free(s->orignonce);
	s->orignonce=NULL;
	free(s->realm);
	s->realm=NULL;
	free(s->domain);
	s->domain=NULL;
	free(s->method);
	s->method=NULL;
	free(s->cnonce);
	s->cnonce=NULL;
	free(s->opaque);
	s->opaque=NULL;
	free(s->qop);
	s->qop=NULL;
	free(s->user);
	s->qop=NULL;
	free(s->uri);
	s->uri=NULL;
	free(s->algorithm);
	s->algorithm=NULL;
	free(s->response);
	s->response=NULL;
	free(s->basic);
	s->basic=NULL;
	memset(s,0,sizeof(struct SessionAuth));
	s->nonceCnt = 0;
}
/*
*  Scan for "argname=arval" 
 * and return strdup pointer to argval;
 * Return NULL is not found or form error;
*/

char *getArg(char *p, char *argname, char **argval)
{
	int		nameLth = strlen(argname);
	int		lth;
	char    *s = p;

	*argval = NULL;
	do {
		if ((s=strcasestr(s, argname)) ) {
			if ( isalpha(*(s-1))) {
				s += nameLth;
				continue;
			}
			s += nameLth;
			while (*s && isblank(*s)) ++s;
			if ( *s == '='){
				++s;
				while (*s && isblank(*s)) ++s;
				if (*s!='\"') {
					/* no quotes around value assume blank delimited or trailing , */
					char *e;
					if ( (e=strchr(s, ',')) || (e=strchr(s,' ')))
						lth = e-s;
					else /* assume hit \0 at end */
						lth = strlen(s);
				} else { /* s at opening quote of string enclosed in quotes */
					char *e;
					++s;
					if ((e=strchr(s,'\"')))
						lth = e-s;
					else
						lth = 0;		/* no closing quote-- ignore */
				}
				if (lth)
					*argval = (char *)strndup(s,lth);
				return *argval;
			}
		}  else	 /* no char sequence found -- return */
            return NULL;

	} while (*s);
	return NULL;
}

void md5ToAscii( unsigned char *s /*16bytes */, unsigned char *d /*33bytes*/)
{
	int i;

	for (i = 0; i < 16; i++)
		snprintf((char *)&d[i*2],3,"%02x", s[i]);
}
/*
* return dynamic memory buffer containing a nonce
*/
static char *makeNonce(void)
{
	struct timeval	tv;
	char	buf[100];
	char	*np;

	gettimeofday( &tv, NULL);
	/* start with something odd but hardly random */
	srand(tv.tv_usec*17);
	snprintf(buf, sizeof(buf), "%8x:%8x:%8x", 
		(unsigned)tv.tv_usec*rand(), (unsigned)tv.tv_usec*rand(), (unsigned)tv.tv_usec*1551*rand());
	b64_encode(buf, 0, &np);
	return np;
}

#define DEFAULT_OPAQUE "5ccc09c403ebaf9f0171e9517f40e41"
char *generateWWWAuthenticateHdr(SessionAuth *sa, char *realm, char *domain, char *method )
{
	char buf[256];

	resetSessionAuth(sa);
	
	sa->nonce = makeNonce(); 
	sa->orignonce = strdup(sa->nonce); /* make copy for later test */
	sa->realm = strdup(realm);
	sa->domain = strdup(domain);
	sa->method = strdup(method);
	sa->qopType = eAuth;
	sa->opaque = strdup(DEFAULT_OPAQUE);
	snprintf(buf, sizeof(buf), 
			 "WWW-Authenticate: Digest realm=\"%s\", domain=\"%s\", nonce=\"%s\", qop=\"auth\","
			 " algorithm=MD5, opaque=\"%s\" ",
			 sa->realm, sa->domain, sa->nonce, sa->opaque);
	return strdup(buf);
}
/*
* Returns value of calculated digest in sa->requestDigest
 * *
*/
static void generateRequestDigest( SessionAuth *sa, char *user, char* pwd)
{
	char md5inbuf[512];
	unsigned char md5buf[16];
	unsigned char HA1[33];
	unsigned char HA2[33];

	snprintf(md5inbuf, sizeof(md5inbuf), "%s:%s:%s", user, sa->realm, pwd);
	tr69_md5it(md5buf, (unsigned char*)md5inbuf);
	md5ToAscii(md5buf,HA1);
	/*if ( sa->algorithm && strcmp(sa->algorithm, "MD5-sess"))
		snprintf(tmpbuf, sizeof(tmpbuf), "%s:%s:%s", HA1, sa->nonce, cnonceBuf); */
	/* don't know how to do auth-int */
	snprintf(md5inbuf, sizeof(md5inbuf),"%s:%s", sa->method, sa->uri);
	tr69_md5it(md5buf,md5inbuf);
	md5ToAscii(md5buf,HA2);

	if (sa->qopType == eNoQop )
		snprintf(md5inbuf, sizeof(md5inbuf), "%s:%s:%s", HA1, sa->nonce, HA2);
    else
		snprintf(md5inbuf, sizeof(md5inbuf), "%s:%s:%08x:%s:%s:%s", HA1, sa->nonce,
				 sa->nonceCnt, sa->cnonce, sa->qop, HA2);
	tr69_md5it(md5buf, (unsigned char*)md5inbuf);
	md5ToAscii(md5buf, sa->requestDigest);
}

/*
* parse Authorization: header and test response against  requestDigest
 * returns : 1 passed authorization
 *         : 0 failed authorization
*/
int parseAuthorizationHdr(char *ahdr, SessionAuth *sa, char *username, char *password)
{
	char    *p;

	if ( ahdr && (p=strcasestr(ahdr, "digest"))) {
		free(sa->realm);
		sa->realm=NULL;
		getArg(p,"realm", &sa->realm);
		free(sa->user);
		sa->user=NULL;
		getArg(p,"username", &sa->user);
		free(sa->opaque);
		sa->opaque=NULL;
		getArg(p,"opaque", &sa->opaque);
		free(sa->nonce);
		sa->nonce=NULL;
		getArg(p,"nonce", &sa->nonce);
		free(sa->uri);
		sa->uri=NULL;
		getArg(p,"uri", &sa->uri);
		free(sa->response);
		sa->response=NULL;
		getArg(p,"response", &sa->response);
		free(sa->cnonce);
		sa->cnonce=NULL;
		getArg(p,"cnonce", &sa->cnonce);
		free(sa->qop);
		sa->qop=NULL;
		getArg(p,"qop", &sa->qop);
		free(sa->nc);
		sa->nc=NULL;
		getArg(p,"nc", &sa->nc);
		if (sa->nc)
			sa->nonceCnt = atoi(sa->nc);
		
		if ( streq(sa->user, username) && streq(sa->nonce, sa->orignonce)) {
			generateRequestDigest(sa, sa->user, password);
			if (!memcmp(sa->response, sa->requestDigest, 32))
				return 1;
		}
	}
	/* else "basic" is not allowed on connectionRequest" */
	return 0;
}

/*
* parse the WWW-Authenticate header and generate response digest in SessionAuth
*/
eAuthentication parseWWWAuthenticate(char *ahdr, SessionAuth *sa)
{
	char    *p;
	eAuthentication auth = eNone;

	resetSessionAuth(sa);
	if ( ahdr ) {
		if ((p=strcasestr(ahdr, "digest"))) {
			getArg(p,"realm", &sa->realm);
			getArg(p,"nonce", &sa->nonce);
			getArg(p,"domain", &sa->domain);
			getArg(p,"opaque", &sa->opaque);
			getArg(p,"cnonce", &sa->cnonce);
			getArg(p,"algorithm", &sa->algorithm);
			getArg(p,"qop", &sa->qop);
			auth = eDigest;
			if(sa->qop)	{
				// sa->qop can be either "auth", "auth-int", "auth,auth-int"                        
				if (strcasestr(sa->qop, "auth"))
					sa->qopType = eAuth;
				else if (strcmp(sa->qop, "auth-int"))
					sa->qopType = eAuthInt;
				else
					sa->qopType = eNoQop;
			} else
				sa->qopType = eNoQop;
		} else if ((p=strcasestr(ahdr, "basic"))) {
			getArg(p,"realm", &sa->realm);
			auth = eBasic;
		} else
			auth = eNone;
	}
	return auth;
}
/* 
* create formated digest string for Authorization header
*/

#define	HDRVALUESZ 512

static char *formatDigestParamStr( SessionAuth *sa, char *user)
{
	char xhdrbuf[256];
	char opaquebuf[256];
	char *hdrvalue;

	if (!(hdrvalue = (char *)malloc(HDRVALUESZ)))
		return NULL;
	if (sa->cnonce)
		snprintf(xhdrbuf, sizeof(xhdrbuf), "cnonce=\"%s\", nc=%08x, ",
				 sa->cnonce, sa->nonceCnt);
	else
		xhdrbuf[0]='\0';
	if (sa->opaque)
		snprintf(opaquebuf, sizeof(opaquebuf), "opaque=\"%s\", ", sa->opaque);
	else
		opaquebuf[0]='\0';
	snprintf(hdrvalue, HDRVALUESZ,
	  "Digest username=\"%s\", realm=\"%s\", algorithm=\"MD5\",%s"
			 " uri=\"%s\", nonce=\"%s\", %s%sresponse=\"%s\"",
			 user, sa->realm, sa->qop? " qop=\"auth\",":"",
			 sa->uri, sa->nonce, xhdrbuf, opaquebuf, sa->requestDigest);
	if (g_debug)
	fprintf(stderr, "Authorization header value =%s\n", hdrvalue);
	return hdrvalue;
}
/* 
* generate the Authorization header value for Digest
*       SessionAuth *sa: Session authorization struct to use.
* 		wwwAuth: pointer to WWWAuthenticate header value
*		method: "GET" or "POST" const string.
*		uri:	pointer to with uri.
*		user:  username for authentication
*       pwd:   password for authentication
* Return is pointer to malloced value: oK,
	        NULL: Parse error on WWWAuthenticate header or malloc failure.
*/

#define CNONCELTH	7
char *generateAuthorizationHdrValue( SessionAuth *sa, char *wwwAuth, char *method,
									  char *uri, char *user, char *pwd)
{
	eAuthentication auth;
	char	*hdrvalue = NULL;

	if ( (auth = parseWWWAuthenticate(wwwAuth, sa))== eDigest){
		sa->method = strdup(method);
		sa->uri = strdup(uri);
		free(sa->cnonce);
		sa->cnonce=NULL;
		generateCnonce(&sa->cnonce);
		generateRequestDigest(sa, user, pwd);
		hdrvalue = formatDigestParamStr(sa, user);
	} else if (auth == eBasic) {
		if ((hdrvalue = (char *)malloc(HDRVALUESZ))){
			generateBasicAuth(sa, user, pwd);
			snprintf(hdrvalue, HDRVALUESZ, "Basic %s", sa->basic); 
		}
	}
	return hdrvalue;
}

/*
* regenerate the Authorization header digest with the next nonce-count (nc)
*/
char *generateNextAuthorizationHdrValue( SessionAuth *sa, char *user, char *pwd)
{
	char	*hdrvalue;

	++sa->nonceCnt;
	//free(sa->cnonce);
	//generateCnonce(&sa->cnonce);
	generateRequestDigest(sa, user, pwd);
	hdrvalue = formatDigestParamStr(sa, user);
	return hdrvalue;
}

/* ---- Base64 Encoding --- */
static const char table64[]=
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
 * Curl_base64_encode()
 *
 * Returns the length of the newly created base64 string. The third argument
 * is a pointer to an allocated area holding the base64 data. If something
 * went wrong, -1 is returned.
 *
 */
size_t b64_encode(const char *inp, size_t insize, char **outptr)
{
	unsigned char ibuf[3];
	unsigned char obuf[4];
	int i;
	int inputparts;
	char *output;
	char *base64data;

	char *indata = (char *)inp;

	*outptr = NULL;	/* set to NULL in case of failure before we reach the end */

	if (0 == insize)
		insize = strlen(indata);

	base64data = output = (char*)malloc(insize*4/3+4);
	if (NULL == output)
		return 0;

	while (insize > 0) {
		for (i = inputparts = 0; i < 3; i++) {
			if (insize > 0) {
				inputparts++;
				ibuf[i] = *indata;
				indata++;
				insize--;
			} else
				ibuf[i]	= 0;
		}

		obuf [0] = (ibuf [0] & 0xFC) >> 2;
		obuf [1] = ((ibuf [0] & 0x03) << 4) | ((ibuf [1] & 0xF0) >> 4);
		obuf [2] = ((ibuf [1] & 0x0F) << 2) | ((ibuf [2] & 0xC0) >> 6);
		obuf [3] = ibuf [2] & 0x3F;

		switch (inputparts) {
		case 1:	/* only one byte read */
			snprintf(output, 5, "%c%c==",
					 table64[obuf[0]],
					 table64[obuf[1]]);
			break;
		case 2:	/* two bytes read */
			snprintf(output, 5, "%c%c%c=",
					 table64[obuf[0]],
					 table64[obuf[1]],
					 table64[obuf[2]]);
			break;
		default:
			snprintf(output, 5, "%c%c%c%c",
					 table64[obuf[0]],
					 table64[obuf[1]],
					 table64[obuf[2]],
					 table64[obuf[3]] );
			break;
		}
		output += 4;
	}
	*output=0;
	*outptr = base64data; /* make it return the actual data memory */

	return strlen(base64data); /* return the length of the new data */
}
/* ---- End of Base64 Encoding ---- */
static void generateCnonce(char **cnonceBuf)
{
	char 	buf[12];
	time_t	now;
	now= time(NULL);
	snprintf(buf, 12, "%011ld", now);
	b64_encode(buf+(12-CNONCELTH), CNONCELTH, cnonceBuf);
}

static void generateBasicAuth(SessionAuth *sa, char *user, char *pwd)
{
	char    raw[256];
	size_t  dataLen;
	char    *b64Buf;
	size_t  b64Len;

	strcpy(raw, user);
	strcat(raw, ":");
	strcat(raw, pwd);
	dataLen=strlen(raw);
	b64Len = b64_encode(raw, dataLen, &b64Buf);
	sa->basic = b64Buf;
}

