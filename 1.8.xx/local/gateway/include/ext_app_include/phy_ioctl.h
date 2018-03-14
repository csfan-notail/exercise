#ifndef PHY_IOCTL_H
#define PHY_IOCTL_H

#define SIOCDEVPRIVATE  0x89F0  /* to 89FF */
#define GBCOM_CRTL_IOC          (SIOCDEVPRIVATE | 0x0F)

#if 0
#define PHY_CMD_GET_SPEED			(SIOCDEVPRIVATE | 0x0C)
//#define PHY_CMD_GET_LINK				(SIOCDEVPRIVATE | 0x2)
#define PHY_CMD_GET_FDX				(SIOCDEVPRIVATE | 0x0D)
//#define PHY_CMD_SET_SPEED			(SIOCDEVPRIVATE | 0x4)
//#define PHY_CMD_LED_CONTROL		(SIOCDEVPRIVATE | 0x5)
#define PHY_CMD_DEBUG					(SIOCDEVPRIVATE | 0x0B)

#define ATHR_MDIO_125 					(SIOCDEVPRIVATE | 0x06)
#define ATHR_MDIO_25 						(SIOCDEVPRIVATE | 0x07)
#define ATHR_DELAY								(SIOCDEVPRIVATE | 0x08)
#define PHY_CMD_SET_SPEED			(SIOCDEVPRIVATE | 0x09)
#define ATHR_STATUS							(SIOCDEVPRIVATE | 0x0A)
#endif

#define PHY_CMD_IOCTL_DEBUG     ((GBCOM_CRTL_IOC << 16) | 0x1)
#define PHY_CMD_GET_PORT_STATUS ((GBCOM_CRTL_IOC << 16) | 0x2)
#define PHY_CMD_SET_PORT_MODE   ((GBCOM_CRTL_IOC << 16) | 0x3)

/*定义速率*/
#define SPEED_10M  			0
#define SPEED_100M 		1
#define SPEED_1000M				2
#define SPEED_AUTO					3
#define SPEED_100M_AUTO 	4

#define HALF_DUPLEX 0
#define FULL_DUPLEX 1

#define AR9344_GBCOM ( board_ver==1 ||board_ver == 5) //自研板

#define PHY_ADDR_GBCOM 0x00
#define PHY_ADDR_OTHERS 0x04

#define PHY_REG_CONTROL 						0x00
#ifndef ADM_AUTONEG_ADVERT
#define ADM_AUTONEG_ADVERT		0x04
#endif
#ifndef ADM_1000_BASE_T_CONTROL
#define ADM_1000_BASE_T_CONTROL	 0x09
#endif
#define ADM_PHY_SPECIFIC_STATUS  	0x11
extern int phyDebugEnable;
#define phy_printk(fmt, args...)   \
  do{                             \
    if (phyDebugEnable)          \
    {                             \
      printk("%s(%d): ",__FUNCTION__, __LINE__); \
      printk(fmt, ##args);        \
    }                             \
  }while(0)
  
#endif


/* BEGIN: Added by xucongjiang, 2012/8/10 */
#define CP_TO_USER_ERR    1000
#define CP_FROM_USER_ERR  1001
#define CALL_FUNCTION_ERR 1002
#define CMD_NOT_FOUDN_ERR 1003

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

typedef struct PHY_PORT_STATUS
{
    char acPortName[IFNAMSIZ];
    unsigned int uiLinkStatus;
    unsigned short usSpeed;
    unsigned short usDuplex;
}PHY_PORT_STATUS_T;

typedef struct PHY_PORT_MODE
{
    char acPortName[IFNAMSIZ];
    unsigned short usSpeed;
    unsigned short usDuplex;
}PHY_PORT_MODE_T;

typedef struct PHY_DEBUG
{
    int iSw;      
}PHY_DEBUG_T;

typedef struct PHY_IOCTL_ARGS
{
    int iCmd;

    union
    {
        PHY_PORT_STATUS_T stPortStatus;
        PHY_PORT_MODE_T stPortMode;
        PHY_DEBUG_T stDebugSw;
    }u;
}PHY_IOCTL_ARGS_T;
/* END:   Added by xucongjiang, 2012/8/10   PN: */
    
