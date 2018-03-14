// vim: set et sw=4 sts=4 cindent:
/*
 * @File: wlanifBSteerEvents.c
 *
 * @Abstract: Load balancing daemon band steering events
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


#include "wlanifBSteerEvents.h"

#include "wlanif.h"
#include "wlanifPrivate.h"
#include "internal.h"
#include "module.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <string.h>

#include <dbg.h>
#include <bufrd.h>

#include <diaglog.h>

#include <ieee80211_band_steering_api.h>

// forward decls
static void wlanifBSteerEventsRegister(struct dbgModule *dbgModule,
                                       wlanifBSteerEventsHandle_t state);
static LBD_STATUS wlanifBSteerEventsUnregister(
        wlanifBSteerEventsHandle_t handle);
static void wlanifBSteerEventsBufRdCB(void *cookie);

// ====================================================================
// Internal types
// ====================================================================

struct wlanifBSteerEventsPriv_t {
    int netlinkSocket;
    int eventsEnabled;
    struct bufrd readBuf;

    /// This debug module is for all events other than probe requests
    /// (which happen too frequently to be lumped together).
    struct dbgModule *dbgModule;

    /// This debug module is only for probe requests, which happen
    /// quite frequently and that we want to suppress in many cases.
    struct dbgModule *probeDbgModule;

    /// Control handle to callback with RSSI measurement
    wlanifBSteerControlHandle_t bsteerControlHandle;
};

// ====================================================================
// Package level functions
// ====================================================================

wlanifBSteerEventsHandle_t wlanifBSteerEventsCreate(struct dbgModule *dbgModule,
                                                    wlanifBSteerControlHandle_t controlHandle) {
    struct wlanifBSteerEventsPriv_t *state =
        calloc(1, sizeof(struct wlanifBSteerEventsPriv_t));
    if (!state) {
        dbgf(dbgModule, DBGERR, "%s: Failed to allocate state structure",
             __func__);
        return NULL;
    }

    state->bsteerControlHandle = controlHandle;
    wlanifBSteerEventsRegister(dbgModule, state);

    if (-1 == state->netlinkSocket) {
        free(state);
        state = NULL;
    } else {
        state->probeDbgModule = dbgModuleFind("probe");
    }

    return state;
}

LBD_STATUS wlanifBSteerEventsEnable(wlanifBSteerEventsHandle_t state) {
    struct sockaddr_nl destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.nl_family = AF_NETLINK;
    destAddr.nl_pid = 0;
    destAddr.nl_groups = 0;

    struct nlmsghdr hdr;
    hdr.nlmsg_len = NLMSG_SPACE(0);
    hdr.nlmsg_flags = 0;
    hdr.nlmsg_type = 0;
    hdr.nlmsg_pid = getpid();

    if (sendto(state->netlinkSocket, &hdr, hdr.nlmsg_len, 0,
               (const struct sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
        dbgf(state->dbgModule, DBGERR, "%s: Failed to send netlink trigger",
             __func__);
        return LBD_NOK;
    }

    // Remember that eventing was enabled in case we need to reenable it
    // after a fatal socket error.
    state->eventsEnabled = 1;
    return LBD_OK;
}

LBD_STATUS wlanifBSteerEventsDestroy(wlanifBSteerEventsHandle_t state) {
    LBD_STATUS result = LBD_OK;
    if (state) {
        result = wlanifBSteerEventsUnregister(state);
        free(state);
    }

    return result;
}

// ====================================================================
// Private helper functions
// ====================================================================

/**
 * @brief Create and bind the netlink socket for band steering events.
 *
 * @param [in] dbgModule  the handle to use for logging
 * @param [inout] state  the internal state for this instance; upon success,
 *                       the socket and debug module members will be
 *                       initialized
 */
