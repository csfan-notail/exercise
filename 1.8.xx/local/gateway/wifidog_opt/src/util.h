/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : util.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : util.c header file
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __UTIL_H__
#define __UTIL_H__

#ifndef MAC_FMT
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

#ifndef MAC_ARG
#define MAC_ARG(x) ((unsigned char*)(x))[0], ((unsigned char*)(x))[1], ((unsigned char*)(x))[2], ((unsigned char*)(x))[3], ((unsigned char*)(x))[4], ((unsigned char*)(x))[5]
#endif

#ifndef IP_FMT
#define IP_FMT	"%u.%u.%u.%u"
#endif

#ifndef IP_ARG
#define IP_ARG(x) ((unsigned char*)(x))[0], ((unsigned char*)(x))[1], ((unsigned char*)(x))[2], ((unsigned char*)(x))[3]
#endif


#define WIFIDOG_STATUS      "/tmp/wifidog.status"

extern inline int mac_hashfn(unsigned char *mac, int hash_size);

extern char *safe_strdup(const char *s);
extern void *safe_malloc(size_t size);
extern pid_t safe_fork(void);
extern int get_iface_ip(const char *ifname, char *ifip, int len);
extern int get_iface_mac(const char *ifname, char *ifmac, int len);
extern struct in_addr *wd_gethostbyname(const char *name);
extern int client_mac_hash(const char *pucMac);
extern int get_end_ip(const char *ip);
extern int update_max_fd(int maxfd, int fd);
extern int safe_asprintf(char **strp, const char *fmt, ...);
extern int safe_vasprintf(char **strp, const char *fmt, va_list ap);
extern int is_online(void);
extern int check_online_interval(unsigned int uiSecond);

extern int is_auth_online(void);
extern int set_is_online(int state);
extern int mac_converter(const char *src, unsigned char *dst);
extern long get_uptime(void);
extern int parp_get(char *req_ip, char *dst_mac);
extern char *util_cmd_return_str(char *cmd);

#endif /* __UTIL_H__ */
