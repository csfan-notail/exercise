// vim: set et sw=4 sts=4 cindent:
/*
 * @File: wlanifBSteerControl.c
 *
 * @Abstract: Load balancing daemon band steering control interface
 *
 * @Notes:
 *
 * @@-COPYRIGHT-START-@@
 *
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 * @@-COPYRIGHT-END-@@
 *
 */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/types.h>
#define _LINUX_IF_H /* Avoid redefinition of stuff */
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/wireless.h>
#include <net/if_arp.h>   // for ARPHRD_ETHER
#include <errno.h>

#include "ieee80211_external.h"

#ifdef GMOCK_UNIT_TESTS
#include "strlcpy.h"
#endif

#include <dbg.h>
#include <split.h>
#include <evloop.h>

#include "profile.h"
#include "module.h"
#include "internal.h"

#include "wlanifPrivate.h"
#include "wlanifBSteerControl.h"

// forward decls
static LBD_BOOL wlanifBSteerControlIsBandValid(wlanifBSteerControlHandle_t state, wlanif_band_e band);
static const char * wlanifBSteerControlGetBandIfname(wlanifBSteerControlHandle_t state, wlanif_band_e band);
static struct wlanifBSteerControlVapInfo * wlanifBSteerControlAllocateVap(wlanifBSteerControlHandle_t state,
                                                                          wlanif_band_e band);
static struct wlanifBSteerControlVapInfo * wlanifBSteerControlInitVapFromIfname(wlanifBSteerControlHandle_t state,
                                                                                const char *ifname);
static LBD_STATUS wlanifBSteerControlResolveWlanIfaces(wlanifBSteerControlHandle_t state);

static LBD_STATUS wlanifBSteerControlInitializeACLs(
        wlanifBSteerControlHandle_t handle, wlanif_band_e band);
static void wlanifBSteerControlFlushACLs(
        wlanifBSteerControlHandle_t handle, wlanif_band_e band);
static void wlanifBSteerControlTeardownACLs(
        wlanifBSteerControlHandle_t handle, wlanif_band_e band);

static LBD_STATUS wlanifBSteerControlReadConfig(wlanifBSteerControlHandle_t state, wlanif_band_e band);
static LBD_STATUS wlanifBSteerControlSendEnable(wlanifBSteerControlHandle_t state, wlanif_band_e band, LBD_BOOL enable);
static LBD_STATUS wlanifBSteerControlSendSetParams(wlanifBSteerControlHandle_t state, wlanif_band_e band);
static LBD_STATUS wlanifBSteerControlSendRequestRSSI(wlanifBSteerControlHandle_t state, wlanif_band_e band,
                                                     const struct ether_addr * staAddr, u_int8_t numSamples);
static LBD_STATUS wlanifBSteerControlSend(wlanifBSteerControlHandle_t state,
                                          wlanif_band_e band, u_int8_t cmd,
                                          const struct ether_addr *destAddr,
                                          void *data, int data_len);
static LBD_STATUS wlanifBSteerControlSendVAP(wlanifBSteerControlHandle_t state,
                                             const char *ifname, u_int8_t cmd,
                                             const struct ether_addr *destAddr,
                                             void *data, int data_len);

// ====================================================================
// Internal types
// ====================================================================

/**
 * @brief internal structure for VAP information
 */
struct wlanifBSteerControlVapInfo {
    // flag indicating if this VAP is valid
    LBD_BOOL valid;

    // interface name, +1 to ensure it is null-terminated
    char ifname[IFNAMSIZ + 1];

    // system index
    int sysIndex;

    // Whether the interface is considered up or not
    LBD_BOOL ifaceUp;

    // frequency on which this VAP is operating
    u_int32_t freq;
};

// Maximum number of VAPs on a single band
#define MAX_VAP_PER_BAND 16

/**
 * @brief internal structure for the STA whose RSSI is requested
 */
typedef struct {
    // Double-linked list for use in a given list
    list_head_t listChain;

    // The MAC address of the STA whose RSSI is requested
    struct ether_addr addr;

    // number of RSSI samples to average before reporting RSSI back
    u_int8_t numSamples;
} wlanifBSteerControlRSSIRequestEntry_t;

/**
 * @brief internal structure for band information
 */
struct wlanifBSteerControlBandInfo {
    // All VAPs on this band
    struct wlanifBSteerControlVapInfo vaps[MAX_VAP_PER_BAND];

    // config parameters
    ieee80211_bsteering_param_t configParams;

    // flag indicating if band steering is enabled on this band
    LBD_BOOL enabled;

    // a list of STAs whose RSSI measurement is requested
    list_head_t rssiWaitingList;
};

struct wlanifBSteerControlPriv_t {
    struct dbgModule *dbgModule;
    struct wlanifBSteerControlBandInfo bandInfo[wlanif_band_invalid];

    // Socket used to send control request down to driver
    int controlSock;

    /// Timer used to periodically check whether ACS and CAC have completed
    struct evloopTimeout vapReadyTimeout;

    /// Flag indicating whether band steering is currently enabled.
    LBD_BOOL bandSteeringEnabled;
};

struct profileElement wlanifElementDefaultTable_24G[] = {
    {WLANIFBSTEERCONTROL_INACT_IDLE_THRESHOLD,        "60"},
    {WLANIFBSTEERCONTROL_INACT_OVERLOAD_THRESHOLD,    "30"},
    {WLANIFBSTEERCONTROL_INACT_RSSI_XING_THRESHOLD,   "50"},
    {WLANIFBSTEERCONTROL_LOW_RSSI_XING_THRESHOLD,     "10"},
    {WLANIFBSTEERCONTROL_MU_CHECK_INTERVAL,           "10"},
    {WLANIFBSTEERCONTROL_MU_AVG_PERIOD,               "300"},
    {WLANIFBSTEERCONTROL_INACT_CHECK_INTERVAL,        "10"},
    {NULL,                              NULL},
};

struct profileElement wlanifElementDefaultTable_5G[] = {
    {WLANIFBSTEERCONTROL_INACT_IDLE_THRESHOLD,        "60"},
    {WLANIFBSTEERCONTROL_INACT_OVERLOAD_THRESHOLD,    "30"},
    {WLANIFBSTEERCONTROL_INACT_RSSI_XING_THRESHOLD,   "10"},
    {WLANIFBSTEERCONTROL_LOW_RSSI_XING_THRESHOLD,     "10"},
    {WLANIFBSTEERCONTROL_MU_CHECK_INTERVAL,           "10"},
    {WLANIFBSTEERCONTROL_MU_AVG_PERIOD,               "300"},
    {WLANIFBSTEERCONTROL_INACT_CHECK_INTERVAL,        "10"},
    {NULL,                              NULL},
};