static void wlanifBSteerEventsRegister(struct dbgModule *dbgModule,
                                       wlanifBSteerEventsHandle_t state) {
    state->netlinkSocket = socket(PF_NETLINK, SOCK_RAW,
                                  NETLINK_BAND_STEERING_EVENT);
    if (-1 == state->netlinkSocket) {
        dbgf(dbgModule, DBGERR, "%s: Netlink socket creation failed",
             __func__);
        return;
    }

    struct sockaddr_nl addr;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = 0;  // using unicast for now

    if (-1 == bind(state->netlinkSocket, (const struct sockaddr *) &addr,
                   sizeof(addr))) {
        dbgf(dbgModule, DBGERR, "%s: Failed to bind netlink socket",
             __func__);
        close(state->netlinkSocket);
        state->netlinkSocket = -1;
        return;
    }

    state->dbgModule = dbgModule;

    u_int32_t bufferSize = NLMSG_SPACE(sizeof(struct nlmsghdr) +
                                       sizeof(struct ath_netlink_bsteering_event));

    bufrdCreate(&state->readBuf, "wlanifBSteerEvents-rd",
                state->netlinkSocket, bufferSize,
                wlanifBSteerEventsBufRdCB, state);
}

/**
 * @brief Clean up the netlink socket and its registration.
 *
 * @param [in] state  the internal state for which the cleanup should occur
 *
 * @return LBD_OK if the socket was closed successfully and unregistered from
 *         the event loop; otherwise LBD_NOK
 */
static LBD_STATUS wlanifBSteerEventsUnregister(
        wlanifBSteerEventsHandle_t state) {
    LBD_STATUS result = LBD_OK;
    if (close(state->netlinkSocket) != 0) {
        dbgf(state->dbgModule, DBGERR, "%s: Socket close failed",
             __func__);
        result = LBD_NOK;
    }

    state->netlinkSocket = -1;
    state->eventsEnabled = 0;

    // We will always have registered the socket if the state is valid.
    bufrdDestroy(&state->readBuf);
    return result;
}

/**
 * @brief React to an indication from the driver that a probe request was
 *        received from a specific client.
 *
 * @param [in] state  the "this" pointer
 * @param [in] event  the data sent from the kernel
 */
static void wlanifBSteerEventsHandleProbeReqInd(
        wlanifBSteerEventsHandle_t state,
        const ath_netlink_bsteering_event_t *event) {
    dbgf(state->probeDbgModule, DBGDUMP,
         "%s: Probe request from " lbMACAddFmt(":")
         ": RSSI %2u BandIndex %u",
         __func__, lbMACAddData(event->data.bs_probe.sender_addr),
         event->data.bs_probe.rssi, event->band_index);

    wlanif_probeReqEvent_t probeEvent;
    lbCopyMACAddr(event->data.bs_probe.sender_addr,
                  probeEvent.sta_addr.ether_addr_octet);

    probeEvent.band = wlanifMapToBand(state->dbgModule, event->band_index);
    probeEvent.rssi = event->data.bs_probe.rssi;

    mdCreateEvent(mdModuleID_WlanIF, mdEventPriority_High,
                  wlanif_event_probereq, &probeEvent, sizeof(probeEvent));
}

/**
 * @brief React to an indication from the driver that a specific client was
 *        authorized on the provided band.
 *
 * @param [in] state  the "this" pointer
 * @param [in] event  the data sent from the kernel
 */
static void wlanifBSteerEventsHandleNodeAuthorizedInd(
        wlanifBSteerEventsHandle_t state,
        const ath_netlink_bsteering_event_t *event) {
    dbgf(state->dbgModule, DBGINFO,
         "%s: Node " lbMACAddFmt(":") " authorized on BandIndex %u",
         __func__, lbMACAddData(event->data.bs_node_authorized.client_addr),
         event->band_index);

    wlanif_assocEvent_t assocEvent;
    lbCopyMACAddr(event->data.bs_node_authorized.client_addr,
                  assocEvent.sta_addr.ether_addr_octet);
    assocEvent.band = wlanifMapToBand(state->dbgModule, event->band_index);

    mdCreateEvent(mdModuleID_WlanIF, mdEventPriority_Low,
                  wlanif_event_assoc, &assocEvent, sizeof(assocEvent));
}

/**
 * @brief React to an indication from the driver that it sent an
 *        authentication message with a failure code due to an ACL match.
 *
 * @param [in] state  the "this" pointer
 * @param [in] event  the data sent from the kernel
 */
