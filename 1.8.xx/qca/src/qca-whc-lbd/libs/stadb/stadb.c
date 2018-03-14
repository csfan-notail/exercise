// vim: set et sw=4 sts=4 cindent:
/*
 * @File: stadb.c
 *
 * @Abstract: Implementation of station database public APIs
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
#include <netinet/ether.h>

#include <dbg.h>
#include <evloop.h>

#ifdef LBD_DBG_MENU
#include <cmd.h>
#endif

#include "internal.h"
#include "module.h"
#include "profile.h"
#include "wlanif.h"

#include "stadb.h"
#include "stadbHashTable.h"
#include "stadbEntryPrivate.h"

// For now, we are only permitting 3 observers, as it is likely that the
// following will need to observe RSSI updates:
//
// 1. Pre-association steering decision maker
// 2. Post-association steering decision maker
// 3. Steering safety mechanism
#define MAX_RSSI_OBSERVERS 3

/**
 * @brief Internal type used for station database iteration, tying the user
 *        requested cookie and function together with this files iteration
 *        function.
 */
typedef struct stadbIterateParams_t {
    /// The caller-provided callback function to invoke for each entry.
    stadb_iterFunc_t callback;

    /// The caller-provided cookie to provide in each invocation.
    void *cookie;
} stadbIterateParams_t;

static struct {
    struct {
        /// Whether out of network devices should be included in the station
        /// database or not.
        u_int32_t includeOutOfNetwork;

        /// Number of entries in the database that triggers aging.
        u_int32_t agingSizeThreshold;

        /// How often to perform aging, in seconds.
        u_int32_t agingFrequency;

        /// The maximum age of an out of network STA after which it should be
        /// removed during aging.
        u_int32_t outOfNetworkMaxAge;

        /// The maximum age of an out of network STA after which it should be
        /// removed during aging.
        u_int32_t inNetworkMaxAge;

        /// Number of seconds of the interval when two consecutive probe requests
        /// should be grouped
        u_int32_t probeMaxInterval;
    } config;

    stadbHashTableHandle_t db;

    /// Observer for all RSSI updates to entries in the database.
    struct stadbRSSIObserver {
        LBD_BOOL isValid;
        stadb_rssiObserverCB callback;
        void *cookie;
    } rssiObservers[MAX_RSSI_OBSERVERS];

    /// Observer for all association updates to entries in the database.
    struct stadbAssocObserver {
        LBD_BOOL isValid;
        stadb_assocObserverCB callback;
        void *cookie;
    } assocObserver;

    /// Observer for all activity status updates to entries in the database.
    struct stadbActivityObserver {
        LBD_BOOL isValid;
        stadb_activityObserverCB callback;
        void *cookie;
    } activityObserver;

    /// Observer for all entries in the database whose RSSI crossing low threshold
    struct stadbLowRSSIObserver {
        LBD_BOOL isValid;
        stadb_lowRSSIObserverCB callback;
        void *cookie;
    } lowRSSIObserver;

    /// Timer used to conduct periodic aging of the database.
    struct evloopTimeout agingTimeout;

    struct dbgModule *dbgModule;

#ifdef LBD_DBG_MENU
    /// When debug mode is enabled, the actChangeEvent/rssiXingEvent/rssiMeasurementEvent
    /// will be ignored and not override RSSI and activity status, and changes have to
    /// come via the debug CLI. But other events can still go through and potentially
    /// overwrite the RSSI value injected via the debug CLI.
    LBD_BOOL debugModeEnabled;
#endif /* LBD_DBG_MENU */
} stadbState;

/**
 * @brief Default configuration values.
 *
 * These are used if the config file does not specify them.
 */
static struct profileElement stadbElementDefaultTable[] = {
    { STADB_INCLUDE_OUT_OF_NETWORK_KEY, "1" },
    { STADB_AGING_SIZE_THRESHOLD_KEY,   "100" },
    { STADB_AGING_FREQUENCY_KEY,        "60" },
    { STADB_OUT_OF_NETWORK_MAX_AGE,     "300" },      // 5 minutes
    { STADB_IN_NETWORK_MAX_AGE,         "2592000" },  // 30 days
    { STADB_PROBE_MAX_INTERVAL,         "5" },        // 5 seconds
    { NULL, NULL }
};

// Forward decls
static void stadbReadConfig(void);
static stadbEntry_handle_t stadbFindOrInsertEntry(
        const struct ether_addr *addr, LBD_BOOL outOfNetwork);

static stadbEntry_handle_t stadbUpdateBandAndRSSI(
        const struct ether_addr *addr, wlanif_band_e band, u_int8_t rssi,
        stadb_rssiUpdateReason_e reason, LBD_BOOL markAssociated,
        LBD_BOOL outOfNetwork);
static void stadbUpdateAssoc(const struct ether_addr *addr,
                             wlanif_band_e band, LBD_BOOL isAssoc);
static void stadbNotifyRSSIObservers(stadbEntry_handle_t entry,
                                     stadb_rssiUpdateReason_e reason);
static void stadbNotifyAssocObserver(stadbEntry_handle_t entry);
static void stadbNotifyActivityObservers(stadbEntry_handle_t entry);
static void stadbNotifyLowRSSIObservers(stadbEntry_handle_t entry);