/*========================================================================*/
/*============ Internal handling =========================================*/
/*========================================================================*/

/**
 * @brief Enable or disable the band steering feature on a given band.
 *
 * @pre state and band are valid
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to enable/disable band steering feature
 * @param [in] band  The band on which to enable/disable band steering feature
 * @param [in] enable  LBD_TRUE for enable, LBD_FALSE for disable
 *
 * @return LBD_OK on successful enable/disable; otherwise LBD_NOK
 */
static LBD_STATUS wlanifBSteerControlSetEnable(
        wlanifBSteerControlHandle_t state, wlanif_band_e band, LBD_BOOL enable)
{
    // TODO Dan: There is no check for whether band is resolved here, since only
    //           after both bands are resolved, a valid handle will be returned.
    //           But it may be necessary after we add periodically VAP
    //           monitoring logic, some band may become invalid.

    if (enable && wlanifBSteerControlSendSetParams(state, band) == LBD_NOK) {
        dbgf(state->dbgModule, DBGERR, "%s: Failed to set band steering parameters on band %u",
             __func__, band);
        return LBD_NOK;
    }

    if (wlanifBSteerControlSendEnable(state, band, enable) == LBD_NOK) {
        dbgf(state->dbgModule, DBGERR, "%s: Failed to %s band steering on band %u",
             __func__, enable ? "enable" : "disable", band);
        return LBD_NOK;
    }

    state->bandInfo[band].enabled = enable;

    dbgf(state->dbgModule, DBGINFO, "%s: Successfully %s band steering on band %u",
         __func__, enable ? "enabled" : "disabled", band);

    return LBD_OK;
}

/**
 * @brief Check if for a given band, there is at least one valid VAP
 *
 * @param [in] state  the "this" pointer
 * @param [in] band  the band to check
 *
 * @return LBD_TRUE if at least one VAP is valid; otherwise LBD_FALSE
 */
static LBD_BOOL
wlanifBSteerControlIsBandValid(wlanifBSteerControlHandle_t state, wlanif_band_e band)
{
    int i;
    for (i = 0; i < MAX_VAP_PER_BAND; ++i) {
        if (state->bandInfo[band].vaps[i].valid) {
            return LBD_TRUE;
        }
    }
    return LBD_FALSE;
}

/**
 * @brief For a given band, get a valid interface name
 *
 * Note that currently it will always return the first VAPs ifname, since
 * we do not have logic to invalidate a VAP yet.
 *
 * @param [in] state  the "this" pointer
 * @param [in] band  the band to get interface name
 *
 * @return the interface name if any; otherwise NULL
 */
static const char *
wlanifBSteerControlGetBandIfname(wlanifBSteerControlHandle_t state, wlanif_band_e band)
{
    int i;
    for (i = 0; i < MAX_VAP_PER_BAND; ++i) {
        if (state->bandInfo[band].vaps[i].valid) {
            return state->bandInfo[band].vaps[i].ifname;
        }
    }
    return NULL;
}

/**
 * @brief For a given band, get an empty entry for VAP information
 *
 * @param [in] state  the "this" pointer
 * @param [in] band  the band to get VAP entry
 *
 * @return the empty VAP entry if any; otherwise NULL
 */
static struct wlanifBSteerControlVapInfo *
wlanifBSteerControlAllocateVap(wlanifBSteerControlHandle_t state, wlanif_band_e band)
{
    int i;
    for (i = 0; i < MAX_VAP_PER_BAND; ++i) {
        if (!state->bandInfo[band].vaps[i].valid) {
            memset(&state->bandInfo[band].vaps[i], 0, sizeof(state->bandInfo[band].vaps[i]));
            return &state->bandInfo[band].vaps[i];
        }
    }
    dbgf(state->dbgModule, DBGERR, "%s: No available VAP entry on band %u; "
                                   "maximum number of VAPs allowed on one band: %u",
        __func__, band, MAX_VAP_PER_BAND);
    return NULL;
}

/**
 * @brief For a given interface name, resolve a VAP entry on the corresponding band
 *
 * Once successfully resolved, the VAP entry will be marked valid, with frequency and
 * system index information
 *
 * @param [in] state  the "this" pointer
 * @param [in] ifname  the interface name
 *
 * @return the VAP entry containing interface name and frequency on success; otherwise NULL
 */
static struct wlanifBSteerControlVapInfo *
wlanifBSteerControlInitVapFromIfname(wlanifBSteerControlHandle_t state, const char *ifname)
{
    struct wlanifBSteerControlVapInfo *vap = NULL;
    struct iwreq Wrq;
    wlanif_band_e band;

    memset(&Wrq, 0, sizeof(Wrq));
    strncpy(Wrq.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(state->controlSock, SIOCGIWFREQ, &Wrq) < 0) {
        dbgf(state->dbgModule, DBGERR, "%s: ioctl() failed, ifName: %s", __func__, ifname);
        return NULL;
    }

    band = wlanifMapFreqToBand(Wrq.u.freq.m);

    vap = wlanifBSteerControlAllocateVap(state, band);
    if (vap == NULL) {
        // Maximum number of VAPs reached on the given band
        return NULL;
    }
    strlcpy(vap->ifname, ifname, IFNAMSIZ + 1);
    vap->freq = Wrq.u.freq.m;

    if (!(vap->sysIndex = if_nametoindex(ifname))) {
        dbgf(state->dbgModule, DBGERR, "%s: Resolve index failed, ifName: %s",
             __func__, ifname);
        return NULL;
    }
    vap->valid = LBD_TRUE;

    return vap;
}

/**
 * @brief Resolve Wlan interfaces from configuration file and system.
 *
 * It will parse interface names from config file, then resolve
 * band and system index using ioctl() and if_nametoindex().
 *
 * @param [in] state  the "this" pointer
 *
 * @return LBD_OK if both bands are resolved; otherwise LBD_NOK
 */
