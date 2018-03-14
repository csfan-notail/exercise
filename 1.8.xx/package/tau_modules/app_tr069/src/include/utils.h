/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 

#ifndef UTILS_H_MAIN
#define UTILS_H_MAIN

#include <stdarg.h>
#include "types.h"

#define TR69C_LOG_EMERG       0       /* system is unusable */
#define TR69C_LOG_ALERT       1       /* action must be taken immediately */
#define TR69C_LOG_CRIT        2       /* critical conditions */
#define TR69C_LOG_ERR         3       /* error conditions */
#define TR69C_LOG_WARNING     4       /* warning conditions */
#define TR69C_LOG_NOTICE      5       /* normal but significant condition */
#define TR69C_LOG_INFO        6       /* informational */
#define TR69C_LOG_DEBUG       7      /* debug-level messages */
#if 1
#define slog(level, format, arg...) \
do{                                    \
    if (level <= g_log_level)       \
    {                                  \
       syslog(level,"[tr69c][%s][%d]"format, __func__,__LINE__,##arg);   \
    }                                  \
}while(0);
#else
void slog(int level, const char* fmt, ...);
#endif



void initLog(int flag); 


void vlog(int level, const char* fmt, va_list ap);
int dns_lookup(const char *name, tIpAddr *res);

extern const u_char zeroMac[6];

/* Files */
int  hasChanged(const char* new, const char* old);
int  mkdirs(const char *path);
void rmrf(const char* path);

/* Time */
time_t getCurrentTime(void);
int  cmpTime(struct timeval* t1, struct timeval* t2);
void addMs(struct timeval* t1, struct timeval* t2, int ms);
void subTime(struct timeval* t1, struct timeval* t2);
char *getXSIdateTime(time_t *tp);

/* hex */
extern const char *util_StringToHex(const char *s);

/* Addresses */
void readMac(u_char* mac, const char* val);
int  readIp(const char* ip);
int  readProto(const char* val);
int readMask(const char *mask);

char* writeMac(const u_char* mac);
char* writeCanonicalMac(const u_char* mac);
char* writeQMac(const u_char* mac);
void  writeIp_b(int ip, char *buf);
char* writeIp(int ip);
char* writeNet(int ip, int bits);
char* writeBcast(int ip, int bits);
char* writeMask(int bits);
char* writeRevNet(int ip, int bits);
char* writeRevHost(int ip, int bits);
char* writeProto(int proto);

/* Text handling and formatting */
void  readHash(u_char* hash, const char* val);
char* writeQHash(const u_char* mac);
char* unquoteText(const char* text);
char* quoteText(const char* text);
int streq(const char *s0, const char *s1);
int stricmp( const char *s1, const char *s2 );
const char *itoa(int i);
int testBoolean(const char *s);

typedef struct WanState {
  u_char mac[6];
  char *name;
  tIpAddr ip;
  tIpAddr mask;
  tIpAddr gw;
} WanState;
extern WanState wanState;

extern WanState wanState;
int getWanInfo(WanState *wanState);

typedef enum {
    eNone,
    eDigest,
    eBasic
} eAuthentication;
typedef enum {
	eNoQop,
	eAuth,
	eAuthInt
} eQop;

/* Used for both server/client */
typedef struct SessionAuth  {
	eQop	qopType;
	int		nonceCnt;
	char 	*nc;		/* str of nonceCnt */
    char    *nonce;
    char    *orignonce;
    char    *realm;
    char    *domain;
    char    *method;
    char    *cnonce;
	char 	*opaque;
	char	*qop;
    char    *user;
    char    *uri;
	char	*algorithm;
    char    *response;
    char    *basic;
    unsigned    char    requestDigest[33];
} SessionAuth;

char *generateWWWAuthenticateHdr(SessionAuth *sa, char *realm, char *domain, char* method);

int parseAuthorizationHdr(char *ahdr, SessionAuth *sa, char *username, char *password);

char *generateAuthorizationHdrValue( SessionAuth *sa, char *wwwAuth, char *method,
									  char *uri, char *user, char *pwd);
char *generateNextAuthorizationHdrValue(SessionAuth *, char *user, char *pwd );
eAuthentication parseWWWAuthenticate(char *ahdr, SessionAuth *sa);
#endif

