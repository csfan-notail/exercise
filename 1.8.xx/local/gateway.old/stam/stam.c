/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : event.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/7/18
  Last Modified :
  Description   : 捕获驱动上报时间
  Function List :
  History       :
  1.Date        : 2013/7/18
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/wireless.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <signal.h>

#include <getopt.h>
#include <time.h>
#include <sys/time.h>


#include "tool.h"

#include "ag_module.h"
#include "list.h"
#include "ag_msg.h"
#ifdef OPENWRT
#include "pal_wifi.h"
#else
#include "gbcom_wifi_pal.h"
#endif
#define EVENT_OPT       "hvp:"

#define STAM_STA_IP_FILE	"/tmp/stam_sta_ip"


typedef struct 
{
	struct event stStaSyn_ev;		/*同步终端信息*/
	struct event stNetlinkUser_ev;	/*用户态netlink消息接收事件*/
	s32 Netlinkfd;
	struct timeval stStaSyn_tv;	/*同步终端时间间隔*/
}STAM_EVENT_INFO;

#define NETLINK_MSG_LENGTH_MAX	64
#define NETLINK_MSG_ID_STA_IP_UPDATE		0x00000001

typedef struct
{
	u32	uiEventId;
	u32	uiMsgLen;
	u8	ucBuff[NETLINK_MSG_LENGTH_MAX];
}NETLINK_EVENT;

typedef struct
{
    	u8 acMac[MAC_LEN];
	u16	usFirst;		//首次学习到IP
    	u32 uiIpAddr;
	u32 uiLearnStartTime; /* 记录用户学习起始时间 */
}USER_STA_INFO_T;

enum STAM_DEV_TYPE
{
	STAM_DEV_TYPE_GW = 0,
	STAM_DEV_TYPE_AP
};

#define STAM_STA_INFO_SYN_TM	300

STAM_EVENT_INFO gstStamEvent = {0};
AG_MODULE_IPC_REGISTER gstStamIPCmgt = {0};
struct hlist_head gstStahlist[STA_HASH_SIZE] = {0};
struct list_head g_stStaList = {&g_stStaList, &g_stStaList};

static s32 giStaNum = 0;
static s32 giDevType = 0;
	
u8 gcWanMac[MAC_LEN] = {0};

pthread_mutex_t sta_hlist_mutex;				/*终端关联信息缓存访问锁*/

#define STA_HLIST_LOCK()		pthread_mutex_lock(&sta_hlist_mutex)
#define STA_HLIST_UNLOCK()		pthread_mutex_unlock(&sta_hlist_mutex)

#define 	STAM_DEL_STA_INFO		0		
#define 	STAM_ADD_STA_INFO		1
#define 	STAM_UPDATE_STA_INFO		2

#define NETLINK_EVENT_PROTOCOL     31
#define NETLINK_EVENT_GROUPS       1

#define STAM_DEV_IS_AP() 	(STAM_DEV_TYPE_AP == giDevType)
#define STAM_DEV_IS_GW() 	(STAM_DEV_TYPE_GW == giDevType)

#if 0
typedef struct _vap_qos
{
    int traffic_type;
    char name[32];
    int ssid_max_upload;
    int ssid_max_download;
    int sta_max_upload;
    int sta_max_download;
    int pcf;
}vap_qos;
#endif
struct rtnl_handle
{
	int			fd;
	struct sockaddr_nl	local;
	struct sockaddr_nl	peer;
	__u32			seq;
	__u32			dump;
};

/*
 * Static information about wireless interface.
 * We cache this info for performance reason.
 */
typedef struct wireless_iface
{
  /* Linked list */
  struct wireless_iface *next;

  /* Interface identification */
  int ifindex;		/* Interface index == black magic */

  /* Interface data */
  char ifname[IFNAMSIZ + 1];	/* Interface name */
  struct iw_range	range;			/* Wireless static data */
  int has_range;
} wireless_iface;

/* Cache of wireless interfaces */
struct wireless_iface *	interface_cache = NULL;

static void usage(void);
static int write_pid(char *path);
static inline int index2name(int skfd, int ifindex, char *name);
static inline void rtnl_close(struct rtnl_handle *rth);
static inline int rtnl_open(struct rtnl_handle *rth, unsigned subscriptions);
static int iw_sockets_open(void);
static inline void iw_sockets_close(int	skfd);
static struct wireless_iface *iw_get_interface_data(int ifindex);
static void iw_del_interface_data(int ifindex);
static inline int print_event_stream(int ifindex, char *data, int len);
static inline int wait_for_event(struct rtnl_handle *rth);
static inline void handle_netlink_events(struct rtnl_handle *rth);
static int LinkCatcher(struct nlmsghdr *nlh);

#define VAP_MAX_TEMP 32

int sta_flow_read(vap_qos *pst_qos)
{
    int len;
    int loop;
    FILE *pf;
    vap_qos qos[VAP_MAX_TEMP];
    len = sizeof(vap_qos)*VAP_MAX_TEMP;
    memset(qos, 0, len);
    pf = fopen("/tmp/.flow_sta", "rb");
    if(NULL == pf)
    {
        memcpy(&qos[0], pst_qos, sizeof(vap_qos));
    }
    else
    {
        fread(qos, len, 1, pf);
        fclose(pf);
        for(loop = 0; loop < VAP_MAX_TEMP; loop++)
        {
            if(0 == memcmp(qos[loop].name, pst_qos->name, strlen(pst_qos->name)))
            {
                memcpy(pst_qos, &qos[loop], sizeof(vap_qos));
                printf("get qos for loop %d, the qos is switch(%d), ssid(up:%d, down:%d), sta(up:%d, down:%d)\n",
                    loop, pst_qos->traffic_type, pst_qos->ssid_max_upload, pst_qos->ssid_max_download,
                    pst_qos->sta_max_upload, pst_qos->sta_max_download);
                break;
            }
        }
        
    }
    return ERROR;
}

int ap_sta_flow_read(int *type, int *upload, int *download)
{
    FILE *pf;
    char buff[128];
    
    pf = fopen("/tmp/.flow_ap_sta", "rb");
    if(NULL == pf)
    {
        type = 0;
    }
    else
    {
        memset(buff, 0, sizeof(buff));
        fread(buff, sizeof(buff), 1, pf);
        fclose(pf);
        sscanf(buff, "%d,%d,%d", type, upload, download);
        printf("type: %d, upload: %d, download: %d\n",
            *type, *upload, *download);
    }
    return ERROR;
}

int sta_flow_config(char *name, char *mac, int flag)
{
    vap_qos st_qos;
    char command[128];
    int iRadio;
    int wlanId;

    int type;
    int upload;
    int download;

    memset(&st_qos, 0, sizeof(vap_qos));
    memcpy(st_qos.name, name, strlen(name));

    sta_flow_read(&st_qos);
    ap_sta_flow_read(&type, &upload, &download);
    if(0 != st_qos.traffic_type)
    {
        if(0 == st_qos.ssid_max_download
            || 0 ==st_qos.ssid_max_upload
            || 0 == st_qos.sta_max_download
            || 0 == st_qos.sta_max_upload)
        {
            return OK;
        }
        
    
        sscanf(name, "ap%d_%d", &iRadio, &wlanId);
        if(1 == flag)
        {
            sprintf(command, "flowctrl addsta %d %d "MAC_FMT" %d %d %d %d",
                iRadio, wlanId, MAC_AGR(mac),
                st_qos.sta_max_upload,
                st_qos.sta_max_upload < 5000 ? 5000 : st_qos.sta_max_upload,
                st_qos.sta_max_download,
                st_qos.sta_max_download < 5000 ? 5000 : st_qos.sta_max_download);
        }
        else
        {
             /* 2014/4/10 added by zhangping */
             sprintf(command, "flowctrl delsta "MAC_FMT,
                MAC_AGR(mac));
        }
        system(command);
        printf("shell exec %s\n", command);
    }
    else if(1 == type)
    {
        if(0 == upload || 0 == download)
        {
            return OK;
        }

        if(1 == flag)
        {
            sprintf(command, "flowctrl addsta 0 0 "MAC_FMT" %d %d %d %d",
                MAC_AGR(mac),
                upload, upload < 5000 ? 5000 : upload,
                download, download < 5000 ? 5000 : download);
        }
        else
        {
             sprintf(command, "flowctrl delsta 0 0 "MAC_FMT, MAC_AGR(mac));
        }
        system(command);
        printf("shell exec %s\n", command);
    }
    return OK;
}