static LBD_STATUS
wlanifBSteerControlResolveWlanIfaces(wlanifBSteerControlHandle_t state)
{
    char ifname[MAX_VAP_PER_BAND * wlanif_band_invalid][IFNAMSIZ + 1];
    u_int8_t i = 0;
    const char *wlanInterfaces;
    int numInterfaces;
    struct wlanifBSteerControlVapInfo *vap = NULL;

    wlanInterfaces = profileGetOpts(mdModuleID_WlanIF,
                                    WLANIFBSTEERCONTROL_WLAN_INTERFACES,
                                    NULL);

    if (!wlanInterfaces) {
        dbgf(state->dbgModule, DBGERR, "%s: No WLAN interface listed in config file", __func__);
        return LBD_NOK;
    }

    do {
        numInterfaces = splitByToken(wlanInterfaces, MAX_VAP_PER_BAND * wlanif_band_invalid, IFNAMSIZ + 1,
                                     (char *)ifname, ',');
        if (numInterfaces < wlanif_band_invalid) {
            dbgf(state->dbgModule, DBGERR, "%s: Failed to resolve WLAN interfaces from %s:"
                                           " at least one interface per band is required",
                 __func__, wlanInterfaces);
            break;
        }

        if ((state->controlSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            dbgf(state->dbgModule, DBGERR, "%s: Create ioctl socket failed", __func__);
            break;
        }

        if (fcntl(state->controlSock, F_SETFL, fcntl(state->controlSock, F_GETFL) | O_NONBLOCK)) {
            dbgf(state->dbgModule, DBGERR, "%s: fcntl() failed", __func__);
            break;
        }

        for (i = 0; i < numInterfaces; i++) {
            vap = wlanifBSteerControlInitVapFromIfname(state, ifname[i]);
            if (!vap) {
                break;
            }
        }

        if (!vap) {
            break;
        }

        if (wlanifBSteerControlIsBandValid(state, wlanif_band_24g) &&
            wlanifBSteerControlIsBandValid(state, wlanif_band_5g)) {
            free((char *)wlanInterfaces);
            return LBD_OK;
        }
    } while(0);

    if (state->controlSock > 0) {
        close(state->controlSock);
    }
    free((char *)wlanInterfaces);
    return LBD_NOK;
}

/**
 * @brief Read configuration parameters from config file and do sanity check
 *
 * If inactivity check period or MU check period from config file is invalid,
 * use the default one.
 *
 * @param [in] state  the "this" pointer
 * @param [in] band  the band to resolve configuration parameters
 *
 * @return LBD_NOK if the MU average period is shorter then MU sample period;
 *                 otherwise return LBD_OK
 */
static LBD_STATUS
wlanifBSteerControlReadConfig(wlanifBSteerControlHandle_t state, wlanif_band_e band)
{
    enum mdModuleID_e moduleID;
    struct profileElement *defaultProfiles;

    if (band == wlanif_band_24g) {
        moduleID = mdModuleID_WlanIF_Config_24G;
        defaultProfiles = wlanifElementDefaultTable_24G;
    } else {
        moduleID = mdModuleID_WlanIF_Config_5G;
        defaultProfiles = wlanifElementDefaultTable_5G;
    }

    state->bandInfo[band].configParams.inactivity_timeout_normal =
        profileGetOptsInt(moduleID,
                          WLANIFBSTEERCONTROL_INACT_IDLE_THRESHOLD,
                          defaultProfiles);

    state->bandInfo[band].configParams.inactivity_timeout_overload =
        profileGetOptsInt(moduleID,
                          WLANIFBSTEERCONTROL_INACT_OVERLOAD_THRESHOLD,
                          defaultProfiles);

    state->bandInfo[band].configParams.inactivity_check_period =
        profileGetOptsInt(moduleID,
                          WLANIFBSTEERCONTROL_INACT_CHECK_INTERVAL,
                          defaultProfiles);
    if (state->bandInfo[band].configParams.inactivity_check_period <= 0) {
        dbgf(state->dbgModule, DBGINFO, "[Band %u] Inactivity check period value is invalid (%d), use default one",
                band, state->bandInfo[band].configParams.inactivity_check_period);
        state->bandInfo[band].configParams.inactivity_check_period =
            atoi(profileElementDefault(WLANIFBSTEERCONTROL_INACT_CHECK_INTERVAL,
                                       defaultProfiles));
    }

    state->bandInfo[band].configParams.inactive_rssi_crossing_threshold =
        profileGetOptsInt(moduleID,
                          WLANIFBSTEERCONTROL_INACT_RSSI_XING_THRESHOLD,
                          defaultProfiles);

    state->bandInfo[band].configParams.low_rssi_crossing_threshold =
        profileGetOptsInt(moduleID,
                          WLANIFBSTEERCONTROL_LOW_RSSI_XING_THRESHOLD,
                          defaultProfiles);

    state->bandInfo[band].configParams.utilization_sample_period =
        profileGetOptsInt(moduleID,
                          WLANIFBSTEERCONTROL_MU_CHECK_INTERVAL,
                          defaultProfiles);
    // Sanity check to make sure unitlization check interval not zero
    if (state->bandInfo[band].configParams.utilization_sample_period <= 0) {
        dbgf(state->dbgModule, DBGINFO, "[Band %u] Utilization sample period value is invalid (%d), use default one",
                band, state->bandInfo[band].configParams.utilization_sample_period);
        state->bandInfo[band].configParams.utilization_sample_period =
            atoi(profileElementDefault(WLANIFBSTEERCONTROL_MU_CHECK_INTERVAL,
                                       defaultProfiles));
    }

    int muAvgPeriod = profileGetOptsInt(moduleID,
                                        WLANIFBSTEERCONTROL_MU_AVG_PERIOD,
                                        defaultProfiles);
    // Sanity check to make sure utilization average period is larger than sample interval
    if (muAvgPeriod <= state->bandInfo[band].configParams.utilization_sample_period) {
        dbgf(state->dbgModule, DBGINFO, "[Band %u] Utilization average period (%d seconds)is shorter than"
                                        " Utilization sample period (%d seconds).",
                band, muAvgPeriod, state->bandInfo[band].configParams.utilization_sample_period);
        return LBD_NOK;
    }
    state->bandInfo[band].configParams.utilization_average_num_samples = muAvgPeriod /
        state->bandInfo[band].configParams.utilization_sample_period;

    return LBD_OK;
}

/**
 * @brief Send IEEE80211_DBGREQ_BSTEERING_ENABLE request
 *
 * @param [in] state  the "this" pointer
 * @param [in] band  The band on which to send this request
 * @param [in] enable  LBD_TRUE for enable, LBD_FALSE for disable
 *
 * @return LBD_OK if the request is sent sucessfully; otherwise LBD_NOK
 */
static LBD_STATUS
wlanifBSteerControlSendEnable(wlanifBSteerControlHandle_t state,
                              wlanif_band_e band, LBD_BOOL enable)
{
    u_int8_t bsteering_enable;

    bsteering_enable = enable ? 1 : 0;

    return wlanifBSteerControlSend(state, band, IEEE80211_DBGREQ_BSTEERING_ENABLE, NULL,
                                   (void *) &bsteering_enable,
                                   sizeof(bsteering_enable));
}

/**
 * @brief Send IEEE80211_DBGREQ_BSTEERING_SET_PARAMS request
 *
 * @param [in] state  the "this" pointer
 * @param [in] band  The band on which to send this request
 *
 * @return LBD_OK if the request is sent sucessfully; otherwise LBD_NOK
 */
static LBD_STATUS
wlanifBSteerControlSendSetParams(wlanifBSteerControlHandle_t state, wlanif_band_e band)
{
    return wlanifBSteerControlSend(state, band, IEEE80211_DBGREQ_BSTEERING_SET_PARAMS, NULL,
                                   (void *) &state->bandInfo[band].configParams,
                                   sizeof(state->bandInfo[band].configParams));
}

/**
 * @brief Send IEEE80211_DBGREQ_BSTEERING_GET_RSSI request
 *
 * @param [in] state  the "this" pointer
 * @param [in] band  The band on which to send this request
 * @param [in] staAddr  the MAC address of the station to request RSSI
 * @param [in] numSamples  number of RSSI measurements to average before reporting back
 *
 * @return LBD_OK if the request is sent sucessfully; otherwise LBD_NOK
 */
static LBD_STATUS
wlanifBSteerControlSendRequestRSSI(wlanifBSteerControlHandle_t state, wlanif_band_e band,
                                   const struct ether_addr * staAddr, u_int8_t numSamples) {
    return wlanifBSteerControlSend(state, band, IEEE80211_DBGREQ_BSTEERING_GET_RSSI,
                                   staAddr, (void *) &numSamples, sizeof(numSamples));
}

/**
 * @brief Send request down to driver using ioctl()
 *
 * @param [in] band  the band on which this request should be sent
 * @param [in] cmd  the command contained in the request
 * @param [in] destAddr  optional parameters to specify the dest client of this request
 * @param [in] data  the data contained in the request
 * @param [in] data_len  the length of data contained in the request
 *
 * @return LBD_OK if the request is sent successfully; otherwise LBD_NOK
 */
static inline LBD_STATUS
wlanifBSteerControlSend(wlanifBSteerControlHandle_t state,
                        wlanif_band_e band, u_int8_t cmd,
                        const struct ether_addr *destAddr,
                        void *data, int data_len)
{
    const char *ifname = wlanifBSteerControlGetBandIfname(state, band);
    return wlanifBSteerControlSendVAP(state, ifname, cmd, destAddr, data,
                                      data_len);
}

/**
 * @brief Send request down to driver using ioctl()
 *
 * @param [in] ifname  the name of the interface on which this request should
 *                     be done
 * @param [in] cmd  the command contained in the request
 * @param [in] destAddr  optional parameters to specify the dest client of this request
 * @param [in] data  the data contained in the request
 * @param [in] data_len  the length of data contained in the request
 *
 * @return LBD_OK if the request is sent successfully; otherwise LBD_NOK
 */
static LBD_STATUS
wlanifBSteerControlSendVAP(wlanifBSteerControlHandle_t state,
                           const char *ifname, u_int8_t cmd,
                           const struct ether_addr *destAddr,
                           void *data, int data_len)
{
    struct iwreq iwr;
    struct ieee80211req_athdbg req;
    int result;

    memset(&req, 0, sizeof(struct ieee80211req_athdbg));
    (void) memset(&iwr, 0, sizeof(iwr));
    (void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));

    if (destAddr) {
        lbCopyMACAddr(destAddr->ether_addr_octet, req.dstmac);
    }

    req.cmd = cmd;
    memcpy(&req.data, data, data_len);

    iwr.u.data.pointer = (void *) &req;
    iwr.u.data.length = (sizeof(struct ieee80211req_athdbg));
    if ((result = ioctl(state->controlSock, IEEE80211_IOCTL_DBGREQ, &iwr)) < 0) {
        dbgf(state->dbgModule, DBGERR, "%s: Send %d request failed (errno=%d)",
             __func__, cmd, errno);
        return LBD_NOK;
    }

    return LBD_OK;
}

