#ifndef __LTE_COMMON_H__
#define __LTE_COMMON_H__

/*dhcp client�����²���ʱ��Ҫȥ��*/
#define DHCLIENT "dhclient"
#define DHCPC "udhcpc"

typedef LTE_RET_E (*LTE_IOCTL_HANDLER_F)(void *pvData, int iDataLen);

typedef struct 
{
    int  iCmd;
    LTE_IOCTL_HANDLER_F  func;
    unsigned int  uiCount;     /*�ܵ��ô���*/
    unsigned int  uiCountErr; /*���ó������*/
    char *pcDesc;       /*�ӿ�����*/
}LTE_IOCTL_FUNC_T;

typedef struct 
{  
    char acCmd[32];    /* ����com�ڵ����� */
    char acFileName[32];   /* ��Ҫ�Ƚϵ��ļ� */
    char acNeedle[32]; /* �Ƚ��ֶ� */
    int iResult;    /* ����ȽϵĽ�� */
    /*altered by caibing 2013-02-27*/
    char acCom[32];    /* ������com�� */
    int iComFd;       /*com fd ��*/
    int iSecond;      /*��дcomʱ��*/
}LTE_NORMAL_AT_T;

//#define DEFAULT_SEARCH_NET_TIMES 50
/*altered by caibing */
#define DEFAULT_SEARCH_NET_TIMES 25

extern LTE_RET_E system_call(char *pcCmd);
extern LTE_RET_E lte_has_valid_process(char *process,char *pNeedle);
extern LTE_RET_E lte_has_valid_ip(void);



#endif /* __LTE_COMMON_H__ */