static void stadbHandleProbeReq(struct mdEventNode *event);
static void stadbHandleAuthRej(struct mdEventNode *event);
static void stadbHandleAssoc(struct mdEventNode *event);
static void stadbHandleActChangeEvent(struct mdEventNode *event);
static void stadbHandleActChange(const struct ether_addr *staAddr,
                                 wlanif_band_e band, LBD_BOOL active);
static void stadbHandleRSSIXingEvent(struct mdEventNode *event);
static void stadbHandleRSSIXing(const struct ether_addr *staAddr,
                                wlanif_band_e band, u_int8_t rssi,
                                wlanif_rssiXingDirection_e lowXing);
static void stadbHandleRSSIMeasurementEvent(struct mdEventNode *event);
static void stadbHandleRSSIMeasurement(const struct ether_addr *staAddr,
                                       wlanif_band_e band, u_int8_t rssi);

static void stadbIterateResetAssoc(stadbEntry_handle_t entry, void *cookie);
static void stadbHandleBandSteeringStateEvent(struct mdEventNode *event);

static void stadbAgingTimeoutHandler(void *cookie);

static void stadbDumpAssociatedSTAsCB(const struct ether_addr *addr,
                                      wlanif_band_e band, void *);

static LBD_BOOL stadbIterateCB(stadbHashTableHandle_t table,
                               stadbEntry_handle_t entry,
                               void *cookie);

static void stadbMenuInit(void);

#ifdef LBD_DBG_MENU
static LBD_STATUS convertCmdToRSSIXingDirection(struct cmdContext *context,
                                                const char* arg,
                                                wlanif_rssiXingDirection_e *direction);
#endif

// ====================================================================
// Public API
// ====================================================================

LBD_STATUS stadb_init(void) {
    stadbState.dbgModule = dbgModuleFind("stadb");
    stadbState.dbgModule->Level = DBGINFO;

    stadbReadConfig();

    stadbState.db = stadbHashTableCreate();
    if (!stadbState.db) {
        return LBD_NOK;
    }

    if (wlanif_dumpAssociatedSTAs(stadbDumpAssociatedSTAsCB,
                                  &stadbState) != LBD_OK) {
        return LBD_NOK;
    }

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_probereq,
                          stadbHandleProbeReq);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_authrej,
                          stadbHandleAuthRej);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_assoc,
                          stadbHandleAssoc);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_disassoc,
                          stadbHandleAssoc);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_act_change,
                          stadbHandleActChangeEvent);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_rssi_xing,
                          stadbHandleRSSIXingEvent);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_rssi_measurement,
                          stadbHandleRSSIMeasurementEvent);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_band_steering_state,
                          stadbHandleBandSteeringStateEvent);

    evloopTimeoutCreate(&stadbState.agingTimeout, "stadbAgingTimeout",
                        stadbAgingTimeoutHandler, NULL);

    stadbMenuInit();
    return LBD_OK;
}

LBD_STATUS stadb_registerRSSIObserver(stadb_rssiObserverCB callback,
                                      void *cookie) {
    if (!callback) {
        return LBD_NOK;
    }

    struct stadbRSSIObserver *freeSlot = NULL;
    size_t i;
    for (i = 0; i < MAX_RSSI_OBSERVERS; ++i) {
        struct stadbRSSIObserver *curSlot = &stadbState.rssiObservers[i];
        if (curSlot->isValid && curSlot->callback == callback &&
            curSlot->cookie == cookie) {
            dbgf(stadbState.dbgModule, DBGERR, "%s: Duplicate registration "
                                               "(func %p, cookie %p)",
                 __func__, callback, cookie);
           return LBD_NOK;
        }

        if (!freeSlot && !curSlot->isValid) {
            freeSlot = &stadbState.rssiObservers[i];
        }

    }

    if (freeSlot) {
        freeSlot->isValid = LBD_TRUE;
        freeSlot->callback = callback;
        freeSlot->cookie = cookie;
        return LBD_OK;
    }

    // No free entries found.
    return LBD_NOK;
}

LBD_STATUS stadb_registerActivityObserver(stadb_activityObserverCB callback,
                                          void *cookie) {
    if (!callback || stadbState.activityObserver.isValid) {
        return LBD_NOK;
    }

    stadbState.activityObserver.isValid = LBD_TRUE;
    stadbState.activityObserver.callback = callback;
    stadbState.activityObserver.cookie = cookie;

    return LBD_OK;
}

LBD_STATUS stadb_unregisterRSSIObserver(stadb_rssiObserverCB callback,
                                        void *cookie) {
    if (!callback) {
        return LBD_NOK;
    }

    size_t i;
    for (i = 0; i < MAX_RSSI_OBSERVERS; ++i) {
        if (stadbState.rssiObservers[i].isValid &&
            stadbState.rssiObservers[i].callback == callback &&
            stadbState.rssiObservers[i].cookie == cookie) {
            stadbState.rssiObservers[i].isValid = LBD_FALSE;
            stadbState.rssiObservers[i].callback = NULL;
            stadbState.rssiObservers[i].cookie = NULL;
            return LBD_OK;
        }
    }

    // No match found
    return LBD_NOK;
}

