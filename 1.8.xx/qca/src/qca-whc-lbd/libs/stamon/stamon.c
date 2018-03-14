// vim: set et sw=4 sts=4 cindent:
/*
 * @File: stamon.c
 *
 * @Abstract: Implementation of station monitor public APIs
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
#include <assert.h>

#include <dbg.h>

#include "internal.h"
#include "module.h"
#include "profile.h"
#include "stadb.h"
#include "steerexec.h"
#include "bandmon.h"

#include "stamon.h"

// Forward decls
static void stamonActivityObserver(stadbEntry_handle_t entry, void *cookie);
static void stamonRSSIObserver(stadbEntry_handle_t entry,
                               stadb_rssiUpdateReason_e reason, void *cookie);
static LBD_BOOL stamonCheckRSSI(wlanif_band_e band, LBD_BOOL estimate, int rssi);
static void stamonHandleSTABecomeActive(stadbEntry_handle_t entry);
static void stamonHandleSTABecomeInactive(stadbEntry_handle_t entry);
static void stamonMakeSteerDecision(stadbEntry_handle_t entry, LBD_BOOL final);
static wlanif_band_e stamonMakeSteerDecisionBasedOnRSSI(stadbEntry_handle_t entry,
    wlanif_band_e associatedBand, wlanif_band_e targetBand, LBD_BOOL final);
static void stamonHandleOverloadChange(struct mdEventNode *event);
static void stamonStaDBIterateCB(stadbEntry_handle_t entry, void *cookie);
static void stamonDoSteering(stadbEntry_handle_t entry, wlanif_band_e targetBand);

/**
 * @brief Internal state for the band monitor module.
 */
static struct {
    struct dbgModule *dbgModule;

    /// Configuration data obtained at init time
    struct {
        /// The number of inst RSSI measurements per-band
        u_int8_t instRSSINumSamples[wlanif_band_invalid];

        /// RSSI threshold when inactive
        u_int8_t inactRSSIXingThreshold[wlanif_band_invalid];

        /// RSSI difference when estimate RSSI on 5 GHz from
        /// the one measured on 2.4 GHz
        int rssiDiffEstimate5gFrom24g;

        /// RSSI difference when estimate RSSI on 2.4 GHz from
        /// the one measured on 5 GHz
        int rssiDiffEstimate24gFrom5g;

        /// Number of seconds allowed for an RSSI measurement to
        /// be considered as recent
        u_int8_t rssiFreshnessLimit;

        /// Number of probes required when non-associted band RSSI is valid
        u_int8_t probeCountThreshold;
    } config;
} stamonState;

/**
 * @brief Default configuration values.
 *
 * These are used if the config file does not specify them.
 */
static struct profileElement stamonElementDefaultTable[] = {
    { STAMON_RSSI_MEASUREMENT_NUM_SAMPLES_W2_KEY, "5" },
    { STAMON_RSSI_MEASUREMENT_NUM_SAMPLES_W5_KEY, "5" },
    { STAMON_INACT_RSSI_THRESHOLD_W2_KEY,         "20" },
    { STAMON_INACT_RSSI_THRESHOLD_W5_KEY,         "30" },
    { STAMON_RSSI_DIFF_EST_W5_FROM_W2_KEY,       "-20" },
    { STAMON_RSSI_DIFF_EST_W2_FROM_W5_KEY,        "10" },
    { STAMON_RSSI_AGE_LIMIT_KEY,                   "5" },
    { STAMON_PROBE_COUNT_THRESHOLD_KEY,            "3" },
    { NULL, NULL }
};


// ====================================================================
// Public API
// ====================================================================