/**
 * @brief Dump the associated STAs for a single interface, identified by its
 *        name.
 *
 * @param [in] ifname  the name of the interface to dump
 * @param [in] band  the current band on which the interface is operating
 * @param [in] callback  the callback function to invoke for each associated
 *                       STA
 * @param [in] cookie  the parameter to provide back in the callback
 *
 * @return LBD_OK if the dump succeeded on this interface; otherwise LBD_NOK
 */
static LBD_STATUS wlanifBSteerControlDumpAssociatedSTAsOneIface(
        wlanifBSteerControlHandle_t state,
        const char *ifname, wlanif_band_e band,
        wlanif_associatedSTAsCB callback, void *cookie) {
#define LIST_STATION_ALLOC_SIZE 24*1024
    u_int8_t *buf = malloc(LIST_STATION_ALLOC_SIZE);
    if (!buf) {
        dbgf(state->dbgModule, DBGERR,
             "%s: Failed to allocate buffer for iface %s", __func__, ifname);
        return LBD_NOK;
    }

    int s = -1;
    LBD_STATUS result = LBD_OK;
    do {
        struct iwreq iwr;
        memset(&iwr, 0, sizeof(iwr));
        strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
        iwr.u.data.pointer = (void *) buf;
        iwr.u.data.length = LIST_STATION_ALLOC_SIZE;

        if (ioctl(state->controlSock, IEEE80211_IOCTL_STA_INFO, &iwr) < 0) {
            dbgf(state->dbgModule, DBGERR,
                 "%s: Failed to perform ioctl for iface %s", __func__, ifname);
            result = LBD_NOK;
            break;
        }

        // Loop over all of the STAs, providing a callback for each one.
        u_int8_t *currentPtr = buf;
        u_int8_t *endPtr = buf + iwr.u.data.length;
        while (currentPtr + sizeof(struct ieee80211req_sta_info) <= endPtr) {
            const struct ieee80211req_sta_info *staInfo =
                (const struct ieee80211req_sta_info *) currentPtr;
            struct ether_addr addr;
            lbCopyMACAddr(staInfo->isi_macaddr, addr.ether_addr_octet);
            callback(&addr, band, cookie);

            currentPtr += staInfo->isi_len;
        }
    } while (0);

    free(buf);
    close(s);
    return result;
}