static void wlanifBSteerEventsHandleTxAuthFailInd(
        wlanifBSteerEventsHandle_t state,
        const ath_netlink_bsteering_event_t *event) {
    dbgf(state->dbgModule, DBGDEBUG,
         "%s: Tx'ed Auth reject to " lbMACAddFmt(":")
         ": RSSI %2u BandIndex %u",
         __func__, lbMACAddData(event->data.bs_auth.client_addr),
         event->data.bs_auth.rssi, event->band_index);

    wlanif_authRejEvent_t authEvent;
    lbCopyMACAddr(event->data.bs_auth.client_addr,
                  authEvent.sta_addr.ether_addr_octet);

    authEvent.band = wlanifMapToBand(state->dbgModule, event->band_index);
    authEvent.rssi = event->data.bs_auth.rssi;

    mdCreateEvent(mdModuleID_WlanIF, mdEventPriority_High,
                  wlanif_event_authrej, &authEvent, sizeof(authEvent));
}

/**
 * @brief React to an indication from the driver that a specific client's
 *        activity status has changed.
 *
 * @param [in] state  the "this" pointer
 * @param [in] event  the data sent from the kernel
 */
static void wlanifBSteerEventsHandleActivityChange(
        wlanifBSteerEventsHandle_t state,
        const ath_netlink_bsteering_event_t *event) {
    dbgf(state->dbgModule, DBGDEBUG,
         "%s: " lbMACAddFmt(":") " activity status changes to %s BandIndex %u",
         __func__, lbMACAddData(event->data.bs_activity_change.client_addr),
         event->data.bs_activity_change.activity ? "ACTIVE" : "INACTIVE",
         event->band_index);

    wlanif_actChangeEvent_t actChangeEvent;
    lbCopyMACAddr(event->data.bs_activity_change.client_addr,
                  actChangeEvent.sta_addr.ether_addr_octet);

    actChangeEvent.band = wlanifMapToBand(state->dbgModule, event->band_index);
    actChangeEvent.active = event->data.bs_activity_change.activity ? LBD_TRUE : LBD_FALSE;

    // Transition to active is high priority event since we would want to cancel
    // any steering that is in process. Transition to inactive is not high priority.
    mdCreateEvent(mdModuleID_WlanIF,
                  actChangeEvent.active ? mdEventPriority_High : mdEventPriority_Low,
                  wlanif_event_act_change, &actChangeEvent, sizeof(actChangeEvent));
}

/**
 * @brief React to an indication from the driver that a specific client's
 *        RSSI measurement has crossed threshold(s).
 *
 * @param [in] state  the "this" pointer
 * @param [in] event  the data sent from the kernel
 */
static void wlanifBSteerEventsHandleRSSIXingInd(
        wlanifBSteerEventsHandle_t state,
        const ath_netlink_bsteering_event_t *event) {
    dbgf(state->dbgModule, DBGDEBUG,
         "%s: " lbMACAddFmt(":") " RSSI measurement %u on BandIndex %u; "
         "Inactivity threshold xing: %u; Low threshold xing: %u",
         __func__, lbMACAddData(event->data.bs_rssi_xing.client_addr),
         event->data.bs_rssi_xing.rssi, event->band_index,
         event->data.bs_rssi_xing.inact_rssi_xing,
         event->data.bs_rssi_xing.low_rssi_xing);

    wlanif_rssiXingEvent_t rssiXingEvent;
    lbCopyMACAddr(event->data.bs_rssi_xing.client_addr,
                  rssiXingEvent.sta_addr.ether_addr_octet);

    rssiXingEvent.band = wlanifMapToBand(state->dbgModule, event->band_index);
    rssiXingEvent.rssi = event->data.bs_rssi_xing.rssi;
    rssiXingEvent.inactRSSIXing = wlanifMapToRSSIXingDirection(state->dbgModule, event->data.bs_rssi_xing.inact_rssi_xing);
    rssiXingEvent.lowRSSIXing = wlanifMapToRSSIXingDirection(state->dbgModule, event->data.bs_rssi_xing.low_rssi_xing);

    mdCreateEvent(mdModuleID_WlanIF, mdEventPriority_Low,
                  wlanif_event_rssi_xing, &rssiXingEvent, sizeof(rssiXingEvent));
}

