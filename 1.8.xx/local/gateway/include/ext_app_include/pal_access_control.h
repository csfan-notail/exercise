
#ifndef __PAL_ACCESS_CONTROL_H__
#define __PAL_ACCESS_CONTROL_H__

typedef struct
{
	unsigned char acMac[6];
}FREEAUTH_MAC_E;            

typedef struct
{
    unsigned int uiIterm;                 /* Iterm count */
    FREEAUTH_MAC_E *pstFreeAuthMac;
}FREEAUTH_MAC_ITEM_T;

/*----------------------------------------------*
 * 导出函数原型说明                             *
 *----------------------------------------------*/
extern int freeauth_mac_add(char *pcMacAddr);
extern int freeauth_mac_del(char *pcMacAddr);
extern int set_lan_isolation(int enable);

#endif