/**
 * @brief Run an ioctl operation that takes a single MAC address on all
 *        interfaces that operate on the provided band.
 *
 * @param [in] state  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 * @param [in] ioctlReq  the operation to run
 * @param [in] band  the band on which to perform the operation
 * @param [in] staAddr  the MAC address of the STA
 * @param [in] ignoreErrors  if true, do not report any errors
 *
 * @return LBD_OK if the operation was successful on all VAPs for that band;
 *         otherwise LBD_NOK
 */
static LBD_STATUS wlanifBSteerControlPerformIoctlOnBandWithMAC(
        wlanifBSteerControlHandle_t state, int ioctlReq, wlanif_band_e band,
        const struct ether_addr *staAddr, LBD_BOOL ignoreErrors) {
    struct iwreq iwr;
    struct sockaddr addr;

    memset(&addr, 0, sizeof(addr));
    addr.sa_family = ARPHRD_ETHER;
    lbCopyMACAddr(staAddr->ether_addr_octet, addr.sa_data);

    memset(&iwr, 0, sizeof(iwr));

    // This parameter is small enough that it can fit in the name union
    // member.
    memcpy(iwr.u.name, &addr, sizeof(addr));

    size_t i;
    for (i = 0; i < MAX_VAP_PER_BAND; ++i) {
        if (state->bandInfo[band].vaps[i].valid) {
            strncpy(iwr.ifr_name, state->bandInfo[band].vaps[i].ifname,
                    IFNAMSIZ);

            if (ioctl(state->controlSock, ioctlReq, &iwr) < 0 &&
                !ignoreErrors) {
                dbgf(state->dbgModule, DBGERR,
                     "%s: ioctl 0x%04x failed with errno %u",
                     __func__, ioctlReq, errno);
                return LBD_NOK;
            }
        }
    }

    return LBD_OK;
}

/**
 * @brief Run the maccmd ioctl with the provided value.
 *
 * @param [in] state  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 * @param [in] cmd  the command to inject
 * @param [in] band  the band on which to perform the operation
 *
 * @return LBD_OK if the operation was successful on all VAPs for that band;
 *         otherwise LBD_NOK
 */
static LBD_STATUS wlanifBSteerControlPerformMacCmdOnBand(
        wlanifBSteerControlHandle_t state, int cmd, wlanif_band_e band) {
    struct iwreq iwr;
    int params[2] = { IEEE80211_PARAM_MACCMD, cmd };

    memset(&iwr, 0, sizeof(iwr));

    // This parameter is small enough that it can fit in the name union
    // member.
    memcpy(iwr.u.name, &params, sizeof(params));

    size_t i;
    for (i = 0; i < MAX_VAP_PER_BAND; ++i) {
        if (state->bandInfo[band].vaps[i].valid) {
            strncpy(iwr.ifr_name, state->bandInfo[band].vaps[i].ifname,
                    IFNAMSIZ);

            if (ioctl(state->controlSock, IEEE80211_IOCTL_SETPARAM, &iwr) < 0) {
                dbgf(state->dbgModule, DBGERR,
                     "%s: ioctl (cmd=%u) failed with errno %u",
                     __func__, cmd, errno);
                return LBD_NOK;
            }
        }
    }

    return LBD_OK;
}

/**
 * @brief Timeout handler that checks if the VAPs are ready and re-enables
 *        band steering if they are.
 *
 * @param [in] cookie  the state object for wlanifBSteerControl
 */
static void wlanifBSteerControlAreVAPsReadyTimeoutHandler(void *cookie)
{
    wlanifBSteerControlHandle_t state =
        (wlanifBSteerControlHandle_t) cookie;

    LBD_BOOL enabled = LBD_FALSE;
    if (wlanifBSteerControlEnableWhenReady(state, &enabled) == LBD_NOK)  {
        dbgf(state->dbgModule, DBGERR,
             "%s: Re-enabling on both bands failed", __func__);
        exit(1);
    }
}

/**
 * @brief Determine if the interface state indicates that it is up.
 *
 * @param [in] state  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 * @param [in] ifname  the name of the interface to check
 *
 * @return LBD_TRUE if the VAPs are ready; otherwise LBD_FALSE
 */
static LBD_BOOL wlanifBSteerControlIsLinkUp(
        wlanifBSteerControlHandle_t state, const char *ifname) {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));

    strlcpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(state->controlSock, SIOCGIFFLAGS, &ifr) < 0) {
        dbgf(state->dbgModule, DBGERR,
             "%s: Failed to get interface flags for %s",
             __func__, ifname);
        return LBD_FALSE;
    }

    return (ifr.ifr_flags & IFF_RUNNING) != 0;
}

/**
 * @brief Determine whether all VAPs are ready for band steering to be
 *        enabled.
 *
 * @pre state has already been checked for validity
 *
 * @param [in] state  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 *
 * @return LBD_TRUE if the VAPs are ready; otherwise LBD_FALSE
 */
static LBD_BOOL wlanifBSteerControlAreAllVAPsReady(
        wlanifBSteerControlHandle_t state) {
    struct iwreq iwr;
    memset(&iwr, 0, sizeof(iwr));

    size_t i, j;
    for (i = 0; i < wlanif_band_invalid; ++i) {
        for (j = 0; j < MAX_VAP_PER_BAND; ++j) {
            if (state->bandInfo[i].vaps[j].valid) {
                // First check that the link is up. It may not be if the
                // operator made the interface administratively down.
                if (!wlanifBSteerControlIsLinkUp(state,
                            state->bandInfo[i].vaps[j].ifname)) {
                    return LBD_FALSE;
                }

                // This may not be necessary, but it should be more efficient
                // than checking it first.
                state->bandInfo[i].vaps[j].ifaceUp = LBD_TRUE;

                strncpy(iwr.ifr_name, state->bandInfo[i].vaps[j].ifname,
                        IFNAMSIZ);

                // This parameter is small enough that it can fit in the name union
                // member.
                int param = IEEE80211_PARAM_GET_ACS;
                memcpy(iwr.u.name, &param, sizeof(param));

                int acsState = 0;
                if (ioctl(state->controlSock, IEEE80211_IOCTL_GETPARAM, &iwr) < 0) {
                    dbgf(state->dbgModule, DBGERR,
                         "%s: GET_ACS failed with errno %u",
                         __func__, errno);
                    return LBD_FALSE;
                }
                memcpy(&acsState, iwr.u.name, sizeof(acsState));

                if (acsState) {
                    dbgf(state->dbgModule, DBGINFO,
                         "%s: ACS scan in progress on %s",
                         __func__, iwr.ifr_name);
                    return LBD_FALSE;
                }

                param = IEEE80211_PARAM_GET_CAC;
                memcpy(iwr.u.name, &param, sizeof(param));

                int cacState = 0;
                if (ioctl(state->controlSock, IEEE80211_IOCTL_GETPARAM, &iwr) < 0) {
                    dbgf(state->dbgModule, DBGERR,
                         "%s: GET_CAC failed with errno %u",
                         __func__, errno);
                    return LBD_FALSE;
                }
                memcpy(&cacState, iwr.u.name, sizeof(cacState));

                if (cacState) {
                    dbgf(state->dbgModule, DBGINFO,
                         "%s: CAC in progress on %s",
                         __func__, iwr.ifr_name);
                    return LBD_FALSE;
                }
            }
        }
    }

    // Got this far, so all of them were ok.
    return LBD_TRUE;
}