/**
 * @brief React to an indication from the driver that a specific client's
 *        requested RSSI measurement is available.
 *
 * @param [in] state  the "this" pointer
 * @param [in] event  the data sent from the kernel
 * @param [in] isRawMeasurement  LBD_TRUE if the measurement is a raw one
 *                               (only suited for logging); otherwise LBD_FALSE
 */
static void wlanifBSteerEventsHandleRSSIMeasurementInd(
        wlanifBSteerEventsHandle_t state,
        const ath_netlink_bsteering_event_t *event,
        LBD_BOOL isRawMeasurement) {
    dbgf(state->dbgModule, isRawMeasurement ? DBGDUMP : DBGDEBUG,
         "%s: " lbMACAddFmt(":") " RSSI measurement %u on BandIndex %u",
         __func__, lbMACAddData(event->data.bs_rssi_measurement.client_addr),
         event->data.bs_rssi_measurement.rssi, event->band_index);

    wlanif_rssiMeasurementEvent_t rssiMeasurementEvent;
    lbCopyMACAddr(event->data.bs_rssi_measurement.client_addr,
                  rssiMeasurementEvent.sta_addr.ether_addr_octet);
    rssiMeasurementEvent.band = wlanifMapToBand(state->dbgModule, event->band_index);
    rssiMeasurementEvent.rssi = wlanifMapToRSSIMeasurement(event->data.bs_rssi_measurement.rssi);

    if (isRawMeasurement) {
        if (diaglog_startEntry(mdModuleID_WlanIF, wlanif_msgId_rawRSSI,
                               diaglog_level_debug)) {
            diaglog_writeMAC(&rssiMeasurementEvent.sta_addr);
            diaglog_write8(rssiMeasurementEvent.band);
            diaglog_write8(rssiMeasurementEvent.rssi);
            diaglog_finishEntry();
        }
        return;
    }

    wlanifBSteerControlHandleRSSIMeasurement(
            state->bsteerControlHandle,
            rssiMeasurementEvent.band,
            &rssiMeasurementEvent.sta_addr);

    mdCreateEvent(mdModuleID_WlanIF, mdEventPriority_High,
                  wlanif_event_rssi_measurement, &rssiMeasurementEvent,
                  sizeof(rssiMeasurementEvent));

}

/**
 * @brief React to an indication from the driver that the channel utilization
 *        has been measured.
 *
 * @param [in] state  the "this" pointer
 * @param [in] event  the data sent from the kernel
 * @param [in] isRawMeasurement  LBD_TRUE if the measurement is a raw one
 *                               (only suited for logging); otherwise LBD_FALSE
 */
static void wlanifBSteerEventsHandleChanUtilInd(
        wlanifBSteerEventsHandle_t state,
        const ath_netlink_bsteering_event_t *event,
        LBD_BOOL isRawMeasurement) {
    enum dbgLevel level = isRawMeasurement ? DBGDUMP : DBGDEBUG;
    dbgf(state->dbgModule, level, "%s: Channel utilization %2u%% on band %u",
         __func__, event->data.bs_chan_util.utilization, event->band_index);

    wlanif_chanUtilEvent_t chanUtilEvent;
    chanUtilEvent.band = wlanifMapToBand(state->dbgModule, event->band_index);
    chanUtilEvent.utilization = event->data.bs_chan_util.utilization;

    if (!isRawMeasurement) {
        // For now we leave the utilization as a low priority event, as at most
        // it will trigger a switch to overload mode and institute
        // pre-association steering. Since it takes many minutes before we get
        // the utilization measurements (using the default settings), it does
        // not matter if it takes a bit longer.
        mdCreateEvent(mdModuleID_WlanIF, mdEventPriority_Low,
                      wlanif_event_chan_util, &chanUtilEvent,
                      sizeof(chanUtilEvent));
    }

    if (isRawMeasurement &&
        diaglog_startEntry(mdModuleID_WlanIF, wlanif_msgId_rawChanUtilization,
                           diaglog_level_debug)) {
        diaglog_write8(chanUtilEvent.band);
        diaglog_write8(chanUtilEvent.utilization);
        diaglog_finishEntry();
    }
}

/**
 * @brief Handle a band steering event received from the kernel.
 *
 * @param [in] state  the "this" pointer
 * @param [in] event  the message to handle
 */
