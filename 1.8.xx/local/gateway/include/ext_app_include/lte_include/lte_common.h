#ifndef __LTE_COMMON_H__
#define __LTE_COMMON_H__

/*dhcp client，重新拨号时需要去掉*/
#define DHCLIENT "dhclient"
#define DHCPC "udhcpc"

typedef LTE_RET_E (*LTE_IOCTL_HANDLER_F)(void *pvData, int iDataLen);

typedef struct 
{
    int  iCmd;
    LTE_IOCTL_HANDLER_F  func;
    unsigned int  uiCount;     /*总调用次数*/
    unsigned int  uiCountErr; /*调用出错次数*/
    char *pcDesc;       /*接口描述*/
}LTE_IOCTL_FUNC_T;

typedef struct 
{  
    char acCmd[32];    /* 输入com口的命令 */
    char acFileName[32];   /* 需要比较的文件 */
    char acNeedle[32]; /* 比较字段 */
    int iResult;    /* 输出比较的结果 */
    /*altered by caibing 2013-02-27*/
    char acCom[32];    /* 操作的com口 */
    int iComFd;       /*com fd 号*/
    int iSecond;      /*读写com时长*/
}LTE_NORMAL_AT_T;

//#define DEFAULT_SEARCH_NET_TIMES 50
/*altered by caibing */
#define DEFAULT_SEARCH_NET_TIMES 25

extern LTE_RET_E system_call(char *pcCmd);
extern LTE_RET_E lte_has_valid_process(char *process,char *pNeedle);
extern LTE_RET_E lte_has_valid_ip(void);



#endif /* __LTE_COMMON_H__ */