LBD_STATUS stadb_unregisterActivityObserver(stadb_activityObserverCB callback,
                                            void *cookie) {
    if (!callback || !stadbState.activityObserver.isValid ||
        stadbState.activityObserver.callback != callback ||
        stadbState.activityObserver.cookie != cookie) {
        return LBD_NOK;
    }

    stadbState.activityObserver.isValid = LBD_FALSE;
    stadbState.activityObserver.callback = NULL;
    stadbState.activityObserver.cookie = NULL;

    return LBD_OK;
}

stadbEntry_handle_t stadb_find(const struct ether_addr *addr) {
    if (addr) {
        return stadbHashTableFind(stadbState.db, addr);
    }

    // Address must be valid.
    return NULL;
}

LBD_STATUS stadb_registerAssocObserver(stadb_assocObserverCB callback,
                                       void *cookie) {
    if (!callback || stadbState.assocObserver.isValid) {
        return LBD_NOK;
    }

    stadbState.assocObserver.isValid = LBD_TRUE;
    stadbState.assocObserver.callback = callback;
    stadbState.assocObserver.cookie = cookie;

    return LBD_OK;
}

LBD_STATUS stadb_unregisterAssocObserver(stadb_assocObserverCB callback,
                                         void *cookie) {
    if (!callback || !stadbState.assocObserver.isValid ||
        stadbState.assocObserver.callback != callback ||
        stadbState.assocObserver.cookie != cookie) {
        return LBD_NOK;
    }

    stadbState.assocObserver.isValid = LBD_FALSE;
    stadbState.assocObserver.callback = NULL;
    stadbState.assocObserver.cookie = NULL;

    return LBD_OK;
}

LBD_STATUS stadb_registerLowRSSIObserver(stadb_lowRSSIObserverCB callback,
                                         void *cookie) {
    if (!callback || stadbState.lowRSSIObserver.isValid) {
        return LBD_NOK;
    }

    stadbState.lowRSSIObserver.isValid = LBD_TRUE;
    stadbState.lowRSSIObserver.callback = callback;
    stadbState.lowRSSIObserver.cookie = cookie;

    return LBD_OK;
}

LBD_STATUS stadb_unregisterLowRSSIObserver(stadb_lowRSSIObserverCB callback,
                                           void *cookie) {
    if (!callback || !stadbState.lowRSSIObserver.isValid ||
        stadbState.lowRSSIObserver.callback != callback ||
        stadbState.lowRSSIObserver.cookie != cookie) {
        return LBD_NOK;
    }

    stadbState.lowRSSIObserver.isValid = LBD_FALSE;
    stadbState.lowRSSIObserver.callback = NULL;
    stadbState.lowRSSIObserver.cookie = NULL;

    return LBD_OK;
}

LBD_STATUS stadb_iterate(stadb_iterFunc_t callback, void *cookie) {
    if (!callback) {
        return LBD_NOK;
    }

    stadbIterateParams_t params = { callback, cookie };
    return stadbHashTableIterate(stadbState.db, stadbIterateCB, &params);
}

LBD_STATUS stadb_fini(void) {
    stadbHashTableDestroy(stadbState.db);
    stadbState.db = NULL;

    return LBD_OK;
}

// ====================================================================
// Private helper functions
// ====================================================================

/**
 * @brief Read all of the configuration data from the file into the internal
 *        state.
 */
static void stadbReadConfig(void) {
    stadbState.config.includeOutOfNetwork =
        profileGetOptsInt(mdModuleID_StaDB, STADB_INCLUDE_OUT_OF_NETWORK_KEY,
                          stadbElementDefaultTable);

    stadbState.config.agingSizeThreshold =
        profileGetOptsInt(mdModuleID_StaDB, STADB_AGING_SIZE_THRESHOLD_KEY,
                          stadbElementDefaultTable);

    stadbState.config.agingFrequency =
        profileGetOptsInt(mdModuleID_StaDB, STADB_AGING_FREQUENCY_KEY,
                          stadbElementDefaultTable);

    stadbState.config.outOfNetworkMaxAge =
        profileGetOptsInt(mdModuleID_StaDB, STADB_OUT_OF_NETWORK_MAX_AGE,
                          stadbElementDefaultTable);

    stadbState.config.inNetworkMaxAge =
        profileGetOptsInt(mdModuleID_StaDB, STADB_IN_NETWORK_MAX_AGE,
                          stadbElementDefaultTable);

    stadbState.config.probeMaxInterval =
        profileGetOptsInt(mdModuleID_StaDB, STADB_PROBE_MAX_INTERVAL,
                          stadbElementDefaultTable);
}

/**
 * @brief Find the entry in the database with the matching MAC address,
 *        or allocate and insert a new entry if not found.
 *
 * @param [in] addr  the MAC address of the entry to find
 * @param [in] outOfNetwork  flag indicating whether the STA should be
 *                           considered out of network for this update;
 *                           if it is considered out of network, an entry
 *                           will only be created if storage of out of
 *                           network devices in the database is enabled
 *
 * @return the handle to the entry, or NULL if the entry did not exist and
 *         one could not be created
 */