LBD_STATUS stamon_init(void) {
    stamonState.dbgModule = dbgModuleFind("stamon");
    stamonState.dbgModule->Level = DBGINFO;

    if (stadb_registerActivityObserver(stamonActivityObserver, &stamonState) != LBD_OK ||
        stadb_registerRSSIObserver(stamonRSSIObserver, &stamonState) != LBD_OK ||
        steerexec_registerSteeringAllowedObserver(stamonActivityObserver,
                                                  &stamonState) != LBD_OK) {
        return LBD_NOK;
    }

    stamonState.config.instRSSINumSamples[wlanif_band_24g] =
        profileGetOptsInt(mdModuleID_StaMon,
                          STAMON_RSSI_MEASUREMENT_NUM_SAMPLES_W2_KEY,
                          stamonElementDefaultTable);
    stamonState.config.instRSSINumSamples[wlanif_band_5g] =
        profileGetOptsInt(mdModuleID_StaMon,
                          STAMON_RSSI_MEASUREMENT_NUM_SAMPLES_W5_KEY,
                          stamonElementDefaultTable);
    stamonState.config.inactRSSIXingThreshold[wlanif_band_24g] =
        profileGetOptsInt(mdModuleID_StaMon,
                          STAMON_INACT_RSSI_THRESHOLD_W2_KEY,
                          stamonElementDefaultTable);
    stamonState.config.inactRSSIXingThreshold[wlanif_band_5g] =
        profileGetOptsInt(mdModuleID_StaMon,
                          STAMON_INACT_RSSI_THRESHOLD_W5_KEY,
                          stamonElementDefaultTable);
    stamonState.config.rssiDiffEstimate5gFrom24g =
        profileGetOptsInt(mdModuleID_StaMon,
                          STAMON_RSSI_DIFF_EST_W5_FROM_W2_KEY,
                          stamonElementDefaultTable);
    stamonState.config.rssiDiffEstimate24gFrom5g =
        profileGetOptsInt(mdModuleID_StaMon,
                          STAMON_RSSI_DIFF_EST_W2_FROM_W5_KEY,
                          stamonElementDefaultTable);
    stamonState.config.rssiFreshnessLimit =
        profileGetOptsInt(mdModuleID_StaMon,
                          STAMON_RSSI_AGE_LIMIT_KEY,
                          stamonElementDefaultTable);
    stamonState.config.probeCountThreshold =
        profileGetOptsInt(mdModuleID_StaMon,
                          STAMON_PROBE_COUNT_THRESHOLD_KEY,
                          stamonElementDefaultTable);

    mdListenTableRegister(mdModuleID_BandMon, bandmon_event_overload_change,
                          stamonHandleOverloadChange);
    return LBD_OK;
}

LBD_STATUS stamon_fini(void) {
    LBD_STATUS status = LBD_OK;
    status |=
        stadb_unregisterActivityObserver(stamonActivityObserver, &stamonState);
    status |=
        stadb_unregisterRSSIObserver(stamonRSSIObserver, &stamonState);
    status |=
        steerexec_unregisterSteeringAllowedObserver(stamonActivityObserver,
                                                    &stamonState);
    return status;
}

// ====================================================================
// Private helper functions
// ====================================================================

/**
 * @brief Handle the activity status update about a STA become active
 *
 * It will try to cancel the ongoing steering if any.
 *
 * @param [in] entry  the STA that becomes active
 */
static void stamonHandleSTABecomeActive(stadbEntry_handle_t entry) {
    wlanif_band_e band = stadbEntry_getAssociatedBand(entry, NULL);
    if (band == wlanif_band_invalid) {
        // Invalid band should not happen now as it is being checked when getActStatus,
        // leave it here in case the getActStatus implementation changes later
        return;
    }

    steerexec_abortSteerToBand(entry,
                               band == wlanif_band_24g ? wlanif_band_5g : wlanif_band_24g,
                               NULL);
}

/**
 * @brief Handle the activity status update about a STA become inactive
 *
 * @param [in] entry  the STA that becomes inactive
 */
static void stamonHandleSTABecomeInactive(stadbEntry_handle_t entry) {
    stamonMakeSteerDecision(entry, LBD_FALSE /* final */);
}

/**
 * @brief Make a steering decision for a station
 *
 * If the STA is associated on 5 GHz band and the band is overloaded, steer the STA
 * immediately. Otherwise, if the target band RSSI is available and recent, make steering
 * decision using this RSSI info; otherwise, make an RSSI estimation based on recent RSSI
 * measurement on the associated band, or request a new RSSI measurement.
 *
 * @param [in] entry  the STA that needs to be checked
 * @param [in] final  flag indicating whether this is a final decision, meaning
 *                    it cannot request RSSI measurement and must make an RSSI
 *                    estimation based on available RSSI information
 */
static void stamonMakeSteerDecision(stadbEntry_handle_t entry, LBD_BOOL final) {
    wlanif_band_e associatedBand = stadbEntry_getAssociatedBand(entry, NULL);
    wlanif_band_e targetBand = wlanif_band_invalid;

    LBD_BOOL overload24g = bandmon_isOverloaded(wlanif_band_24g);
    LBD_BOOL overload5g = bandmon_isOverloaded(wlanif_band_5g);

    if (overload24g && overload5g) {
        // No steering is performed if both bands are overloaded. We just
        // let the client decide, as there is not really anything we can do
        // to make the situation better.
        return;
    }

    switch (associatedBand) {
        case wlanif_band_24g:
            // If overload at 5 GHz, there is no reason to steer there so do
            // not even bother measuring RSSI.
            if (overload5g) {
                return;
            } else {
                // Need RSSI information to make steering decision
                targetBand = stamonMakeSteerDecisionBasedOnRSSI(
                    entry, wlanif_band_24g, wlanif_band_5g, final);
            }
            break;
        case wlanif_band_5g:
            if (overload5g) {
                targetBand = wlanif_band_24g;
            } else if (overload24g) {
                // 2.4 GHz is overloaded. Thus, we will just let the client
                // decide where it wants to associate. Ideally it should stay
                // on 5 GHz as long as possible, since its performance may
                // degrade on 2.4 GHz due to the overload.
                return;
            } else {
                // Need RSSI information to make steering decision
                targetBand = stamonMakeSteerDecisionBasedOnRSSI(
                    entry, wlanif_band_5g, wlanif_band_24g, final);
            }
            break;
        default:
            // Invalid band should not happen now as it is being checked when getActStatus,
            // leave it here in case the getActStatus implementation changes later
            break;
    }

    stamonDoSteering(entry, targetBand);
}