// ====================================================================
// Package level functions
// ====================================================================

wlanifBSteerControlHandle_t
wlanifBSteerControlCreate(struct dbgModule *dbgModule)
{
    struct wlanifBSteerControlPriv_t *state =
        calloc(1, sizeof(struct wlanifBSteerControlPriv_t));
    if (!state) {
        dbgf(dbgModule, DBGERR, "%s: Failed to allocate state structure",
             __func__);
        return NULL;
    }

    state->dbgModule = dbgModule;
    state->controlSock = -1;

    /* Resovlve WLAN interfaces */
    if (wlanifBSteerControlResolveWlanIfaces(state) == LBD_NOK) {
        free(state);
        return NULL;
    }

    // Initialize waiting list for RSSI measurement to be empty
    size_t i;
    for (i = 0; i < wlanif_band_invalid; ++i) {
        list_set_head(&state->bandInfo[i].rssiWaitingList);
    }

    // Socket is open at this point, so if an error is encountered, we need
    // to make sure to close it so as not to leak it.
    do {
        if (wlanifBSteerControlInitializeACLs(state, wlanif_band_24g) == LBD_NOK ||
            wlanifBSteerControlInitializeACLs(state, wlanif_band_5g) == LBD_NOK) {
            break;
        }

        /* Get configuration parameters from configuration file. */
        if (wlanifBSteerControlReadConfig(state, wlanif_band_24g) == LBD_NOK ||
            wlanifBSteerControlReadConfig(state, wlanif_band_5g) == LBD_NOK) {
            break;
        }

        evloopTimeoutCreate(&state->vapReadyTimeout, "vapReadyTimeout",
                            wlanifBSteerControlAreVAPsReadyTimeoutHandler,
                            state);

        return state;
    } while (0);

    // This will tear down the ACLs, close the socket, and then deallocate
    // the state object.
    wlanifBSteerControlDestroy(state);
    return NULL;
}

LBD_STATUS wlanifBSteerControlEnableWhenReady(
        wlanifBSteerControlHandle_t state, LBD_BOOL *enabled) {
    // Sanity check
    if (!state || !enabled) {
        return LBD_NOK;
    }

    *enabled = LBD_FALSE;

    // Check whether all VAPs on both bands are ready, and only
    // then perform the enable.
    if (wlanifBSteerControlAreAllVAPsReady(state)) {
        if (wlanifBSteerControlSetEnable(state,
                                         wlanif_band_24g, LBD_TRUE) == LBD_NOK ||
            wlanifBSteerControlSetEnable(state,
                                         wlanif_band_5g, LBD_TRUE) == LBD_NOK) {
            dbgf(state->dbgModule, DBGERR,
                 "%s: Enabling on both bands failed", __func__);
            return LBD_NOK;
        }

        *enabled = LBD_TRUE;
        state->bandSteeringEnabled = LBD_TRUE;

        wlanif_bandSteeringStateEvent_t bandSteeringStateEvent;
        bandSteeringStateEvent.enabled = LBD_TRUE;

        mdCreateEvent(mdModuleID_WlanIF, mdEventPriority_High,
                      wlanif_event_band_steering_state,
                      &bandSteeringStateEvent, sizeof(bandSteeringStateEvent));

        return LBD_OK;
    } else {
        evloopTimeoutRegister(&state->vapReadyTimeout,
                              VAP_READY_CHECK_PERIOD,
                              0 /* us */);

        return LBD_OK;
    }
}

LBD_STATUS wlanifBSteerControlDisable(wlanifBSteerControlHandle_t state) {
    // Sanity check
    if (!state) {
        return LBD_NOK;
    }

    if (wlanifBSteerControlSetEnable(state, wlanif_band_24g,
                                     LBD_FALSE) == LBD_NOK ||
        wlanifBSteerControlSetEnable(state, wlanif_band_5g,
                                     LBD_FALSE) == LBD_NOK) {
        return LBD_NOK;
    }

    state->bandSteeringEnabled = LBD_FALSE;
    return LBD_OK;
}

LBD_STATUS wlanifBSteerControlDestroy(wlanifBSteerControlHandle_t state)
{
    if (state) {
        wlanifBSteerControlTeardownACLs(state, wlanif_band_24g);
        wlanifBSteerControlTeardownACLs(state, wlanif_band_5g);

        size_t i;
        for (i = 0; i < wlanif_band_invalid; ++i) {
            list_head_t *iter = state->bandInfo[i].rssiWaitingList.next;
            while (iter != &state->bandInfo[i].rssiWaitingList) {
                wlanifBSteerControlRSSIRequestEntry_t *curEntry =
                    list_entry(iter, wlanifBSteerControlRSSIRequestEntry_t, listChain);

                iter = iter->next;
                free(curEntry);
            }
        }

        close(state->controlSock);
        evloopTimeoutUnregister(&state->vapReadyTimeout);

        free(state);
    }

    return LBD_OK;
}

LBD_STATUS wlanifBSteerControlSetOverload(wlanifBSteerControlHandle_t state,
                                          wlanif_band_e band, LBD_BOOL overload)
{
    LBD_STATUS status;
    u_int8_t bsteering_overload;

    // Sanity check
    if (!state || band >= wlanif_band_invalid) {
        return LBD_NOK;
    }

    if (!state->bandInfo[band].enabled) {
        dbgf(state->dbgModule, DBGERR, "%s: Band Steering is not enabled on band %u",
             __func__, band);
        return LBD_NOK;
    }

    bsteering_overload = overload ? 1 : 0;

    status = wlanifBSteerControlSend(state, band, IEEE80211_DBGREQ_BSTEERING_SET_OVERLOAD, NULL,
                                     (void *) &bsteering_overload,
                                     sizeof(bsteering_overload));

    return status;
}