static stadbEntry_handle_t stadbFindOrInsertEntry(
        const struct ether_addr *addr, LBD_BOOL outOfNetwork) {
    stadbEntry_handle_t entry =
        stadbHashTableFind(stadbState.db, addr);
    if (!entry && (!outOfNetwork || stadbState.config.includeOutOfNetwork)) {
        entry = stadbEntryCreate(addr);
        if (!entry) {
            dbgf(stadbState.dbgModule, DBGERR, "%s: Failed to allocate "
                                               "new entry; dropping",
                 __func__);
            return NULL;
        }

        stadbHashTableInsert(stadbState.db, entry);

        // If the size is now at the limit where aging is required, start up
        // the timer.
        if (stadbHashTableGetSize(stadbState.db) ==
                stadbState.config.agingSizeThreshold) {
            evloopTimeoutRegister(&stadbState.agingTimeout,
                                  stadbState.config.agingFrequency,
                                  0 /* us */);
        }
    }

    return entry;
}

/**
 * @brief Record in the database the band and RSSI on which a STA was active .
 *
 * @param [in] addr  the MAC address of the STA
 * @param [in] band  the band on which the STA sent a packet
 * @param [in] rssi  the signal strength of the received packet
 * @param [in] reason  the reason for the RSSI update
 * @param [in] markAssociated  flag indicating if this update implies that the STA
 *                             is associated on the given band
 * @param [in] outOfNetwork  whether this update should be considered as for
 *                           a potentially out of network device; note that
 *                           this only is relevant if storage of out of network
 *                           devices in the database is disabled
 *
 * @return the handle for the STA entry
 */
static stadbEntry_handle_t stadbUpdateBandAndRSSI(const struct ether_addr *addr,
                                                  wlanif_band_e band, u_int8_t rssi,
                                                  stadb_rssiUpdateReason_e reason,
                                                  LBD_BOOL markAssociated,
                                                  LBD_BOOL outOfNetwork) {
    // If the entry does not exist in the DB, create and add it (unless
    // storage of out of network devices is disabled).
    stadbEntry_handle_t entry =
        stadbFindOrInsertEntry(addr, outOfNetwork);
    if (entry) {
        if (markAssociated) {
            // Not update activity status based on RSSI report
            stadbEntryMarkAssociated(entry, band, LBD_TRUE, /* isAssociated */
                                     LBD_FALSE /* updateAct */,
                                     LBD_FALSE /* emitDiagLog */);
        }
        if (reason == stadb_rssiUpdateReason_probe) {
            stadbEntryRecordProbeRSSI(entry, band, rssi,
                                      stadbState.config.probeMaxInterval);
        } else {
            stadbEntryRecordRSSI(entry, band, rssi);
        }
        stadbNotifyRSSIObservers(entry, reason);
    }

    return entry;
}

/**
 * @brief Record in the database that the STA associated or disassociated on
 *        the specified band.
 *
 * @param [in] addr  the MAC address of the STA
 * @param [in] band  the band on which the STA associated/disassociated
 * @param [in] isAssoc  LBD_TRUE if it was an association; otherwise LBD_FALSE
 */
static void stadbUpdateAssoc(const struct ether_addr *addr,
                             wlanif_band_e band, LBD_BOOL isAssoc) {
    // Association events always result in the device being considered in
    // network. It is possible this will not be the case as we do not
    // necessarily know that the device successfully completed the security
    // handshake, but this is our best guess.
    stadbEntry_handle_t entry =
        stadbFindOrInsertEntry(addr, LBD_FALSE /* outOfNetwork */);
    if (entry) {
        // Update activity status when asociation status changes
        stadbEntryMarkAssociated(entry, band, isAssoc, LBD_TRUE /* updateAct */,
                                 LBD_TRUE /* emitDiaglog */);
        stadbNotifyAssocObserver(entry);
    }
}

/**
 * @brief Notify all registered oberservers that the provided entry was
 *        updated.
 *
 * @param [in] entry  the entry that was updated
 * @param [in] reason  the reason for the RSSI update
 */
static void stadbNotifyRSSIObservers(stadbEntry_handle_t entry,
                                     stadb_rssiUpdateReason_e reason) {
    size_t i;
    for (i = 0; i < MAX_RSSI_OBSERVERS; ++i) {
        if (stadbState.rssiObservers[i].isValid) {
            stadbState.rssiObservers[i].callback(
                    entry, reason, stadbState.rssiObservers[i].cookie);
        }
    }
}

/**
 * @brief Notify all registered oberservers that the provided entry's
 *        association information was updated.
 *
 * @param [in] entry  the entry that was updated
 */
static void stadbNotifyAssocObserver(stadbEntry_handle_t entry) {
    if (stadbState.assocObserver.isValid) {
        stadbState.assocObserver.callback(
            entry, stadbState.assocObserver.cookie);
    }
}

/**
 * @brief Notify all registered oberservers that the activity status of
 *        the provided entry was updated.
 *
 * @param [in] entry  the entry that was updated
 */
static void stadbNotifyActivityObservers(stadbEntry_handle_t entry) {
    if (stadbState.activityObserver.isValid) {
        stadbState.activityObserver.callback(
            entry, stadbState.activityObserver.cookie);
    }
}

/**
 * @brief Notify all registered oberservers that the RSSI of the provided entry
 *        crossed the low threshold.
 *
 * @param [in] entry  the entry that was updated
 */