/**
 * @brief Perform the steering operation if the target band is valid
 *
 * @param [in] entry  the STA that needs to be steered
 * @param [in] targetBand  the band that the STA should be steered to; do
 *                         nothing for invalid band
 */
static void stamonDoSteering(stadbEntry_handle_t entry, wlanif_band_e targetBand) {
    if (targetBand != wlanif_band_invalid) {
        // Steering decision has been made
        LBD_BOOL ignored;
        const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
        assert(staAddr);

        if (LBD_NOK == steerexec_steerToBand(entry, targetBand, &ignored)) {
            dbgf(stamonState.dbgModule, DBGERR,
                 "%s: Failed to steer "lbMACAddFmt(":")" to band %u.",
                 __func__, lbMACAddData(staAddr->ether_addr_octet), targetBand);
        } else if (!ignored){
            dbgf(stamonState.dbgModule, DBGINFO,
                 "%s: "lbMACAddFmt(":")" is being steered to band %u.",
                 __func__, lbMACAddData(staAddr->ether_addr_octet), targetBand);
        }
    }
}

/**
 * @brief Make steering decision based on RSSI information
 *
 * @param [in] entry  the STA that needs to be checked
 * @param [in] associatedBand  the current associated band
 * @param [in] targetBand  the target band that could be steered to
 * @param [in] final  flag indicating whether this is a final decision, meaning
 *                    it cannot request RSSI measurement and must make an RSSI
 *                    estimation based on available RSSI information
 *
 * @return the target band if the steering decision has been made;
 *         otherwise, return wlanif_band_invalid
 */
static wlanif_band_e
stamonMakeSteerDecisionBasedOnRSSI(stadbEntry_handle_t entry, wlanif_band_e associatedBand,
                                   wlanif_band_e targetBand, LBD_BOOL final) {
    u_int8_t targetBandRSSI, rssi;
    time_t rssiAgeSecs = 0xFF;
    u_int8_t probeCount = 0;

    targetBandRSSI = stadbEntry_getProbeRSSI(entry, targetBand, &rssiAgeSecs, &probeCount);
    if (targetBandRSSI != LBD_INVALID_RSSI &&
        rssiAgeSecs <= stamonState.config.rssiFreshnessLimit &&
        probeCount >= stamonState.config.probeCountThreshold) {
        // If target band RSSI is recent and valid, make the decision
        if (stamonCheckRSSI(targetBand, LBD_FALSE /* estimate */, targetBandRSSI)) {
            return targetBand;
        }
    } else {
        // Require RSSI estimate
        rssi = stadbEntry_getRSSI(entry, associatedBand, &rssiAgeSecs);
        if (final || (rssi != LBD_INVALID_RSSI &&
                      rssiAgeSecs <= stamonState.config.rssiFreshnessLimit)) {
            if (rssi != LBD_INVALID_RSSI &&
                stamonCheckRSSI(targetBand, LBD_TRUE /* estimate */, rssi)) {
                return targetBand;
            }
        } else {
            const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
            assert(staAddr);
            if (LBD_NOK == wlanif_requestStaRSSI(associatedBand, staAddr,
                                                 stamonState.config.instRSSINumSamples[associatedBand])) {
                dbgf(stamonState.dbgModule, DBGERR,
                     "%s: Failed to request RSSI measurement for "lbMACAddFmt(":")" on band %u",
                     __func__, lbMACAddData(staAddr->ether_addr_octet), associatedBand);
            }
        }
    }

    return wlanif_band_invalid;
}

/**
 * @brief Callback function invoked by the station database module when
 *        the activity status for a specific STA has been updated.
 *
 * @param [in] entry  the entry that was updated
 * @param [in] cookie  the pointer to our internal state
 */
static void stamonActivityObserver(stadbEntry_handle_t entry, void *cookie) {
    // We ignore the cookie here, since our state is static anyways.
    LBD_BOOL isActive;

    // We only care about entries that are dual band capable
    if (!stadbEntry_isDualBand(entry) ||
        LBD_NOK == stadbEntry_getActStatus(entry, &isActive, NULL)) {
        return;
    }

    if (isActive) {
        stamonHandleSTABecomeActive(entry);
    } else {
        stamonHandleSTABecomeInactive(entry);
    }
}