static void usage(void)
{
    printf("Usage: event [option] ...\n");
    printf("Options:\n");
    printf("    -v\t\tPrint the program version.\n");
    printf("    -h\t\tDisplay the information.\n");
    return;
}

static int write_pid(char *path)
{
    FILE *pf;
    char buff[32];

    sprintf(buff, "%s", getpid());
    pf = fopen(path, "w+");
    if(NULL == pf) {
        printf("write %s failed\n", path);
        return -1;
    }
    return 0;
}

static inline void rtnl_close(struct rtnl_handle *rth)
{
	close(rth->fd);
}

int iw_sockets_open(void)
{
    static const int families[] = {
        AF_INET, AF_IPX, AF_AX25, AF_APPLETALK
    };
    unsigned int	i;
    int		sock;

    /*
     * Now pick any (exisiting) useful socket family for generic queries
     * Note : don't open all the socket, only returns when one matches,
     * all protocols might not be valid.
     * Workaround by Jim Kaba <jkaba@sarnoff.com>
     * Note : in 99% of the case, we will just open the inet_sock.
     * The remaining 1% case are not fully correct...
     */

    /* Try all families we support */
    for(i = 0; i < sizeof(families)/sizeof(int); ++i) {
        /* Try to open the socket, if success returns it */
        sock = socket(families[i], SOCK_DGRAM, 0);
        if(sock >= 0)
            return sock;
    }

    return -1;
}

static inline void iw_sockets_close(int	skfd)
{
    close(skfd);
}

static inline int index2name(int skfd,
                             int ifindex,
                             char *name)
{
    struct ifreq	irq;
    int		ret = 0;

    memset(name, 0, IFNAMSIZ + 1);

    /* Get interface name */
    irq.ifr_ifindex = ifindex;
    if(ioctl(skfd, SIOCGIFNAME, &irq) < 0)
        ret = -1;
    else
        strncpy(name, irq.ifr_name, IFNAMSIZ);

    return(ret);
}

static inline int rtnl_open(struct rtnl_handle *rth, unsigned subscriptions)
{
	int addr_len;

	memset(rth, 0, sizeof(rth));

	rth->fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (rth->fd < 0) {
		perror("Cannot open netlink socket");
		return -1;
	}

	memset(&rth->local, 0, sizeof(rth->local));
	rth->local.nl_family = AF_NETLINK;
	rth->local.nl_groups = subscriptions;

	if (bind(rth->fd, (struct sockaddr*)&rth->local, sizeof(rth->local)) < 0) {
		perror("Cannot bind netlink socket");
		return -1;
	}
	addr_len = sizeof(rth->local);
	if (getsockname(rth->fd, (struct sockaddr*)&rth->local,
			(socklen_t *) &addr_len) < 0) {
		perror("Cannot getsockname");
		return -1;
	}
	if (addr_len != sizeof(rth->local)) {
		fprintf(stderr, "Wrong address length %d\n", addr_len);
		return -1;
	}
	if (rth->local.nl_family != AF_NETLINK) {
		fprintf(stderr, "Wrong address family %d\n", rth->local.nl_family);
		return -1;
	}
	rth->seq = time(NULL);
	return 0;
}

/*------------------------------------------------------------------*/
/*
 * Get interface data from cache or live interface
 */
static struct wireless_iface *iw_get_interface_data(int	ifindex)
{
    struct wireless_iface *	curr;
    int				skfd = -1;	/* ioctl socket */

    /* Search for it in the database */
    curr = interface_cache;
    while(curr != NULL) {
        /* Match ? */
        if(curr->ifindex == ifindex) {
            //printf("Cache : found %d-%s\n", curr->ifindex, curr->ifname);

            /* Return */
            return(curr);
        }
        /* Next entry */
        curr = curr->next;
    }

    /* Create a channel to the NET kernel. Doesn't happen too often, so
     * socket creation overhead is minimal... */
    if((skfd = iw_sockets_open()) < 0) {
        perror("iw_sockets_open");
        return(NULL);
    }

    /* Create new entry, zero, init */
    curr = calloc(1, sizeof(struct wireless_iface));
    if(!curr) {
        fprintf(stderr, "Malloc failed\n");
        return(NULL);
    }
    curr->ifindex = ifindex;

    /* Extract static data */
    if(index2name(skfd, ifindex, curr->ifname) < 0) {
        perror("index2name");
        free(curr);
        return(NULL);
    }
    //curr->has_range = (iw_get_range_info(skfd, curr->ifname, &curr->range) >= 0);
    //printf("Cache : create %d-%s\n", curr->ifindex, curr->ifname);

    /* Done */
    iw_sockets_close(skfd);

    /* Link it */
    curr->next = interface_cache;
    interface_cache = curr;

    return(curr);
}

/*------------------------------------------------------------------*/
/*
 * Remove interface data from cache (if it exist)
 */
static void iw_del_interface_data(int ifindex)
{
    struct wireless_iface *	curr;
    struct wireless_iface *	prev = NULL;
    struct wireless_iface *	next;

    /* Go through the list, find the interface, kills it */
    curr = interface_cache;
    while(curr) {
        next = curr->next;

        /* Got a match ? */
        if(curr->ifindex == ifindex) {
            /* Unlink. Root ? */
            if(!prev)
                interface_cache = next;
            else
                prev->next = next;
            //printf("Cache : purge %d-%s\n", curr->ifindex, curr->ifname);

            /* Destroy */
            free(curr);
        } else {
            /* Keep as previous */
            prev = curr;
        }

        /* Next entry */
        curr = next;
    }
}

static inline void handle_netlink_events(struct rtnl_handle *rth)
{
    while(1) {
        struct sockaddr_nl sanl;
        socklen_t sanllen = sizeof(struct sockaddr_nl);

        struct nlmsghdr *h;
        int amt;
        char buf[8192];

        amt = recvfrom(rth->fd, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr*)&sanl, &sanllen);
        if(amt < 0) {
            if(errno != EINTR && errno != EAGAIN) {
                fprintf(stderr, "%s: error reading netlink: %s.\n",
                        __PRETTY_FUNCTION__, strerror(errno));
            }
            return;
        }

        if(amt == 0) {
            fprintf(stderr, "%s: EOF on netlink??\n", __PRETTY_FUNCTION__);
            return;
        }

        h = (struct nlmsghdr*)buf;
        while(amt >= (int)sizeof(*h)) {
            int len = h->nlmsg_len;
            int l = len - sizeof(*h);

            if(l < 0 || len > amt) {
                fprintf(stderr, "%s: malformed netlink message: len=%d\n", __PRETTY_FUNCTION__, len);
                break;
            }

            switch(h->nlmsg_type) {
            case RTM_NEWLINK:
            case RTM_DELLINK:
                LinkCatcher(h);
                break;
            default:
                break;
            }

            len = NLMSG_ALIGN(len);
            amt -= len;
            h = (struct nlmsghdr*)((char*)h + len);
        }

        if(amt > 0)
            fprintf(stderr, "%s: remnant of size %d on netlink\n", __PRETTY_FUNCTION__, amt);
    }
}