static void stadbNotifyLowRSSIObservers(stadbEntry_handle_t entry) {
    if (stadbState.lowRSSIObserver.isValid) {
        stadbState.lowRSSIObserver.callback(
            entry, stadbState.lowRSSIObserver.cookie);
    }
}

/**
 * @brief React to an event indicating that a probe request was received.
 *
 * @param [in] event  the event received
 */
static void stadbHandleProbeReq(struct mdEventNode *event) {
    const wlanif_probeReqEvent_t *probeEvent =
        (const wlanif_probeReqEvent_t *) event->Data;

    stadbUpdateBandAndRSSI(&probeEvent->sta_addr, probeEvent->band,
                           probeEvent->rssi, stadb_rssiUpdateReason_probe,
                           LBD_FALSE /* markAssociated */,
                           LBD_TRUE /* outOfNetwork */);
}

/**
 * @brief React to an event indicating than an authentication reject message
 *        was sent to a STA that tried to authenticate on a blacklisted
 *        band.
 *
 * @param [in] event  the event received
 */
static void stadbHandleAuthRej(struct mdEventNode *event) {
    const wlanif_authRejEvent_t *authEvent =
        (const wlanif_authRejEvent_t *) event->Data;

    // An auth reject should indicate the device was considered in network
    // at some point, as otherwise its MAC address would not have been in
    // the ACLs.
    stadbUpdateBandAndRSSI(&authEvent->sta_addr, authEvent->band,
                           authEvent->rssi, stadb_rssiUpdateReason_authrej,
                           LBD_FALSE /* markAssociated */,
                           LBD_FALSE /* outOfNetwork */);
}

/**
 * @brief React to an event indicating a station associated or disassociated
 *        on a given band.
 *
 * @param [in] event  the event received
 */
static void stadbHandleAssoc(struct mdEventNode *event) {
    const wlanif_assocEvent_t *assocEvent =
        (const wlanif_assocEvent_t *) event->Data;

    stadbUpdateAssoc(&assocEvent->sta_addr, assocEvent->band,
                     event->EventID == wlanif_event_assoc);
}

/**
 * @brief React to an event indicating a station's activity status changed
 *        on a given band
 *
 * @param [in] event  the event received
 */
static void stadbHandleActChangeEvent(struct mdEventNode *event) {
    const wlanif_actChangeEvent_t *actChangeEvent =
        (const wlanif_actChangeEvent_t *) event->Data;

#ifdef LBD_DBG_MENU
    if (!stadbState.debugModeEnabled)
#endif
    {
        stadbHandleActChange(&actChangeEvent->sta_addr,
                             actChangeEvent->band,
                             actChangeEvent->active);
    }
}

/**
 * @brief Handle the activity status reported from activity change event
 *
 * @param [in] staAddr  the MAC address of the STA
 * @param [in] band  the band on which this event is received
 * @param [in] active  flag indicating the STA is active or not
 */
static void stadbHandleActChange(const struct ether_addr *staAddr,
                                 wlanif_band_e band, LBD_BOOL active) {
    stadbEntry_handle_t entry =
        stadbFindOrInsertEntry(staAddr, LBD_FALSE /* outOfNetwork */);
    if (entry) {
        stadbEntryMarkActive(entry, band, active);

        // Only notify the observers if the activity event occurred for
        // the currently associated band (which may have just been updated).
        if (stadbEntry_getAssociatedBand(entry, NULL) == band) {
            stadbNotifyActivityObservers(entry);
        }
    }
}

/**
 * @brief React to an event indicating the requested RSSI measurement is available
 *
 * @param [in] event  the event received
 */
static void stadbHandleRSSIMeasurementEvent(struct mdEventNode *event) {
    const wlanif_rssiMeasurementEvent_t *rssiMeasurementEvent =
        (const wlanif_rssiMeasurementEvent_t *) event->Data;

#ifdef LBD_DBG_MENU
    if (!stadbState.debugModeEnabled)
#endif
    {
        stadbHandleRSSIMeasurement(&rssiMeasurementEvent->sta_addr,
                                   rssiMeasurementEvent->band,
                                   rssiMeasurementEvent->rssi);
    }
}

/**
 * @brief Handle requested RSSI measurement result
 *
 * For valid RSSI measurement, update RSSI and band info.
 *
 * @param [in] staAddr  the MAC address of the STA
 * @param [in] band  the band on which this measurement is taken
 * @param [in] rssi  the RSSI measurement
 */
static void stadbHandleRSSIMeasurement(const struct ether_addr *staAddr,
                                       wlanif_band_e band, u_int8_t rssi) {

    if (rssi == LBD_INVALID_RSSI) {
        // Observers are not informed about INVALID_RSSI, as they are expected to
        // rely on other RSSI updates, e.g RSSI crossing event, probe request
        dbgf(stadbState.dbgModule, DBGERR,
             "%s: Invalid RSSI measurement for "lbMACAddFmt(":"),
             __func__, lbMACAddData(staAddr->ether_addr_octet));
        return;
    } else {
        stadbUpdateBandAndRSSI(staAddr, band, rssi,
                               stadb_rssiUpdateReason_measurement,
                               LBD_TRUE /* markAssociated */,
                               LBD_FALSE /* outOfNetwork */);
    }
}

