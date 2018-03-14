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

#define LIST_NUM_MAX        64
#define STRING_LEN_EX       256
#define WIFIDOG_STATUS      "/tmp/wifidog.status"
typedef enum{
    HTTP_REQ_TYPE_SYNC=0,
    HTTP_REQ_TYPE_ASYNC=1,
}REQ_TYPE_E;

#define JSON_GET_OBJ_STRING(obj,name,str,defaul){              \
    cJSON *pstObject = NULL;                            \
    pstObject=cJSON_GetObjectItem((obj),(name));       \
    if(NULL == pstObject)                               \
    {                                                   \
        strncpy((str),(defaul),sizeof(str));           \
    }                                                   \
    else                                               \
    {                                                         \
        strncpy((str),pstObject->valuestring,sizeof(str));\
    }                                                      \
}
#define JSON_GET_OBJ_INT(obj,name,integer,defaul){          \
    cJSON *pstObject = NULL;                            \
    pstObject=cJSON_GetObjectItem((obj),(name));       \
    if(NULL == pstObject)                           \
    {                                               \
        integer=defaul;                                \
    }                                               \
    else                                            \
    {                                               \
        integer=pstObject->valueint;                    \
    }                                               \
}


#define NVRAM_NAME_MAX_LEN 128
#define NVRAM_VALUE_MAX_LEN 256

typedef  int (*pFunc)(cJSON*, void*); 

extern int get_mac(char* file_mac,unsigned char *pMac);

extern int get_ip(char* file_ip,unsigned char *pIpAddr);


#define GATEWAY "gateway.@gateway[0]."

#define LAN_MAX 4

extern char *safe_strdup(const char *s);
extern void *safe_malloc(size_t size);
extern pid_t safe_fork(void);
extern char *get_iface_ip(const char *ifname);
extern char *get_iface_mac(const char *ifname);
extern struct in_addr *wd_gethostbyname(const char *name);
extern int get_end_ip(const char *ip);
extern int update_max_fd(int maxfd, int fd);
extern int safe_asprintf(char **strp, const char *fmt, ...);
extern int safe_vasprintf(char **strp, const char *fmt, va_list ap);
extern int is_online(void);
extern int is_auth_online(void);
extern int set_is_online(int state);
extern int mac_converter(const char *src, unsigned char *dst);
extern long get_uptime(void);
extern int parp_get(char *req_ip, char *dst_mac);


int strmac_to_mac(char *str_mac, unsigned char *mac);


int http_get_url_info(char* requestpath, char* host,unsigned short port, char *header, char *post_data,
                                REQ_TYPE_E reqType, pFunc pfunc,void* out);
int ap_sta_auth_write(int idletimeout, int forcetimeout);

int get_local_time(char *pacTime);

int tools_str_to_num_list(char *list_str,unsigned char *list_num,unsigned int *iTotal,int iMax);
int tools_nvram_list_add(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,unsigned int *ulNodeNo, unsigned int ulMaxNum, char *pucValue);
int tools_nvram_list_del(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,char *pucValue);
int tools_nvram_list_get(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,char acListValue[LIST_NUM_MAX][NVRAM_VALUE_MAX_LEN]);
int tools_nvram_list_get(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,char acListValue[LIST_NUM_MAX][NVRAM_VALUE_MAX_LEN]);
int tools_nvram_listnode_get(char *pcListName, char *pcNumName, char *pcListHead, unsigned int *ulCount,char *pucValue);
#endif /* __UTIL_H__ */