static inline int wait_for_event(struct rtnl_handle *rth)
{

  /* Forever */
    while(1) {
        fd_set		rfds;		/* File descriptors for select */
        int		last_fd;	/* Last fd */
        int		ret;

        /* Guess what ? We must re-generate rfds each time */
        FD_ZERO(&rfds);
        FD_SET(rth->fd, &rfds);
        last_fd = rth->fd;

        /* Wait until something happens */
        ret = select(last_fd + 1, &rfds, NULL, NULL, NULL);

        /* Check if there was an error */
        if(ret < 0) {
            if(errno == EAGAIN || errno == EINTR)
                continue;
            fprintf(stderr, "Unhandled signal - exiting...\n");
            break;
        }

        /* Check if there was a timeout */
        if(ret == 0) {
            continue;
        }

        /* Check for interface discovery events. */
        if(FD_ISSET(rth->fd, &rfds))
            handle_netlink_events(rth);
    }

    return(0);
}

s32 stam_module_build_socket(s32 *iSocket)
{
	struct timeval timeout = {10, 0};
	*iSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == *iSocket)
	{
		AG_MODULE_LOG(LOG_ALERT, "build socket fail");
		return FALSE;
	}
	setsockopt(*iSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval));
	setsockopt(*iSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
	return TRUE;
}
static int stam_do80211priv(struct iwreq *iwr, const char *ifname, int op, void *data, u32 len)
{
	s32 iSocket;
	s32 iRet;
	iRet = stam_module_build_socket(&iSocket);
    	if (FALSE == iRet)
    	{
    		AG_MODULE_LOG(LOG_INFO, "do80211priv: invalid socket");
       	return FALSE;
    	}
	memset(iwr, 0, sizeof(struct iwreq));
#ifdef WIN32
    	strncpy(iwr->ifr_ifrn.ifrn_name, ifname, IFNAMSIZ);
#else
	strncpy(iwr->ifr_name, ifname, IFNAMSIZ);
#endif
	if (len < IFNAMSIZ)
    {
		memcpy(iwr->u.name, data, len);
	}
    else
    {
		iwr->u.data.pointer = data;
		iwr->u.data.length = len;
	}
	if (ioctl(iSocket, op, iwr) < 0)
    	{
    		AG_MODULE_LOG(LOG_INFO, "ioctl fail");
		return FALSE;
	}
	close(iSocket);
	return TRUE;
}
static int stam_get80211priv(const char *ifname, int op, void *data, size_t len)
{
	struct iwreq iwr;
	if (stam_do80211priv(&iwr, ifname, op, data, len) < 0)
	{
		AG_MODULE_LOG(LOG_INFO, "do80211priv");
		return FALSE;
	}
	if (len < IFNAMSIZ)
		memcpy(data, iwr.u.name, len);
	AG_MODULE_LOG(LOG_INFO, "data:%s  len:%d  data.length:%d", data, len, iwr.u.data.length);
	return iwr.u.data.length;
}
s32 stam_module_get_essid(u8 *dev, u8 *ucEssid)
{
	 s32 op = 0;
	 s32 iRet = 0;
	op = SIOCGIWESSID;
	iRet = stam_get80211priv(dev, op, ucEssid, 32);
	if ( !(iRet > 0 ))
    	{
	        AG_MODULE_LOG(LOG_ALERT, "handler failure(0x%x)", op);
	        return FALSE;
    	}
	return TRUE;
}
s32 stam_module_get_wan_ip_mac(u32 *uiIp, u8 *mac)
{
	u8 * pt = NULL;
	u8 config[64] = {0};
	s32 iRet = 0;
	s32 iSocket = 0;
	s32 intrface = 0;
	s32 ip = 0;
	struct ifconf ifc;
	struct ifreq req[16];
	iRet = stam_module_build_socket(&iSocket);
	if(FALSE == iRet)
	{
		AG_MODULE_LOG(LOG_ALERT, "error");
		return FALSE;
	}
	iRet = nvram_get(config, "ipv4_0_ip_address", 64);
	if(0 != iRet)
	{
		AG_MODULE_LOG(LOG_ALERT, "nvram error");
		close(iSocket);
		return FALSE;
	}
	AG_MODULE_LOG(LOG_INFO, "ipv4_0_ip_address=%s", config);
	pt = &config[0];
	while(*pt != '\0'){ /*dhcp获取的地址可能存在末尾空格，去掉*/
		if(' ' == *pt){
			*pt = '\0';
			break;
		}
		++pt;
	}
	if(inet_pton(AF_INET, config, (void *)uiIp) <= 0){
		AG_MODULE_LOG(LOG_ALERT, "inet_pton error for %s: %s(errno: %d)", 
			config,  strerror(errno), errno);
	}
	AG_MODULE_LOG(LOG_INFO, " uiIp:"IP_FMT, IP_ARG(uiIp));
	ifc.ifc_len = sizeof(req);
	ifc.ifc_buf = (caddr_t)req;
	if (!ioctl(iSocket, SIOCGIFCONF, (s8 *)&ifc))
	{
		intrface = ifc.ifc_len / sizeof (struct ifreq); 
		while (intrface-- > 0){
			if (!(ioctl (iSocket, SIOCGIFADDR, (s8 *)&req[intrface]))){
				ip = inet_addr( inet_ntoa( ((struct sockaddr_in*)(&req[intrface].ifr_addr))->sin_addr) );
				AG_MODULE_LOG(LOG_INFO, "ip:"IP_FMT" uiIp:"IP_FMT, IP_ARG(&ip), IP_ARG(uiIp));
				if(ip == *uiIp){
					if (!(ioctl (iSocket, SIOCGIFHWADDR, (s8 *)&req[intrface])))
						memcpy(mac, req[intrface].ifr_hwaddr.sa_data, MAC_LEN);
					else{
						AG_MODULE_LOG(LOG_INFO, "Get WAN IF MAC error. line %d", __LINE__);
						close(iSocket);
						return FALSE;
					}
					break;
				}
			}
		}
		if(intrface < 0){
			AG_MODULE_LOG(LOG_DEBUG, "Get WAN IF MAC error. line %d", __LINE__);
			close(iSocket);
			return FALSE;
		}
	}
	close(iSocket);
	return TRUE;
}
s32 stam_module_get_if_mac(u8 *mac, s8 *port)
{
	s32 skdf;
	struct ifreq ifr;
	skdf = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == skdf){
	    AG_MODULE_LOG(LOG_ERR, "creat socket failed");
	    return -1;
	}
	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, port, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ -1] = '\0';
	if(ioctl(skdf, SIOCGIFHWADDR, &ifr)){
	    AG_MODULE_LOG(LOG_ERR, "ioctl's SIOCGIFHWADDR(%x) get %s mac faile, error: %s", 
	        SIOCGIFHWADDR, port, strerror(errno));
	    close(skdf);
	    return -1;
	}
	close(skdf);
	memcpy(mac, (u8 *)ifr.ifr_hwaddr.sa_data, MAC_LEN);
	return TRUE;
}
s32 stam_module_ipc_msg_package_register(u8 *buff, char *name, char *mac, int add)
{
	WIFIOG_EVENT_ADD_STA_T pstStaInfo;
	u32 uiIp = 0;
	u8 ucWanMac[MAC_LEN] = {0};
	struct timespec time = {0,0};
	memset(&pstStaInfo, 0, sizeof(WIFIOG_EVENT_ADD_STA_T));
	clock_gettime(CLOCK_MONOTONIC, &time);
	strncpy(pstStaInfo.acVapName, name, strlen(name));
	memcpy(pstStaInfo.aucStaMac, mac, MAC_LEN);
	//event_module_get_if_mac(ucWanMac,   PHY_IF);
	memcpy(pstStaInfo.aucApMac, gcWanMac, MAC_LEN);
	stam_module_get_essid(pstStaInfo.acVapName, pstStaInfo.acSsid);
	AG_MODULE_LOG(LOG_INFO, "ifname:%s mac:"MAC_FMT" AP IP:"IP_FMT" MAC:"MAC_FMT" ssid:%s", 
		pstStaInfo.acVapName, MAC_ARG(pstStaInfo.aucStaMac), IP_ARG(pstStaInfo.aucIp), MAC_ARG(pstStaInfo.aucApMac), pstStaInfo.acSsid);
	memcpy(buff, &pstStaInfo, sizeof(WIFIOG_EVENT_ADD_STA_T));

	stam_update_sta_list(mac, gcWanMac, pstStaInfo.acSsid, name, add, &time,NULL);
	return TRUE;
}
s32 stam_module_ipc_msg_package_expired(u8 *buff, char *name, char *mac)
{
	u32 uiIp = 0;
	u8 ucWanMac[MAC_LEN] = {0};
	WIFIOG_EVENT_DEL_STA_T pstStainfo;
	memset(&pstStainfo, 0, sizeof(WIFIOG_EVENT_DEL_STA_T));
	memcpy(pstStainfo.aucMac, mac, MAC_LEN);
	strncpy(pstStainfo.acVapName, name, strlen(name));
	memcpy(pstStainfo.aucApMac, gcWanMac, MAC_LEN);
	stam_module_get_essid(pstStainfo.acVapName, pstStainfo.acSsid);
	AG_MODULE_LOG(LOG_INFO, "ifname:%s mac:"MAC_FMT" AP ip:"IP_FMT"apmac:"MAC_FMT" ssid: %s", 
		pstStainfo.acVapName, MAC_ARG(pstStainfo.aucMac), IP_ARG(pstStainfo.aucIp),
		MAC_ARG(pstStainfo.aucApMac), pstStainfo.acSsid);
	memcpy(buff, &pstStainfo, sizeof(WIFIOG_EVENT_DEL_STA_T));
	stam_update_sta_list(mac, NULL, NULL, NULL, STAM_DEL_STA_INFO, NULL, NULL);
	return TRUE;
}
struct hlist_head *stam_mac_hashfn(u8 *mac, struct hlist_head list[], u32 hash_size)
{
	int	x;
	int	index;
	