/**
 * @brief React to an event indicating the RSSI measurement crossed thresholds
 *
 * @param [in] event  the event received
 */
static void stadbHandleRSSIXingEvent(struct mdEventNode *event) {
    const wlanif_rssiXingEvent_t *rssiXingEvent =
        (const wlanif_rssiXingEvent_t *) event->Data;

#ifdef LBD_DBG_MENU
    if (!stadbState.debugModeEnabled)
#endif
    {
        stadbHandleRSSIXing(&rssiXingEvent->sta_addr,
                           rssiXingEvent->band,
                           rssiXingEvent->rssi,
                           rssiXingEvent->lowRSSIXing);
    }
}

/**
 * @brief Handle the RSSI information from RSSI crossing event
 *
 * @param [in] staAddr  the MAC address of the STA
 * @param [in] band  the band on which this event is received
 * @param [in] rssi  the RSSI measurement
 */
static void stadbHandleRSSIXing(const struct ether_addr *staAddr,
                                wlanif_band_e band, u_int8_t rssi,
                                wlanif_rssiXingDirection_e lowXing) {
    stadbEntry_handle_t entry =
        stadbUpdateBandAndRSSI(staAddr, band, rssi,
                               stadb_rssiUpdateReason_crossing,
                               LBD_TRUE /* markAssociated */,
                               LBD_FALSE /* outOfNetwork */);

    if (entry && lowXing == wlanif_rssi_down) {
        stadbNotifyLowRSSIObservers(entry);
    }
}

/**
 * @brief Reset the association information for the entry in the database.
 *
 * If the entry is not associated, this does nothing.
 *
 * @param [in] entry  the entry to reset
 * @param [in] cookie  unused
 */
static void stadbIterateResetAssoc(stadbEntry_handle_t entry, void *cookie) {
    wlanif_band_e assocBand = stadbEntry_getAssociatedBand(entry, NULL);
    if (assocBand != wlanif_band_invalid) {
        stadbEntryMarkAssociated(entry, assocBand,
                                 LBD_FALSE /* isAssociated */,
                                 LBD_FALSE /* updateActive */,
                                 LBD_FALSE /* emitDiagLog */);
    }
}

/**
 * @brief React to a change in the band steering state.
 *
 * @param [in] event  the event carrying the band steering state
 */
static void stadbHandleBandSteeringStateEvent(struct mdEventNode *event) {
    const wlanif_bandSteeringStateEvent_t *stateEvent =
        (const wlanif_bandSteeringStateEvent_t *) event->Data;

    if (!stateEvent->enabled) {
        // For now we do not do anything on a disable (and in fact wlanif
        // does not even generate a disabled event).
        return;
    }

    // Iterate over hash table and mark all STAs as no longer
    // associated. Then dump the information to start afresh.
    if (stadb_iterate(stadbIterateResetAssoc, NULL) == LBD_NOK ||
        wlanif_dumpAssociatedSTAs(stadbDumpAssociatedSTAsCB,
                                  &stadbState) != LBD_OK) {
        dbgf(stadbState.dbgModule, DBGERR,
             "%s: Failed to re-populate stadb association info",
             __func__);
    } else {
        dbgf(stadbState.dbgModule, DBGINFO,
             "%s: stadb populated with currently associated STAs",
             __func__);
    }
}

/**
 * @brief Process a single associated STA entry in the dump of all currently
 *        associated STAs.
 *
 * @param [in] addr  the MAC address of the STA
 * @param [in] band  the band on which the STA is associated
 */
static void stadbDumpAssociatedSTAsCB(const struct ether_addr *addr,
                                      wlanif_band_e band, void *cookie) {
    stadbUpdateAssoc(addr, band, LBD_TRUE);
}

/**
 * @brief Process each STA entry in the database, checking if it should be
 *        aged out.
 *
 * @param [in] handle  the handle to the complete station database
 * @param [in] entry  the STA entry to examine
 * @param [in] cookie  the cookie provided for iteration (not used)
 *
 * @return LBD_TRUE if the entry should be deleted; otherwise LBD_FALSE
 */
static LBD_BOOL stadbCheckEntryAge(stadbHashTableHandle_t handle,
                                   stadbEntry_handle_t entry,
                                   void *cookie) {
    // This should never fail, but we are being defensive here.
    time_t ageSecs;
    if (stadbEntry_getAge(entry, &ageSecs) != LBD_OK) {
        return LBD_FALSE;
    }

    const char *deviceType;
    enum dbgLevel level;
    LBD_BOOL doRemove = LBD_FALSE;

    // For out-of-network entries, the only condition for removal is that
    // their age exceeds the threshold.
    //
    // In-network entries must not be currently associated (in addition to
    // their age exceeding the threshold).
    if (!stadbEntry_isInNetwork(entry) &&
        ageSecs >= stadbState.config.outOfNetworkMaxAge) {
        deviceType = "out-of-network";
        level = DBGDEBUG;
        doRemove = LBD_TRUE;
    } else if (stadbEntry_isInNetwork(entry) &&
               stadbEntry_getAssociatedBand(entry, NULL) ==
               wlanif_band_invalid &&
               ageSecs >= stadbState.config.inNetworkMaxAge) {
        deviceType = "in-network";
        level = DBGINFO;
        doRemove = LBD_TRUE;
    }

    if (doRemove) {
        const struct ether_addr *addr = stadbEntry_getAddr(entry);

        dbgf(stadbState.dbgModule, level,
             "%s: Removing %s device " lbMACAddFmt(":")
             " due to age", __func__, deviceType,
             lbMACAddData(addr->ether_addr_octet));
    }

    return doRemove;
}