/**
 * @brief Callback function invoked by the station database module when
 *        the RSSI for a specific STA has been updated.
 *
 * For a dual-band and inactive STA, estimate RSSI value on the other band
 * based on the RSSI measurement received, and make the steering decision
 * based on the estimated RSSI value.
 *
 * @param [in] entry  the entry that was updated
 * @param [in] reason  the reason for the updated RSSI measurement
 * @param [in] cookie  the pointer to our internal state
 */
static void stamonRSSIObserver(stadbEntry_handle_t entry,
                               stadb_rssiUpdateReason_e reason, void *cookie) {
    // We ignore the cookie here, since our state is static anyways.
    LBD_BOOL isActive;

    // We only care about entries that are dual band capable and inactive
    if (!stadbEntry_isDualBand(entry) ||
        LBD_NOK == stadbEntry_getActStatus(entry, &isActive, NULL) ||
        isActive) {
        return;
    }

    stamonMakeSteerDecision(entry, LBD_TRUE /* final */);
}

/**
 * @brief Check if an RSSI measurement or estimation meets the threshold requirement
 *
 * The estimation is based on the RSSI measurement done on the other band.
 *
 * @param [in] band  the band on which to check RSSI against threshold
 * @param [in] estimate  whether to estimate RSSI before comparison
 * @param [in] rssi  if estimate, the RSSI measurement done on the other band;
 *                   otherwise, the RSSI measurement done on the given band.
 *                   Note that it's possible that after adjustment, RSSI
 *                   becomes negative, in which case, it should be detected
 *                   and marked not meet threshold
 *
 * @return LBD_TRUE if the RSSI value meets the threshold requirement; otherwise LBD_FALSE
 */
static LBD_BOOL stamonCheckRSSI(wlanif_band_e band, LBD_BOOL estimate, int rssi) {
    switch (band) {
        case wlanif_band_24g:
            if (estimate) {
                rssi += stamonState.config.rssiDiffEstimate24gFrom5g;
            }
            return rssi < stamonState.config.inactRSSIXingThreshold[wlanif_band_24g];
        case wlanif_band_5g:
            if (estimate) {
                rssi += stamonState.config.rssiDiffEstimate5gFrom24g;
            }
            return rssi > stamonState.config.inactRSSIXingThreshold[wlanif_band_5g];
        default:
            // Should not reach here now
            break;
    }

    return LBD_FALSE;
}

/**
 * @brief React to an event providing the updated overload status information
 */
static void stamonHandleOverloadChange(struct mdEventNode *event) {
    const bandmon_overloadChangeEvent_t *overloadChangeEvent =
        (const bandmon_overloadChangeEvent_t *) event->Data;
    if (stadb_iterate(stamonStaDBIterateCB, (void *) overloadChangeEvent) != LBD_OK) {
        dbgf(stamonState.dbgModule, DBGERR,
             "%s: Failed to iterate over STA DB; will wait for RSSI "
             "or inactivity updates", __func__);
        return;
    }
}

/**
 * @brief Handler for each entry in the station database.
 *
 * @param [in] entry  the current entry being examined
 * @param [in] cookie  the parameter provided in the stadb_iterate call
 */
static void stamonStaDBIterateCB(stadbEntry_handle_t entry, void *cookie) {
    const bandmon_overloadChangeEvent_t *event =
        (const bandmon_overloadChangeEvent_t *) cookie;

    LBD_BOOL isActive;
    wlanif_band_e band = stadbEntry_getAssociatedBand(entry, NULL);
    wlanif_band_e targetBand = wlanif_band_invalid;
    // We only care about entries that are associated, dual band capable,
    // inactive and potential target band not overloaded.
    if (band == wlanif_band_invalid ||
        (band == wlanif_band_24g && event->overload5g) ||
        (band == wlanif_band_5g && event->overload24g) ||
        !stadbEntry_isDualBand(entry) ||
        LBD_NOK == stadbEntry_getActStatus(entry, &isActive, NULL) ||
        isActive) {
        return;
    } else if (band == wlanif_band_5g && event->overload5g) {
        // If only 5 GHz becomes overloaded, steer all inactive
        // 5 GHz clients to 2.4 GHz.
        targetBand = wlanif_band_24g;
    } else {
        targetBand = stamonMakeSteerDecisionBasedOnRSSI(
            entry, band, band == wlanif_band_24g ? wlanif_band_5g : wlanif_band_24g,
            LBD_FALSE /* final */);
    }

    stamonDoSteering(entry, targetBand);
}