wlanif_band_e wlanifBSteerControlResolveBandFromSystemIndex(wlanifBSteerControlHandle_t state,
                                                            int index)
{
    wlanif_band_e band;
    int i;

    if (!state) {
         // Invalid control handle
        return wlanif_band_invalid;
    }

    for (band = wlanif_band_24g ; band < wlanif_band_invalid; ++band) {
        for (i = 0; i < MAX_VAP_PER_BAND; ++i) {
            if (state->bandInfo[band].vaps[i].valid &&
                state->bandInfo[band].vaps[i].sysIndex == index) {
                return band;
            }
        }
    }
    return wlanif_band_invalid;
}

void wlanifBSteerControlUpdateLinkState(wlanifBSteerControlHandle_t state,
                                        int sysIndex, LBD_BOOL ifaceUp,
                                        LBD_BOOL *changed) {
    if (state && changed) {
        wlanif_band_e band;
        int i;

        for (band = wlanif_band_24g ; band < wlanif_band_invalid; ++band) {
            for (i = 0; i < MAX_VAP_PER_BAND; ++i) {
                if (state->bandInfo[band].vaps[i].valid &&
                    state->bandInfo[band].vaps[i].sysIndex == sysIndex) {
                    if (state->bandInfo[band].vaps[i].ifaceUp != ifaceUp) {
                        *changed = LBD_TRUE;
                        state->bandInfo[band].vaps[i].ifaceUp = ifaceUp;
                    }
                    return;
                }
            }
        }
    }

    // Not found, invalid control handle, or invalid changed param. Do nothing.
}

LBD_STATUS wlanifBSteerControlDumpAssociatedSTAs(wlanifBSteerControlHandle_t state,
                                                 wlanif_associatedSTAsCB callback,
                                                 void *cookie) {
    size_t i;
    for (i = 0; i < wlanif_band_invalid; ++i) {
        size_t j;
        for (j = 0; j < MAX_VAP_PER_BAND; ++j) {
            if (state->bandInfo[i].vaps[j].valid) {
                if (wlanifBSteerControlDumpAssociatedSTAsOneIface(state,
                            state->bandInfo[i].vaps[j].ifname,
                            (wlanif_band_e) i, callback, cookie) != LBD_OK) {
                    return LBD_NOK;
                }
            }
        }
    }

    return LBD_OK;
}

LBD_STATUS wlanifBSteerControlRequestStaRSSI(wlanifBSteerControlHandle_t state,
                                             wlanif_band_e band,
                                             const struct ether_addr * staAddr,
                                             u_int8_t numSamples) {
    LBD_STATUS status = LBD_NOK;

    // Sanity check
    if (!state || band >= wlanif_band_invalid || !staAddr || !numSamples) {
        return status;
    }

    if (!state->bandInfo[band].enabled) {
        dbgf(state->dbgModule, DBGERR, "%s: Cannot request RSSI for STA "lbMACAddFmt(":")
                                       " since band Steering is not enabled on band %u.",
             __func__, lbMACAddData(staAddr->ether_addr_octet), band);
        return status;
    }

    wlanifBSteerControlRSSIRequestEntry_t *entry = calloc(1,
            sizeof(wlanifBSteerControlRSSIRequestEntry_t));
    if (!entry) {
        dbgf(state->dbgModule, DBGERR, "%s: Failed to allocate entry for "
                                       "STA "lbMACAddFmt(":")".",
             __func__, lbMACAddData(staAddr->ether_addr_octet));
        return status;
    }

    lbCopyMACAddr(staAddr, &entry->addr);
    entry->numSamples = numSamples;

    LBD_BOOL measurementBusy = !list_is_empty(&state->bandInfo[band].rssiWaitingList);

    if (measurementBusy) {
        // Wait for other RSSI measurement done
        dbgf(state->dbgModule, DBGDEBUG, "%s: RSSI measurement request for STA "
                                         lbMACAddFmt(":")" is queued.",
             __func__, lbMACAddData(staAddr->ether_addr_octet));
        status = LBD_OK;
    } else {
        status = wlanifBSteerControlSendRequestRSSI(state, band, staAddr, numSamples);
    }

    if (status == LBD_OK) {
        list_insert_entry(&entry->listChain, &state->bandInfo[band].rssiWaitingList);
    } else {
        free(entry);
    }

    return status;
}

LBD_STATUS wlanifBSteerControlSetProbeRespWHForSTA(
        wlanifBSteerControlHandle_t state, wlanif_band_e band,
        const struct ether_addr *staAddr, LBD_BOOL withholdProbes) {
    LBD_STATUS status = LBD_OK;
    u_int8_t bsteering_withhold;

    // Sanity check
    if (!state || !staAddr || band >= wlanif_band_invalid) {
        return LBD_NOK;
    }

    if (!state->bandInfo[band].enabled) {
        dbgf(state->dbgModule, DBGERR,
             "%s: Band Steering is not enabled on band %u", __func__, band);
        return LBD_NOK;
    }

    bsteering_withhold = withholdProbes ? 1 : 0;

    size_t i;
    for (i = 0; i < MAX_VAP_PER_BAND; ++i) {
        if (state->bandInfo[band].vaps[i].valid) {
            status |= wlanifBSteerControlSendVAP(
                    state, state->bandInfo[band].vaps[i].ifname,
                    IEEE80211_DBGREQ_BSTEERING_SET_PROBE_RESP_WH, staAddr,
                    (void *) &bsteering_withhold, sizeof(bsteering_withhold));
        }
    }

    return status;
}

/**
 * @brief Initialize the ACLs on the provided band (as being empty).
 *
 * @pre state and band are valid
 *
 * @param [in] state  the handle returned from wlanifBSteerControlCreate()
 *                    to use for the initilization
 * @param [in] band  the band on which to initialize
 *
 * @return LBD_OK if the ACLs were initialized; otherwise LBD_NOK
 */
static LBD_STATUS wlanifBSteerControlInitializeACLs(
        wlanifBSteerControlHandle_t state, wlanif_band_e band) {
    if (wlanifBSteerControlPerformMacCmdOnBand(
                state, IEEE80211_MACCMD_FLUSH, band) == LBD_OK &&
        wlanifBSteerControlPerformMacCmdOnBand(
            state, IEEE80211_MACCMD_POLICY_DENY, band) == LBD_OK) {
        return LBD_OK;
    }

    return LBD_NOK;
}

/**
 * @brief Clear the ACLs on the provided band.
 *
 * @pre state and band are valid
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use for the teardown
 * @param [in] band  the band on which to teardown
 *
 * @return LBD_OK if the ACLs were flushed; otherwise LBD_NOK
 */