	x = mac[0];
	x = (x << 2) ^ mac[1];
	x = (x << 2) ^ mac[2];
	x = (x << 2) ^ mac[3];
	x = (x << 2) ^ mac[4];
	x = (x << 2) ^ mac[5];

	x ^= x >> 8;

	index = x & (hash_size - 1);

	return &(list[index]);
}

s32 stam_printf_halist()
{
	int iLoop;
	struct hlist_head *head;
	struct hlist_node *p, *n;
	struct sta_node *sta;
	if(0 == access(AG_MODULE_PRINTF, F_OK))
	{
		for(iLoop = 0; iLoop <STA_HASH_SIZE; iLoop++)
		{
			head = &(gstStahlist[iLoop]);
			hlist_for_each(p, head)
			{
				sta = hlist_entry(p, struct sta_node, id_hlist);
				AG_MODULE_LOG(LOG_INFO, "add sta "MAC_FMT", essid:%s", MAC_ARG(sta->mac), sta->ap_ssid);
			}
		}
	}

	return TRUE;
}
s32 stam_sync_sta_connect_info()
{
	s32 iLoop;
	struct hlist_node *p;
	struct sta_node *sta;
	u8 ucSendBuf[AG_MODULE_BUFFER_SIZE] = {0};

	/*向网关同步当前终端关联信息*/
	AG_MODULE_LOG(LOG_INFO, "Sync sta info to gateway...");
	for(iLoop=0; iLoop < STA_HASH_SIZE; iLoop++){
		hlist_for_each(p, &gstStahlist[iLoop])
		{
			sta = hlist_entry(p, struct sta_node, id_hlist);
			
			stam_module_ipc_msg_package_register(ucSendBuf, sta->vap_name, sta->mac, STAM_ADD_STA_INFO);

			/*ag_module_ipc_client_send_msg(STAM_IDSET_EVENT_STAM_ADD_STA, AG_IPC_MSG_TYPE_SELF_CPU, &gstEventIPCmgt, \
			ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));*/
			if(STAM_DEV_IS_AP())
			{
				ag_module_ipc_client_send_msg(WIFIDOG_MSG_ADD_STA, AG_IPC_MSG_TYPE_OTHER_CPU, &gstStamIPCmgt, \
					ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));
				AG_MODULE_LOG(LOG_INFO, "Dev type is AP");
			}
			else
			{
				ag_module_ipc_client_send_msg(WIFIDOG_MSG_ADD_STA, AG_IPC_MSG_TYPE_SELF_CPU, &gstStamIPCmgt, \
					ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));
				AG_MODULE_LOG(LOG_INFO, "Dev type is GW");
			}
			/*ag_module_ipc_client_send_msg(AGLINK_IDSET_STA_ADD, AG_IPC_MSG_TYPE_OTHER_CPU, &gstEventIPCmgt, \
				ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));*/
		}
	}
	return TRUE;
}
s32 stam_update_sta_list(u8 *sta_mac, u8 *wan_mac, s8 *ap_ssid, s8* vap_name, int add, struct timespec *ptime, WIFI_API_STA_STATS_T *pstStaStats)
{
	struct hlist_node *p, *n;
	struct hlist_head *head;
	struct sta_node *sta;
	struct sta_node *newsta;
	s32 sta_exist = 0;

	STA_HLIST_LOCK();
	if(add)
	{
		head = stam_mac_hashfn(sta_mac, gstStahlist, STA_HASH_SIZE);
		hlist_for_each(p, head)
		{
			sta = hlist_entry(p, struct sta_node, id_hlist);
			if (!memcmp(sta->mac, sta_mac, MAC_LEN)){
				sta_exist = 1;
				break;
			}
		}
		/* not have this sta */
		if(!sta_exist)
		{
			newsta = (struct sta_node *)malloc(sizeof(struct sta_node));
			if(NULL == newsta)
			{
				AG_MODULE_LOG(LOG_ERR, "Allocate sta node failed!");
				STA_HLIST_UNLOCK();
				return FALSE;
			}
			memset(newsta, 0, sizeof(struct sta_node));
			memcpy(newsta->ap_mac, wan_mac, MAC_LEN);
			memcpy(newsta->mac, sta_mac, MAC_LEN);
			memcpy(newsta->ap_ssid, ap_ssid, strlen(ap_ssid));
			memcpy(newsta->vap_name, vap_name, strlen(vap_name));
			newsta->associate_time =  ptime->tv_sec;
			
			AG_MODULE_LOG(LOG_INFO, "add sta "MAC_FMT", essid:%s vap_name:%s  assoctm:%d", 
				MAC_ARG(newsta->mac), newsta->ap_ssid, newsta->vap_name, newsta->associate_time);
			hlist_add_head(&(newsta->id_hlist), head);
			list_add(&(newsta->stStaList), &g_stStaList);
		}
		/* update sta info */
		else
		{
			AG_MODULE_LOG(LOG_INFO, "Update sta info");
			memcpy(sta->ap_mac, wan_mac, MAC_LEN);
			memcpy(sta->ap_ssid, ap_ssid, strlen(ap_ssid));
			if(NULL != pstStaStats && NULL != time)
			{
				sta->rssi = pstStaStats->cStaRssi;
				sta->connect_time = ptime->tv_sec - sta->associate_time;
				sta->total_down = pstStaStats->ullTxDataBytes;
				sta->total_up = pstStaStats->ullRxDataBytes;
			}
		}
	}
	/* delete sta */
	else
	{
		head = stam_mac_hashfn(sta_mac, gstStahlist, STA_HASH_SIZE);
		hlist_for_each_safe(p, n, head)
		{
			sta = hlist_entry(p, struct sta_node, id_hlist);
			if (!memcmp(sta->mac, sta_mac, MAC_LEN)){
				hlist_del(&(sta->id_hlist));
				list_del(&(sta->stStaList));
				AG_MODULE_LOG(LOG_INFO, "del STA "MAC_FMT, MAC_ARG(sta->mac));
				free(sta);
				break;
			}
		}
	}
	STA_HLIST_UNLOCK();
	
	return TRUE;
}
s32 stam_get_user_stats(char *devname)
{
	u8 ucWanMac[MAC_LEN] = {0};
	int i, iRet;
	WIFI_API_ENDUSER_STATS_T stEndUserOmc = {0};
	struct timespec time = {0,0};
		

	if(NULL == devname)
		return -1;
	
	
	clock_gettime(CLOCK_MONOTONIC, &time);
	AG_MODULE_LOG(LOG_INFO, "get time %d sec",time.tv_sec);
	
#ifdef _BOXLL_
	/* BEGIN:fengjing add 2014-09-16 */
	char vapssid[32] = {0};
	
	iRet = wifi_api_vap_get_ssid(devname, vapssid, sizeof(vapssid));
	if(iRet < 0)
	{
		AG_MODULE_LOG(LOG_ERR, "there is no vap %s for devcie.", devname);
		
		return -1;	 
	}
	AG_MODULE_LOG(LOG_INFO, "there is %s,ssid %s", devname, vapssid);
	/* END:fengjing add 2014-09-16 */
#endif
	stEndUserOmc.ulBufLen = sizeof(WIFI_API_STA_STATS_T)*VAP_MAX_USER;
	stEndUserOmc.pstStaStats = malloc(stEndUserOmc.ulBufLen);
	if(NULL == stEndUserOmc.pstStaStats)
	{
		AG_MODULE_LOG(LOG_ERR, "%s:malloc fail\n",__func__);
		return -1;
	}
	memset(stEndUserOmc.pstStaStats,0,stEndUserOmc.ulBufLen);

	iRet = pal_wifi_vap_get_enduseromc(devname,&stEndUserOmc);
	
	if(iRet == 0)
	{
		for(i=0; i<stEndUserOmc.usTotalStaNumber; ++i){
			AG_MODULE_LOG(LOG_INFO, "get sta%d: "MAC_FMT"rssi:%d RxDataBytes:%lld, TxDataBytes:%lld", 
				i, MAC_ARG(stEndUserOmc.pstStaStats[i].aucStaMac), stEndUserOmc.pstStaStats[i].cStaRssi, stEndUserOmc.pstStaStats[i].ullRxDataBytes, stEndUserOmc.pstStaStats[i].ullTxDataBytes);

			/*将终端加入到终端链中*/
#ifdef _BOXLL_
			//aglink_update_sta_list(stEndUserOmc.pstStaStats[i].aucStaMac, NULL, vapssid, 1);
			//event_module_get_if_mac(ucWanMac, PHY_IF);

			stam_update_sta_list(stEndUserOmc.pstStaStats[i].aucStaMac, gcWanMac, vapssid, devname, STAM_ADD_STA_INFO, &time, &stEndUserOmc.pstStaStats[i]);
			
#else
			//aglink_update_sta_list(stEndUserOmc.pstStaStats[i].aucStaMac, NULL, 1);
#endif
		}
	}
	else
	{
		AG_MODULE_LOG(LOG_ERR, "%s:iRet = %d\n",__func__,iRet);
	}
	
	stam_printf_halist();
	
	free(stEndUserOmc.pstStaStats);
	return iRet;
}


