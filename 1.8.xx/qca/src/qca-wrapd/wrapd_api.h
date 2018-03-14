#ifndef _WRAPD_API__H
#define _WRAPD_API__H

#if 1
#define wrapd_printf(fmt, args...) do { \
        printf("wrapd: %s: %d: " fmt "\n", __func__, __LINE__, ## args); \
} while (0)
#else
#define wrapd_printf(args...) do { } while (0)
#endif

#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#endif
#define MAC2STR_ADDR(a) &(a)[0], &(a)[1], &(a)[2], &(a)[3], &(a)[4], &(a)[5]

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/* 
 * HOSTAPD_CNT can be 2 for DBDC qwrap case
 * vaps on 2G and 5G radios
 */
#define HOSTAPD_CNT     2

typedef void wrapd_hdl_t;

typedef unsigned char mac_addr_t[ETH_ALEN];
typedef unsigned int mac_addr_int_t[ETH_ALEN];

typedef enum {
WRAPD_STATUS_OK = 0,
WRAPD_STATUS_ETIMEDOUT,
WRAPD_STATUS_FAILED,
WRAPD_STATUS_BAD_ARG,
WRAPD_STATUS_IN_USE,
WRAPD_STATUS_ARG_TOO_BIG,
} wrapd_status_t;

#ifdef QSDK_WRAPD
wrapd_hdl_t *wrapd_conn_to_global_wpa_s(const char *ifname, const char *wifi_ifname, const char *confname, int isolation, int timer);
#else
wrapd_hdl_t *wrapd_conn_to_global_wpa_s(const char *ifname, const char *confname, int isolation, int timer);
#endif

void wrapd_ctrl_iface_receive(int sock, void *eloop_ctx, void *sock_ctx);
void wrapd_hostapd_ctrl_iface_receive(int sock, void *eloop_ctx, void *sock_ctx);
void wrapd_wpa_s_ctrl_iface_receive(int sock, void *eloop_ctx, void *sock_ctx);
void wrapd_load_vma_list(const char *conf_file, wrapd_hdl_t *handle);
void wrapd_ifname_to_parent_ifname(wrapd_hdl_t *handle, char *child, char *parent);
#ifdef QSDK_WRAPD
void free_predefined_vma_list(wrapd_hdl_t *handle);
#endif
#endif //_WRAPD_API__H
