/*
 * ag_common.h
 *
 ****************************************************************************
 */

#ifndef _AG_COMMON_H_
#define _AG_COMMON_H_

#ifndef __KERNEL__

typedef char s8;
typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;

typedef int s32;
typedef unsigned int u32;

#if __WORDSIZE == 64
typedef long s64;
typedef unsigned long u64;
#else
typedef long long s64;
typedef unsigned long long u64;
#endif


#endif

#ifndef BOOL
#define BOOL UINT32
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef TRUE
#define TRUE 		1
#endif
#ifndef FALSE
#define FALSE		0
#endif

#if 0
typedef enum
{
	FALSE,
	TRUE,
} BOOL;
#endif

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
#ifndef GW_X86
	#if defined(AP9344_OAM)
	#define PHY_IF "eth0"
	#elif defined(AP9341_OAM) 
	#define PHY_IF "wan0"
	#elif defined(AP9341_OAM_21) 
	#define PHY_IF "eth0"
	#elif defined(OPENWRT) 
	#define PHY_IF "eth0"
	#else
	#define PHY_IF "ixp0"
	#endif
#else
	#define PHY_IF "eth0"
#endif
#endif