s32 stam_get_sta_all(void)
{
	s8 func_str[16];
	s8 *args[1];

	sprintf(func_str, "%08x", &stam_get_user_stats);
	args[0] = &func_str[0];

	stam_get_vap_dev(args, 1);
	
	return 0;

}
#define IW_EV_LCP_PK_LEN (4)
static inline int print_event_stream(int ifindex, char *data, int len)
{
	u8 ucSendBuf[AG_MODULE_BUFFER_SIZE] = {0};
    unsigned char mac[6];
    struct iw_event	iwe;
    struct wireless_iface *	wireless_data;
    char command[128];

    /* Get data from cache */
    wireless_data = iw_get_interface_data(ifindex);
    if(wireless_data == NULL)
        return(-1);

    memset(&iwe, 0, sizeof(struct iw_event));
    memcpy((char *)&iwe, data, IW_EV_LCP_PK_LEN);

    switch(iwe.cmd)
    {
        case IWEVREGISTERED:
            printf("Discover a new node (AP mode)\n");
            memcpy(mac, data+6, 6);
            printf("vap: %s, sta:%02x:%02x:%02x:%02x:%02x:%02x\n",
                wireless_data->ifname,
                mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

            // 添加流控
            sta_flow_config(wireless_data->ifname, mac, 1);

            //添加用户表
            sprintf(command, "user addsta "MAC_FMT,MAC_AGR(mac));

		memset(ucSendBuf, 0, sizeof(AG_MODULE_BUFFER_SIZE));
		stam_module_ipc_msg_package_register(ucSendBuf, wireless_data->ifname, mac, STAM_ADD_STA_INFO);
		/*ag_module_ipc_client_send_msg(STAM_IDSET_EVENT_STAM_ADD_STA, AG_IPC_MSG_TYPE_SELF_CPU, &gstEventIPCmgt, \
			ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));*/

		if(STAM_DEV_IS_AP())
		{
			ag_module_ipc_client_send_msg(WIFIDOG_MSG_ADD_STA, AG_IPC_MSG_TYPE_OTHER_CPU, &gstStamIPCmgt, \
				ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));
			AG_MODULE_LOG(LOG_INFO, "Dev type is AP");
		}
		else
		{
			ag_module_ipc_client_send_msg(WIFIDOG_MSG_ADD_STA, AG_IPC_MSG_TYPE_SELF_CPU, &gstStamIPCmgt, \
				ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));
			AG_MODULE_LOG(LOG_INFO, "Dev type is GW");
		}

		/*ag_module_ipc_client_send_msg(SNMP_MSG_STA_ADD, AG_IPC_MSG_TYPE_OTHER_CPU, &gstStamIPCmgt, \
			ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));
		*/
		/*ag_module_ipc_client_send_msg(AGLINK_IDSET_STA_ADD, AG_IPC_MSG_TYPE_OTHER_CPU, &gstEventIPCmgt, \
			ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));*/
		++giStaNum;
            break;

        case IWEVEXPIRED:
            printf("Expired a node (AP mode)\n");
            memcpy(mac, data+6, 6);
            printf("vap: %s, sta:%02x:%02x:%02x:%02x:%02x:%02x\n",
                wireless_data->ifname,
                mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

            // 删除流控
            sta_flow_config(wireless_data->ifname, mac, 2);   

            //删除用户表
            sprintf(command, "user delsta "MAC_FMT,MAC_AGR(mac));  
		memset(ucSendBuf, 0, sizeof(AG_MODULE_BUFFER_SIZE));
		stam_module_ipc_msg_package_expired(ucSendBuf, wireless_data->ifname, mac);
		/*ag_module_ipc_client_send_msg(STAM_IDSET_EVENT_STAM_DEL_STA, AG_IPC_MSG_TYPE_SELF_CPU, &gstEventIPCmgt, \
			ucSendBuf, sizeof(WIFIOG_EVENT_DEL_STA_T));*/
		if(STAM_DEV_IS_AP())
		{
			ag_module_ipc_client_send_msg(WIFIDOG_MSG_DEL_STA, AG_IPC_MSG_TYPE_OTHER_CPU, &gstStamIPCmgt, \
				ucSendBuf, sizeof(WIFIOG_EVENT_DEL_STA_T));
			AG_MODULE_LOG(LOG_INFO, "Dev type is AP");
		}
		else
		{
			ag_module_ipc_client_send_msg(WIFIDOG_MSG_DEL_STA, AG_IPC_MSG_TYPE_SELF_CPU, &gstStamIPCmgt, \
				ucSendBuf, sizeof(WIFIOG_EVENT_DEL_STA_T));
			AG_MODULE_LOG(LOG_INFO, "Dev type is GW");
		}


		/*ag_module_ipc_client_send_msg(SNMP_MSG_STA_DEL, AG_IPC_MSG_TYPE_OTHER_CPU, &gstStamIPCmgt, \
			ucSendBuf, sizeof(WIFIOG_EVENT_DEL_STA_T));*/
		/*memset(ucSendBuf, 0, sizeof(ucSendBuf));
		stam_module_ipc_msg_package_register(ucSendBuf, wireless_data->ifname, mac, 0);
		ag_module_ipc_client_send_msg(AGLINK_IDSET_STA_DEL, AG_IPC_MSG_TYPE_OTHER_CPU, &gstEventIPCmgt, \
			ucSendBuf, sizeof(WIFIOG_EVENT_ADD_STA_T));*/
		--giStaNum;
		if(0 > giStaNum)
			giStaNum = 0;
		
            break;
        default:
            break;
    }
    system(command);
    
    return(0);
}


