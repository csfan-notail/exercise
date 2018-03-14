


#ifndef __PAL_COMMON_H__
#define __PAL_COMMON_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/if.h>
#include <linux/sockios.h>


#include <sys/sysinfo.h>
#include <errno.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>




#include "ap_types.h"
#include "log.h"







#define MAC_LEN				6
#define IP_LEN				16
#define SSID_LEN            32


#ifndef MAC_FMT
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#endif
#ifndef MAC_ARG
#define MAC_ARG(x) ((u8*)(x))[0], ((u8*)(x))[1], ((u8*)(x))[2], \
	((u8*)(x))[3], ((u8*)(x))[4], ((u8*)(x))[5]
#endif

#ifndef IP_FMT
#define IP_FMT	"%u.%u.%u.%u"
#endif
#ifndef IP_ARG
#define IP_ARG(x) ((u8*)(x))[0], ((u8*)(x))[1], ((u8*)(x))[2], ((u8*)(x))[3]
#endif


#define STR_LEN_32 32






/*公用配置宏定义*/







/*申请内存并初始化为0*/
#define MEM_CALLOC(__size__)  calloc(1, __size__)

/*释放内存并赋值为NULL*/
#define MEM_FREE(__addr__) \
	do{\
		if(__addr__ != NULL){ \
			free(__addr__); \
			__addr__ = NULL; \
		}\
	}while(0)





#define STRCMP(x,y)  strncmp(x,y,strnlen(y))


#endif