static void wlanifBSteerControlFlushACLs(
        wlanifBSteerControlHandle_t state, wlanif_band_e band) {
    // Note that errors are ignored here, as we want to clean up all the
    // way regardless.
    wlanifBSteerControlPerformMacCmdOnBand(
        state, IEEE80211_MACCMD_FLUSH, band);
}

/**
 * @brief Clear and disable the ACLs on the provided band.
 *
 * @pre state and band are valid
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use for the teardown
 * @param [in] band  the band on which to teardown
 *
 * @return LBD_OK if the ACLs were torn down; otherwise LBD_NOK
 */
static void wlanifBSteerControlTeardownACLs(
        wlanifBSteerControlHandle_t state, wlanif_band_e band) {
    // Note that errors are ignored here, as we want to clean up all the
    // way regardless.
    wlanifBSteerControlPerformMacCmdOnBand(
        state, IEEE80211_MACCMD_FLUSH, band);
    wlanifBSteerControlPerformMacCmdOnBand(
        state, IEEE80211_MACCMD_POLICY_OPEN, band);
}

LBD_STATUS wlanifBSteerControlDisableBandForSTA(
        wlanifBSteerControlHandle_t state, wlanif_band_e band,
        const struct ether_addr *staAddr) {
    if (!state || band >= wlanif_band_invalid || !staAddr) {
        return LBD_NOK;
    }

    return wlanifBSteerControlPerformIoctlOnBandWithMAC(
            state, IEEE80211_IOCTL_ADDMAC, band, staAddr,
            LBD_FALSE /* ignoreErrors */);
}

LBD_STATUS wlanifBSteerControlEnableBandForSTA(
        wlanifBSteerControlHandle_t state, wlanif_band_e band,
        const struct ether_addr *staAddr) {
    if (!state || band >= wlanif_band_invalid || !staAddr) {
        return LBD_NOK;
    }

    return wlanifBSteerControlPerformIoctlOnBandWithMAC(
            state, IEEE80211_IOCTL_DELMAC, band, staAddr,
            LBD_FALSE /* ignoreErrors */);
}

LBD_STATUS wlanifBSteerControlDisassociateStaOnBand(
        wlanifBSteerControlHandle_t state, wlanif_band_e band,
        const struct ether_addr *staAddr) {
    if (!state || band >= wlanif_band_invalid || !staAddr) {
        return LBD_NOK;
    }

    return wlanifBSteerControlPerformIoctlOnBandWithMAC(
            state, IEEE80211_IOCTL_KICKMAC, band, staAddr,
            LBD_TRUE /* ignoreErrors */);
}

LBD_STATUS wlanifBSteerControlRestartChannelUtilizationMonitoring(
        wlanifBSteerControlHandle_t state) {
    // Sanity check
    if (!state) {
        return LBD_NOK;
    }

    // If band steering is not currently enabled, we do not want to try to
    // disable it, as otherwise we might get a false failure (since the driver
    // checks for a double disable).
    if (state->bandSteeringEnabled) {
        if (wlanifBSteerControlDisable(state) == LBD_NOK) {
            dbgf(state->dbgModule, DBGERR,
                 "%s: Temporarily disabling on both bands failed",
                 __func__);
            return LBD_NOK;
        }
    }

    // Flush the ACLs as we could be disabled for a while if the new channel
    // is a DFS one.
    wlanifBSteerControlFlushACLs(state, wlanif_band_24g);
    wlanifBSteerControlFlushACLs(state, wlanif_band_5g);

    LBD_BOOL enabled = LBD_FALSE;
    if (wlanifBSteerControlEnableWhenReady(state, &enabled) == LBD_NOK) {
        dbgf(state->dbgModule, DBGERR,
             "%s: Re-enabling on both bands failed", __func__);
        return LBD_NOK;
    }

    if (enabled) {
        dbgf(state->dbgModule, DBGINFO, "%s: Restart complete", __func__);
        evloopTimeoutUnregister(&state->vapReadyTimeout);
    }
    return LBD_OK;
}

void wlanifBSteerControlHandleRSSIMeasurement(
        wlanifBSteerControlHandle_t state, wlanif_band_e band,
        const struct ether_addr *staAddr) {
    // Sanity check
    if (!state || band >= wlanif_band_invalid || !staAddr) {
        return;
    }

    if (list_is_empty(&state->bandInfo[band].rssiWaitingList)) {
        dbgf(state->dbgModule, DBGERR, "%s: No RSSI measurement is pending (received one from "
                                       lbMACAddFmt(":")").",
             __func__, lbMACAddData(staAddr->ether_addr_octet));
        return;
    }

    wlanifBSteerControlRSSIRequestEntry_t *head =
        list_first_entry(&state->bandInfo[band].rssiWaitingList,
                         wlanifBSteerControlRSSIRequestEntry_t,
                         listChain);

    if (!lbAreEqualMACAddrs(head->addr.ether_addr_octet,
                            staAddr->ether_addr_octet)) {
        dbgf(state->dbgModule, DBGERR, "%s: Expecting RSSI measurement from "
                                       lbMACAddFmt(":")", received one from "
                                       lbMACAddFmt(":")".",
             __func__, lbMACAddData(head->addr.ether_addr_octet),
             lbMACAddData(staAddr->ether_addr_octet));
        return;
    }

    list_remove_entry(&head->listChain);
    free(head);

    if (list_is_empty(&state->bandInfo[band].rssiWaitingList)) {
        return;
    }

    list_head_t *iter = state->bandInfo[band].rssiWaitingList.next;
    while (iter != &state->bandInfo[band].rssiWaitingList) {
        wlanifBSteerControlRSSIRequestEntry_t *curEntry =
            list_entry(iter, wlanifBSteerControlRSSIRequestEntry_t, listChain);

        iter = iter->next;

        if (LBD_NOK == wlanifBSteerControlSendRequestRSSI(state, band, &curEntry->addr,
                                                          curEntry->numSamples)) {
            // If request RSSI fails, do not retry, rely on RSSI xing event to update RSSI
            dbgf(state->dbgModule, DBGERR, "%s: Failed to request RSSI measurement for "
                                           lbMACAddFmt(":")".",
                 __func__, lbMACAddData(curEntry->addr.ether_addr_octet));
            list_remove_entry(&curEntry->listChain);
            free(curEntry);
        } else {
            dbgf(state->dbgModule, DBGDEBUG, "%s: RSSI measurement request for STA "
                                         lbMACAddFmt(":")" is dequeued and sent.",
                 __func__, lbMACAddData(curEntry->addr.ether_addr_octet));
            break;
        }
    }
}