static int LinkCatcher(struct nlmsghdr *nlh)
{
    struct ifinfomsg* ifi;

    //fprintf(stderr, "nlmsg_type = %d.\n", nlh->nlmsg_type);
    ifi = NLMSG_DATA(nlh);

    /* Code is ugly, but sort of works - Jean II */

    /* If interface is getting destoyed */
    if(nlh->nlmsg_type == RTM_DELLINK) {
        /* Remove from cache (if in cache) */
        //iw_del_interface_data(ifi->ifi_index);
        return 0;
    }

    /* Only keep add/change events */
    if(nlh->nlmsg_type != RTM_NEWLINK)
        return 0;

    /* Check for attributes */
    if (nlh->nlmsg_len > NLMSG_ALIGN(sizeof(struct ifinfomsg))) {
        int attrlen = nlh->nlmsg_len - NLMSG_ALIGN(sizeof(struct ifinfomsg));
        struct rtattr *attr = (void *) ((char *) ifi +
                                        NLMSG_ALIGN(sizeof(struct ifinfomsg)));

        while (RTA_OK(attr, attrlen)) {
            /* Check if the Wireless kind */
            if(attr->rta_type == IFLA_WIRELESS) {
                /* Go to display it */
                print_event_stream(ifi->ifi_index,
                                   (char *) attr + RTA_ALIGN(sizeof(struct rtattr)),
                                   attr->rta_len - RTA_ALIGN(sizeof(struct rtattr)));
             }
            attr = RTA_NEXT(attr, attrlen);
        }
    }

    return 0;
}

s32 stam_module_sta_ip_update(u8 *pBuff)
{
	struct hlist_node *p, *n;
	struct hlist_head *head;
	struct sta_node *sta;
	s32 sta_exist = 0;
	USER_STA_INFO_T stStaInfo = {0};

	memcpy(&stStaInfo, pBuff, sizeof(USER_STA_INFO_T));

	AG_MODULE_LOG(LOG_INFO, "Sta MAC:"MAC_FMT" IP:"IP_FMT, MAC_ARG(stStaInfo.acMac), IP_ARG(&stStaInfo.uiIpAddr));

	STA_HLIST_LOCK();
	head = stam_mac_hashfn(stStaInfo.acMac, gstStahlist, STA_HASH_SIZE);
	hlist_for_each(p, head)
	{
		sta = hlist_entry(p, struct sta_node, id_hlist);
		if (!memcmp(sta->mac, stStaInfo.acMac, MAC_LEN)){
			sta_exist = 1;
			break;
		}
	}
	/*find it*/
	if(sta_exist)
	{		
		memcpy(&sta->aucIp, (u8 *)&stStaInfo.uiIpAddr, sizeof(stStaInfo.uiIpAddr));
		
		AG_MODULE_LOG(LOG_INFO, "STA MAC:"MAC_FMT" ip:"IP_FMT" RSSI:%d connect_time:%d", 
			MAC_ARG(sta->mac), IP_ARG(sta->aucIp), sta->rssi, sta->connect_time);
	}
	/*not find it*/
	else
	{			
		AG_MODULE_LOG(LOG_ALERT, "Can`t find this sta");
	}
	STA_HLIST_UNLOCK();

	stam_module_send_sta_list(giStaNum);
	
	return TRUE;
}

u32 stam_module_parse_netlink_msg(evutil_socket_t fd, short event, void *arg)
{
	u8 *p;
	u32 ulLength = 0, iRet = 0;
	u8 ucRecvbuf[AG_MODULE_BUFFER_SIZE] = {0};
	struct sockaddr_nl stNlPeer;
    	u32 iNlSockLen = sizeof(struct sockaddr_nl);
	NETLINK_EVENT stNetEvent = {0};
	
	iRet = recvfrom(fd, (void *)ucRecvbuf, AG_MODULE_BUFFER_SIZE, 0,  (struct sockaddr*)&stNlPeer, &iNlSockLen);
	if(-1 == iRet)
	{
		AG_MODULE_LOG(LOG_ERR, "receive data error!\n");
		return FALSE;
	}

	p = ucRecvbuf;
	p +=sizeof(struct nlmsghdr);

	AG_MODULE_LOG(LOG_INFO, "Msg length:%d  date length:%d sizeof(NETLINK_EVENT):%d", iRet, iRet-sizeof(struct nlmsghdr), sizeof(NETLINK_EVENT));

	memcpy(&stNetEvent, p, sizeof(NETLINK_EVENT));

	switch(stNetEvent.uiEventId)
	{
		case NETLINK_MSG_ID_STA_IP_UPDATE:
			AG_MODULE_LOG(LOG_INFO, "Recv msg NETLINK_MSG_ID_STA_IP, MsgLength:%d ", stNetEvent.uiMsgLen);
			stam_module_sta_ip_update(stNetEvent.ucBuff);
			break;
		default:
			AG_MODULE_LOG(LOG_ALERT, "Event id is:%d", stNetEvent.uiEventId);
			break;
	}
	
	return TRUE;
}