static void wlanifBSteerEventsMsgRx(wlanifBSteerEventsHandle_t state,
                                    const ath_netlink_bsteering_event_t *event) {
    switch (event->type) {
        case ATH_EVENT_BSTEERING_CHAN_UTIL:
            wlanifBSteerEventsHandleChanUtilInd(
                    state, event, LBD_FALSE /* isRawMeasurement */);
            break;
        case ATH_EVENT_BSTEERING_PROBE_REQ:
            wlanifBSteerEventsHandleProbeReqInd(state, event);
            break;

        case ATH_EVENT_BSTEERING_NODE_AUTHORIZED:
            wlanifBSteerEventsHandleNodeAuthorizedInd(state, event);
            break;

        case ATH_EVENT_BSTEERING_CLIENT_ACTIVITY_CHANGE:
            wlanifBSteerEventsHandleActivityChange(state, event);
            break;

        case ATH_EVENT_BSTEERING_TX_AUTH_FAIL:
            wlanifBSteerEventsHandleTxAuthFailInd(state, event);
            break;

        case ATH_EVENT_BSTEERING_DBG_CHAN_UTIL:
            wlanifBSteerEventsHandleChanUtilInd(
                    state, event, LBD_TRUE /* isRawMeasurement */);
            break;

        case ATH_EVENT_BSTEERING_CLIENT_RSSI_CROSSING:
            wlanifBSteerEventsHandleRSSIXingInd(state, event);
            break;

        case ATH_EVENT_BSTEERING_CLIENT_RSSI_MEASUREMENT:
            wlanifBSteerEventsHandleRSSIMeasurementInd(
                    state, event, LBD_FALSE /* isRawMeasurement*/);
            break;

        case ATH_EVENT_BSTEERING_DBG_RSSI:
            wlanifBSteerEventsHandleRSSIMeasurementInd(
                    state, event, LBD_TRUE /* isRawMeasurement*/);
            break;

        default:
            dbgf(state->dbgModule, DBGINFO, "%s: Unhandled msg: type %u band %u",
                 __func__, event->type, event->band_index);
            break;
    }
}

/**
 * @brief React to the indication that the netlink socket is readable.
 *
 * @param [in] cookie  the "this" pointer provided during registration
 */
static void wlanifBSteerEventsBufRdCB(void *cookie) {
    u_int32_t numBytes;
    const u_int8_t *msg;

    wlanifBSteerEventsHandle_t state = (wlanifBSteerEventsHandle_t) cookie;

    numBytes = bufrdNBytesGet(&state->readBuf);
    msg = bufrdBufGet(&state->readBuf);

    do {
        if (bufrdErrorGet(&state->readBuf)) {
            dbgf(state->dbgModule, DBGERR, "%s: Read error! # bytes=%u",
                 __func__, numBytes);

            int eventsEnabled = state->eventsEnabled;
            wlanifBSteerEventsUnregister(state);
            wlanifBSteerEventsRegister(state->dbgModule, state);

            if (-1 == state->netlinkSocket ||
                (eventsEnabled &&
                 wlanifBSteerEventsEnable(state) == LBD_NOK)) {
                dbgf(state->dbgModule, DBGERR,
                     "%s: Failed to recover from fatal error", __func__);
                exit(1);
            }

            return;
        }

        // bufrd will keep calling us back until no more progress is made.
        // This includes when there is no more data to be read, so we need
        // to bail out here to avoid the error below.
        if (!numBytes) {
            return;
        }

        const struct nlmsghdr *hdr = (const struct nlmsghdr *) msg;
        if (numBytes < sizeof(struct nlmsghdr) +
                       sizeof(ath_netlink_bsteering_event_t) ||
            hdr->nlmsg_len < sizeof(ath_netlink_bsteering_event_t)) {
            dbgf(state->dbgModule, DBGERR, "%s: Invalid message len: %u bytes",
                 __func__, numBytes);
            break;
        }

        const ath_netlink_bsteering_event_t *event = NLMSG_DATA(hdr);
        wlanifBSteerEventsMsgRx(state, event);
    } while (0);

    bufrdConsume(&state->readBuf, numBytes);
}