/**
 * @brief React to the aging timeout by sweeping over the database, looking
 *        for STAs that should be aged out.
 *
 * @param [in] cookie  the cookie provided when the timeout was created
 */
static void stadbAgingTimeoutHandler(void *cookie) {
    stadbHashTableIterate(stadbState.db, stadbCheckEntryAge, cookie);

    if (stadbHashTableGetSize(stadbState.db) >=
            stadbState.config.agingSizeThreshold) {
        evloopTimeoutRegister(&stadbState.agingTimeout,
                              stadbState.config.agingFrequency,
                              0 /* us */);
    }
}

/**
 * @brief Process one entry in the database during iteration.
 *
 * @param [in] table  the table over which the iteration is happening
 * @param [in] cookie  the wrapped parameters from the stadb_iterate()
 *                     invocation
 *
 * @return LBD_FALSE always (to signal no entry should be deleted)
 */
static LBD_BOOL stadbIterateCB(stadbHashTableHandle_t table,
                               stadbEntry_handle_t entry,
                               void *cookie) {
    stadbIterateParams_t *iterateParams = (stadbIterateParams_t *) cookie;
    iterateParams->callback(entry, iterateParams->cookie);
    return LBD_FALSE;  // no deletes allowed through this function
}

#ifdef LBD_DBG_MENU