u32 stam_module_ipc_parse_msg(u8 *buf, u32 len, struct sockaddr_in *sk_addr)
{
	u8 *p = buf;
	u8 **pnt = &p;
	u32 iRet;
	AG_LINK_MSG_HEAD_T  stMsgHead;
	AG_LINK_MSG_HEAD_T *pstMsgHead;
	if(len <= 0)
	{
		AG_MODULE_LOG(LOG_ERR, "Msg length <= 0");
		return FALSE;
	}

	memset(&stMsgHead, 0 ,sizeof(AG_LINK_MSG_HEAD_T));
	pstMsgHead = &stMsgHead;
	
	DECODE_GETL(pnt, stMsgHead.usMsgId);
	DECODE_GETW(pnt, stMsgHead.usMsgType);
	DECODE_GETW(pnt, stMsgHead.uiUnicast);
	DECODE_GET(stMsgHead.acname, pnt, MODULE_NAME_LENGTH);
	DECODE_GETL(pnt, stMsgHead.uiMsgLen);
	
	AG_MODULE_LOG(LOG_INFO, "acname:%s usMsgId:%d usMsgType:%d uiMsgLen:%d  msg total len:%d", 
		pstMsgHead->acname, pstMsgHead->usMsgId, pstMsgHead->usMsgType, pstMsgHead->uiMsgLen, len);
	switch(pstMsgHead->usMsgId)
	{
		case AG_MSG_IPC_REGISTER_ACK:
			gstStamIPCmgt.uiFlag = 1;
			AG_MODULE_LOG(LOG_INFO, "RECV AG_IPC_MSG_REGISTER_ACK msg");
			stam_get_sta_all();
			stam_sync_sta_connect_info();
			event_add(&gstStamEvent.stStaSyn_ev, &gstStamEvent.stStaSyn_tv);
			break;
		case AG_MSG_IPC_UNREGISTER:
			AG_MODULE_LOG(LOG_INFO, "RECV AG_IPC_MSG_UNREGISTER msg");
			gstStamIPCmgt.uiFlag = 0;
			ag_module_ipc_reconnect_server(&gstStamIPCmgt);
			event_del(&gstStamEvent.stStaSyn_ev);
			break;
		case BC_MSG_AP_REGISTER_TO_GW_SUCCESS:
			AG_MODULE_LOG(LOG_INFO, "RECV BC_MSG_AP_REGISTER_TO_GW_SUCCESS msg");
			stam_get_sta_all();
			stam_sync_sta_connect_info();
			break;
		default:
           		 AG_MODULE_LOG(LOG_INFO, "UNKOWN MSG");
            	break;
	}
	return TRUE;
}
s32 stam_module_ipc_recv_msg(evutil_socket_t fd, short event, void *arg)
{
	s32 size;
	s32 sockfd = gstStamIPCmgt.uipcSocket;
	u8 ucRecvbuf[AG_MODULE_BUFFER_SIZE] = {0};
	struct sockaddr_in sk_addr;
	socklen_t len = sizeof(sk_addr);
	AG_MODULE_LOG(LOG_INFO, "fd:%d sockfd:%d", fd, sockfd);
	size = recvfrom(sockfd, (void *)ucRecvbuf, AG_MODULE_BUFFER_SIZE, 0, (struct sockaddr *)&sk_addr, &len);
	if(-1 == size)
	{
		AG_MODULE_LOG(LOG_ERR, "receive data error!\n");
		return FALSE;
	}
	else
	{
		stam_module_ipc_parse_msg(ucRecvbuf, size, &sk_addr);
	}
	return TRUE;
}

s32 stam_module_send_sta_list(s32 StaNum)
{
	struct sta_node *ptmp = NULL;
	STAM_ALL_STA_LIST *pstStaList = NULL;
	s32 iLoop = 0, iLength;


	AG_MODULE_LOG(LOG_INFO, "Sta Num:%d", StaNum);
	if(0 >= StaNum)
	{
		return TRUE;
	}
	
	iLength = 16 + (sizeof(SNMP_EVENT_STA_INFO)*StaNum);
	pstStaList = (STAM_ALL_STA_LIST *)malloc(iLength);
	memset(pstStaList, 0, iLength);
	
	if(NULL == pstStaList)
	{
		AG_MODULE_LOG(LOG_ALERT, "malloc fail");
		return FALSE;
	}

	memcpy(pstStaList->aucApMac, gcWanMac, MAC_LEN);
	pstStaList->iStaNum = StaNum;
	pstStaList->iFlag = 1;

	AG_MODULE_LOG(LOG_INFO, "AP mac:"MAC_FMT" StaNum:%d ", MAC_ARG(pstStaList->aucApMac),pstStaList->iStaNum);

	STA_HLIST_LOCK();
	list_for_each_entry(ptmp, &g_stStaList, stStaList)
	{
		if(iLoop >= StaNum)
		{
			break;
		}
		memcpy(&pstStaList->astStaInfo[iLoop++], ptmp, sizeof(SNMP_EVENT_STA_INFO));
		AG_MODULE_LOG(LOG_INFO, "AP MAC:"MAC_FMT" IP:"IP_FMT" ssid:%s vap_name:%s rssi:%d connect_time:%d total_up:%lld total_down:%lld", 
			MAC_ARG(ptmp->ap_mac), IP_ARG(ptmp->aucIp), ptmp->ap_ssid, ptmp->vap_name, ptmp->rssi, ptmp->connect_time, ptmp->total_up, ptmp->total_down);
		
	}

	for(iLoop = 0; iLoop < pstStaList->iStaNum; iLoop++)
	{
		AG_HTONL(pstStaList->astStaInfo[iLoop].rssi);
		AG_HTONL(pstStaList->astStaInfo[iLoop].connect_time);
		AG_HTONL(pstStaList->astStaInfo[iLoop].total_up);
		AG_HTONL(pstStaList->astStaInfo[iLoop].total_down);
	}
	
	AG_HTONL(pstStaList->iStaNum);
	AG_HTONL(pstStaList->iFlag);

	if(STAM_DEV_IS_AP())
	{
		ag_module_ipc_client_send_msg(SNMP_MSG_STA_INFO, AG_IPC_MSG_TYPE_OTHER_CPU, &gstStamIPCmgt, (u8 *)pstStaList, iLength);
		AG_MODULE_LOG(LOG_INFO, "Dev type is AP");
	}
	else
	{
		ag_module_ipc_client_send_msg(SNMP_MSG_STA_INFO, AG_IPC_MSG_TYPE_SELF_CPU, &gstStamIPCmgt, (u8 *)pstStaList, iLength);
		AG_MODULE_LOG(LOG_INFO, "Dev type is GW");
	}


	STA_HLIST_UNLOCK();

	free(pstStaList);
	
	return TRUE;
}

s32 stam_sync_sta_info_cycle(s32 fd, short event, void *arg)
{
	FILE *pFile;
	struct sta_node *ptmp;
	u8 acShellCmd[128] = {0}, acBuffer[128] = {0};
	u32 iRet = 0, ip[4], iLoop, iStaNum = 0;

	/*更新终端列表信息，除了sta IP*/
	stam_get_sta_all();

	/*获取IP*/
	STA_HLIST_LOCK();
	list_for_each_entry(ptmp, &g_stStaList, stStaList)
	{
		iStaNum++;
		memset(acShellCmd, 0 ,sizeof(acShellCmd));
		sprintf(acShellCmd, "user  getsta "MAC_FMT" > "STAM_STA_IP_FILE, MAC_ARG(ptmp->mac));
		iRet = system(acShellCmd);
		if (0 != iRet)
		{
		    AG_MODULE_LOG(LOG_ALERT, "user getsta "MAC_FMT" fail!", MAC_ARG(ptmp->mac));
		    continue;  
		}
		pFile = fopen(STAM_STA_IP_FILE, "rb");
		if(NULL == pFile)
		{
			AG_MODULE_LOG(LOG_ALERT, "Open "STAM_STA_IP_FILE" fail");
		    	continue;  
		}

		memset(acBuffer, 0, sizeof(acBuffer));
		fread(acBuffer, 1, sizeof(acBuffer), pFile); 
            	fclose(pFile);
		sscanf(acBuffer, "%u.%u.%u.%u",&ip[0],&ip[1],&ip[2],&ip[3]);
		for(iLoop = 0; iLoop < 4; iLoop++)
		{
			ptmp->aucIp[iLoop] = (u8)ip[iLoop];
		}

		AG_MODULE_LOG(LOG_INFO, "MAC: "MAC_FMT" IP:"IP_FMT, MAC_ARG(ptmp->mac), IP_ARG(ptmp->aucIp));
	}
	STA_HLIST_UNLOCK();

	stam_module_send_sta_list(iStaNum);
	event_add(&gstStamEvent.stStaSyn_ev, &gstStamEvent.stStaSyn_tv);
	return TRUE;
}


s32 stam_module_netlink_init(s32 *psocket)
{
	int iRet;
    	struct sockaddr_nl stNlAddr;

	/* 创建 netlink socket*/
	*psocket = socket(AF_NETLINK, SOCK_RAW, NETLINK_EVENT_PROTOCOL);
	if (*psocket < 0)
	{
		return FALSE;
	}

	memset(&stNlAddr, 0, sizeof(struct sockaddr_nl));
    	stNlAddr.nl_family = AF_NETLINK;
	stNlAddr.nl_pad = 0;
	stNlAddr.nl_pid = getpid();
	stNlAddr.nl_groups = NETLINK_EVENT_GROUPS;
	iRet = bind(*psocket, (struct sockaddr*)&stNlAddr, sizeof(stNlAddr));
	if (iRet < 0)
	{
		AG_MODULE_LOG(LOG_INFO, "Create Netlink socket error.%s(errno: %d)", strerror(errno), errno);
	    	return FALSE;
	}
	
	return TRUE;
}

s32 stam_module_event_init(void)
{
	s32 iRet = 0;
	gstStamEvent.stStaSyn_tv.tv_sec = STAM_STA_INFO_SYN_TM;
	gstStamEvent.stStaSyn_tv.tv_usec = 0;

	evtimer_set(&gstStamEvent.stStaSyn_ev, stam_sync_sta_info_cycle, NULL);

	iRet = stam_module_netlink_init(&gstStamEvent.Netlinkfd);
	if(FALSE == iRet)
	{
		AG_MODULE_LOG(LOG_INFO, "Create Netlink socket error");
	}
	else
	{
		event_set(&gstStamEvent.stNetlinkUser_ev, gstStamEvent.Netlinkfd,  EV_READ | EV_PERSIST, 
			stam_module_parse_netlink_msg, (void *)&gstStamEvent.stNetlinkUser_ev);
		event_add(&gstStamEvent.stNetlinkUser_ev, NULL);
	}
	
	return TRUE;
}

s32 stam_module_register()
{
	u32 uiModuleId = 0;
	u32 uiLoop;
	u32 iRet;
	#if 0
	for(uiLoop = EVENT_IDSET_BEGIN; uiLoop < EVENT_IDSET_MAX; uiLoop++)
	{
		uiModuleId |= uiLoop; 
	}
	#endif
	uiModuleId = (u32)AG_GET_MODULE_ID(STAM_MSG_BEGIN);
	AG_MODULE_LOG(LOG_INFO, "uiModuleId is :%d", uiModuleId);
	memset(&gstStamIPCmgt, 0, sizeof(gstStamIPCmgt));
	iRet = ag_module_ipc_module_init("stam", uiModuleId, &gstStamIPCmgt, stam_module_ipc_recv_msg);
	return iRet;
}
void * stam_module_ipc_start()
{
	s32 iRet = 0;
	
	stam_module_get_if_mac(gcWanMac,   PHY_IF);

	iRet = stam_module_register();
	if(FALSE == iRet)
	{
		AG_MODULE_LOG(LOG_ALERT, "Register fail!");
	}

	stam_module_event_init();
	AG_MODULE_LOG(LOG_INFO, "Power on");
	ag_module_ipc_power_on(&gstStamIPCmgt);
	AG_MODULE_LOG(LOG_INFO, "exit");
	return 0;
}
s32 stam_module_ipc_thread_entry()
{
	s32 iRet;
	pthread_t stpthread;
	iRet = pthread_create(&stpthread, NULL, (void*)stam_module_ipc_start, NULL);
	if(0 != iRet)
	{
		AG_MODULE_LOG(LOG_ALERT, "Create pthread fail!");
		return FALSE;
	}
	AG_MODULE_LOG(LOG_ALERT, "thread start success!");

	//pthread_join(stpthread, NULL);
	return TRUE;
}

s32 stam_stop_process(void)
{
	struct sta_node *stTmpSta, *stTmpStaNext;
	
	/*free sta */
	list_for_each_entry_safe(stTmpSta, stTmpStaNext, &g_stStaList, stStaList)
	{
		hlist_del(&(stTmpSta->id_hlist));
  		list_del(&(stTmpSta->stStaList));
 		 free(stTmpSta);
	}
	
	return 0;
}

s32 stam_get_dev_type(void)
{
	#ifdef OPENWRT
	giDevType =  STAM_DEV_TYPE_GW;
	AG_MODULE_LOG(LOG_INFO, "dev type Gateway.");
	#else
	
	s8 forward_mode[NVRAM_LEN*2] = {0};
	s32 iRet = -1;
	
	iRet = nvram_get(forward_mode, "net_forward_mode", (NVRAM_LEN*2));

	/* default is ap module */
	if(0 != iRet)
	{
		giDevType = STAM_DEV_TYPE_AP;
	}
	/*  */
	else
	{
		if(!strcmp(forward_mode, "2")){
			giDevType = STAM_DEV_TYPE_AP;
			AG_MODULE_LOG(LOG_INFO, "dev type AP.");
		}
		else{
			giDevType =  STAM_DEV_TYPE_GW;
			AG_MODULE_LOG(LOG_INFO, "dev type Gateway.");
		}
	}
	#endif
	return TRUE;
}

void stam_handle_signal(int signo)
{
	AG_MODULE_LOG(LOG_INFO, "Signo is %d\n", signo);

	/*if support backtrace*/
	#if 0
	get_backtrace();
	#endif

	stam_stop_process();
    	exit(0);
}

s32 stam_init_signal(void)
{
	struct sigaction stSig;

	stSig.sa_flags = 0;
	sigemptyset(&stSig.sa_mask);

	sigaddset(&stSig.sa_mask, SIGSEGV);
	sigaddset(&stSig.sa_mask, SIGBUS);
	sigaddset(&stSig.sa_mask, SIGTERM);
	sigaddset(&stSig.sa_mask, SIGHUP);
	sigaddset(&stSig.sa_mask, SIGINT);
	sigaddset(&stSig.sa_mask, SIGPIPE);
	sigaddset(&stSig.sa_mask, SIGALRM);

	stSig.sa_handler = stam_handle_signal;
	sigaction(SIGSEGV, &stSig, NULL);
	sigaction(SIGBUS, &stSig, NULL);
	sigaction(SIGTERM, &stSig, NULL);
	sigaction(SIGHUP, &stSig, NULL);
	sigaction(SIGINT, &stSig, NULL);
	sigaction(SIGPIPE, &stSig, NULL);
	sigaction(SIGALRM, &stSig, NULL);

	/*慢系统调用被信号中断后可能会重启，关闭重启或者将sa_flags设置为取消SA_RESTART标志*/
	siginterrupt(SIGALRM, 1);
	siginterrupt(SIGBUS, 1);
	siginterrupt(SIGTERM, 1);
	siginterrupt(SIGHUP, 1);
	siginterrupt(SIGKILL, 1);
	siginterrupt(SIGSEGV, 1);
	siginterrupt(SIGPIPE, 1);

	return 0;
}



/* start main */
int main(int argc, char *argv[])
{
    char c;
    struct rtnl_handle rth;
    char *pid_path = NULL;

	stam_init_signal();
    while(-1 != (c = getopt(argc, argv, EVENT_OPT))) {
        switch(c) {
            case 'p':
                pid_path = optarg;
                break;
            case 'h':
                usage();
                break;
            case 'v':
                //printf("Version 1.0 (GBCOM)\n");
                printf("Version 1.0\n");
                break;
            default:
                printf("Cannot find %c, Try 'event -h' for usage.\n", c);
                break;
        }
    }

    if(NULL != pid_path) {
        write_pid(pid_path);
    }
	pthread_mutex_init(&sta_hlist_mutex, NULL);	
	stam_get_dev_type();
	
	if(TRUE != stam_module_ipc_thread_entry())
	{
		AG_MODULE_LOG(LOG_ALERT, "Create thread fail!");
		exit(-1);
	}
    if(rtnl_open(&rth, RTMGRP_LINK) < 0) {
        perror("Can't initialize rtnetlink socket");
        return(1);
    }
    wait_for_event(&rth);
    rtnl_close(&rth);
    return (0);
}/* end main */