static const char *stadbMenuStatusHelp[] = {
    "s -- print station database contents",
    "Usage:",
    "\ts: display all nodes",
    "\ts in: display only in-network nodes",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void stadbMenuStatusHandler(struct cmdContext *context,
                            const char *cmd) {
    LBD_BOOL inNetworkOnly = LBD_FALSE;
    const char *arg = cmdWordFirst(cmd);

    if (arg && strncmp("in", arg, 2) == 0) {
        inNetworkOnly = LBD_TRUE;
    }

    stadbHashTablePrint(stadbState.db, context, inNetworkOnly);
}

static const char *stadbMenuDebugHelp[] = {
    "d -- enable/disable station database debug mode",
    "Usage:",
    "\td on: enable debug mode (ignore RSSI/Activity status update)",
    "\td off: disable debug mode (handling RSSI/Activity status update)",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void stadbMenuDebugHandler(struct cmdContext *context, const char *cmd) {
    LBD_BOOL isOn = LBD_FALSE;
    const char *arg = cmdWordFirst(cmd);

    if (!arg) {
        cmdf(context, "stadb 'd' command requires on/off argument\n");
        return;
    }

    if (cmdWordEq(arg, "on")) {
        isOn = LBD_TRUE;
    } else if (cmdWordEq(arg, "off")) {
        isOn = LBD_FALSE;
    } else {
        cmdf(context, "stadb 'd' command: invalid arg '%s'\n", arg);
        return;
    }

    dbgf(stadbState.dbgModule, DBGINFO, "%s: Setting debug mode to %u",
         __func__, isOn);
    stadbState.debugModeEnabled = isOn;
}

static const char *stadbMenuRSSIHelp[] = {
    "rssi -- inject an RSSI measurement",
    "Usage:",
    "\trssi <2|5> <mac_addr> <value> [low-xing <direction>]: inject RSSI <value> for the <mac_addr> station on the band\n"
    "\t                                                      Optionally inject RSSI crossing low threshold event",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void stadbMenuRSSIHandler(struct cmdContext *context, const char *cmd) {
    const char *arg = cmdWordFirst(cmd);

    if (!stadbState.debugModeEnabled) {
        cmdf(context, "stadb 'rssi' command not allowed unless debug mode "
                      "is enabled\n");
        return;
    }

    if (!arg || cmdWordLen(arg) != 1) {
        cmdf(context, "stadb 'rssi' command invalid band - "
                      "expected '2|5'\n");
        return;
    }

    char bandChar = arg[0];
    wlanif_band_e band = wlanif_band_invalid;
    switch (bandChar) {
        case '2':
            band = wlanif_band_24g;
            break;

        case '5':
            band = wlanif_band_5g;
            break;

        default:
            cmdf(context, "stadb 'rssi' command invalid band '%c'\n",
                 bandChar);
            return;
    }

    arg = cmdWordNext(arg);
    const struct ether_addr *staAddr = ether_aton(arg);
    if (!staAddr) {
        cmdf(context, "stadb 'rssi' command invalid MAC address: %s\n", arg);
        return;
    }

    arg = cmdWordNext(arg);
    if (!cmdWordDigits(arg)) {
        cmdf(context, "stadb 'rssi' command invalid rssi: '%s'\n",
             arg);
        return;
    }

    int rssi = atoi(arg);
    const int STADB_DBG_MAX_RSSI = 95;
    if (rssi > STADB_DBG_MAX_RSSI) {
        cmdf(context, "stadb 'rssi' command RSSI value must be "
                      "between %d and %d\n",
             LBD_INVALID_RSSI, STADB_DBG_MAX_RSSI);
        return;
    }

    arg = cmdWordNext(arg);
    if (!cmdWordEq(arg, "low-xing")) {
        // RSSI measurement event
        dbgf(stadbState.dbgModule, DBGINFO,
             "%s: Spoofing RSSI measurement: %s", __func__, cmd);
        stadbHandleRSSIMeasurement(staAddr, band, rssi);
    } else {
        // RSSI Xing event
        wlanif_rssiXingDirection_e low;
        arg = cmdWordNext(arg);
        if (LBD_NOK == convertCmdToRSSIXingDirection(context, arg, &low)) {
            return;
        }

        dbgf(stadbState.dbgModule, DBGINFO,
             "%s: Spoofing RSSI crossing: %s", __func__, cmd);
        stadbHandleRSSIXing(staAddr, band, rssi, low);
    }
}

/**
 * @brief Resolve RSSI xing direction from command
 *
 * @param [in] context  the output context
 * @param [in] cmd  the command in the debug CLI containing RSSI xing direction
 * @param [out] direction  the resolved direction
 *
 * @return LBD_OK if the direction is resolve properly; otherwise LBD_NOK
 */
static LBD_STATUS convertCmdToRSSIXingDirection(struct cmdContext *context,
                                                const char* arg,
                                                wlanif_rssiXingDirection_e *direction) {
    if(cmdWordLen(arg) != 1) {
        cmdf(context, "stadb 'rssi' command invalid xing direction: '%s': "
                      "expect 0 (unchanged) | 1 (up) | 2 (down)\n",
             arg);
            return LBD_NOK;
    }

    switch (arg[0]) {
        case '0':
            *direction = wlanif_rssi_unchanged;
            break;
        case '1':
            *direction = wlanif_rssi_up;
            break;
        case '2':
            *direction = wlanif_rssi_down;
            break;
        default:
            cmdf(context, "stadb 'rssi' command invalid xing direction: '%c': "
                          "expect 0 (unchanged) | 1 (up) | 2 (down)",
                 arg[0]);
            return LBD_NOK;
    }

    return LBD_OK;
}

static const char *stadbMenuActivityHelp[] = {
    "act -- inject an activity status",
    "Usage:",
    "\tact <2|5> <mac_addr> <1|0>: inject activity status for the <mac_addr> station on the band",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void stadbMenuActivityHandler(struct cmdContext *context, const char *cmd) {
    const char *arg = cmdWordFirst(cmd);

    if (!stadbState.debugModeEnabled) {
        cmdf(context, "stadb 'act' command not allowed unless debug mode "
                      "is enabled\n");
        return;
    }

    if (!arg || cmdWordLen(arg) != 1) {
        cmdf(context, "stadb 'act' command invalid band - "
                      "expected '2|5'\n");
        return;
    }

    char bandChar = arg[0];
    wlanif_band_e band = wlanif_band_invalid;
    switch (bandChar) {
        case '2':
            band = wlanif_band_24g;
            break;

        case '5':
            band = wlanif_band_5g;
            break;

        default:
            cmdf(context, "stadb 'act' command invalid band '%c'\n",
                 bandChar);
            return;
    }

    arg = cmdWordNext(arg);
    const struct ether_addr *staAddr = ether_aton(arg);
    if (!staAddr) {
        cmdf(context, "stadb 'act' command invalid MAC address: %s\n", arg);
        return;
    }

    arg = cmdWordNext(arg);
    if (cmdWordLen(arg) != 1) {
        cmdf(context, "stadb 'act' command invalid activity status - "
                      "expected '1|0'\n");
        return;
    }

    char actChar = arg[0];
    LBD_BOOL active;
    switch (actChar) {
        case '1':
            active = LBD_TRUE;
            break;

        case '0':
            active = LBD_FALSE;
            break;

        default:
            cmdf(context, "stadb 'act' command invalid activity status '%c'\n",
                actChar);
            return;
    }

    dbgf(stadbState.dbgModule, DBGINFO,
         "%s: Spoofing inactivity change: %s", __func__, cmd);
    stadbHandleActChange(staAddr, band, active);
}

static const struct cmdMenuItem stadbMenu[] = {
    CMD_MENU_STANDARD_STUFF(),
    { "s", stadbMenuStatusHandler, NULL, stadbMenuStatusHelp },
    { "d", stadbMenuDebugHandler, NULL, stadbMenuDebugHelp },
    { "rssi", stadbMenuRSSIHandler, NULL, stadbMenuRSSIHelp },
    { "act", stadbMenuActivityHandler, NULL, stadbMenuActivityHelp },
    CMD_MENU_END()
};

static const char *stadbMenuHelp[] = {
    "stadb -- Station Database",
    NULL
};

static const struct cmdMenuItem stadbMenuItem = {
    "stadb",
    cmdMenu,
    (struct cmdMenuItem *) stadbMenu,
    stadbMenuHelp
};

#endif /* LBD_DBG_MENU */

static void stadbMenuInit(void) {
#ifdef LBD_DBG_MENU
    cmdMainMenuAdd(&stadbMenuItem);
#endif /* LBD_DBG_MENU */
}
