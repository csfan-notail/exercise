// vim: set et sw=4 sts=4 cindent:
/*
 * @File: bandmon.c
 *
 * @Abstract: Implementation of band monitor public APIs
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

#include <dbg.h>

#if LBD_DBG_MENU
#include <cmd.h>
#endif  /* LBD_DBG_MENU */

#include "internal.h"
#include "module.h"
#include "profile.h"
#include "stadb.h"
#include "steerexec.h"
#include "diaglog.h"

#include "bandmon.h"
#include "bandmonDiaglogDefs.h"

// Forward decls
static void bandmonMenuInit(void);

static void bandmonReadConfig(void);

static void bandmonRSSIObserver(stadbEntry_handle_t entry,
                                stadb_rssiUpdateReason_e reason, void *cookie);
static void bandmonSteeringAllowedObserver(stadbEntry_handle_t entry, void *cookie);

static LBD_BOOL bandmonIsBandOverloaded(wlanif_band_e band);
static LBD_BOOL bandmonDetermineOperatingRegion(
        LBD_BOOL overloads[wlanif_band_invalid]);

static void bandmonHandleChanUtilEvent(struct mdEventNode *event);
static void bandmonHandleVAPRestartEvent(struct mdEventNode *event);
static void bandmonHandleChanUtil(wlanif_band_e band, u_int8_t utilization);

static inline char bandmonOverloadToChar(
        const LBD_BOOL overloads[wlanif_band_invalid],
        wlanif_band_e band);
static LBD_STATUS bandmonUpdateOverload(
        const LBD_BOOL overloads[wlanif_band_invalid]);
static void bandmonProcessOperatingRegion(
        const LBD_BOOL overloads[wlanif_band_invalid]);

static void bandmonUpdatePreAssocSteering(stadbEntry_handle_t entry,
                                          LBD_BOOL abortRequired);
static LBD_BOOL bandmonIsPreAssocSteeringPermitted(void);
static void bandmonStaDBIterateCB(stadbEntry_handle_t entry, void *cookie);

/**
 * @brief Internal state for the band monitor module.
 */
static struct {
    struct dbgModule *dbgModule;

#ifdef LBD_DBG_MENU
    /// When debug mode is enabled, all utilization events are ignored and
    /// changes have to come via the debug CLI.
    LBD_BOOL debugModeEnabled;
#endif /* LBD_DBG_MENU */

    /// Configuration data obtained at init time.
    struct {
        /// The per-band medium utilization overload thresholds for the slight
        /// overload condition.
        u_int8_t slightOverloadThresholds[wlanif_band_invalid];

        /// The per-band medium utilization overload thresholds for the heavy
        /// overload condition.
        u_int8_t heavyOverloadThresholds[wlanif_band_invalid];

        /// The amount of medium utilization reserved for specific clients
        /// on the band (eg. set top boxes).
        u_int8_t reservedUtilizations[wlanif_band_invalid];

        /// The maximum age an RSSI measurements can be before it is
        /// considered to old to be used when making a steering decision.
        time_t rssiMaxAge;

        /// The RSSI value on 5 GHz above which steering to 5 GHz under a
        /// 2.4 GHz overload is done.
        u_int8_t rssiThresholdW5;

        /// Number of probes required when non-associted band RSSI is valid
        u_int8_t probeCountThreshold;
    } config;

    /// Bitmask that is used to keep track of the updates for each band.
    /// The updates are complete on both bands when the bitmask is either
    /// all 0's or all 1's (up to the invalid band bit index).
    u_int8_t utilizationsState;

    /// The last measured utilization on each band.
    u_int8_t utilizations[wlanif_band_invalid];

    /// The current state of overload across both bands.
    LBD_BOOL currentOverloads[wlanif_band_invalid];

    /// The previous state of overload across both bands.
    LBD_BOOL previousOverloads[wlanif_band_invalid];
} bandmonState;

/**
 * @brief Default configuration values.
 *
 * These are used if the config file does not specify them.
 */
static struct profileElement bandmonElementDefaultTable[] = {
    { BANDMON_MU_SLIGHT_OVERLOAD_THRESHOLD_W2_KEY, "70" },
    { BANDMON_MU_HEAVY_OVERLOAD_THRESHOLD_W2_KEY,  "80" },
    { BANDMON_MU_SLIGHT_OVERLOAD_THRESHOLD_W5_KEY, "70" },
    { BANDMON_MU_HEAVY_OVERLOAD_THRESHOLD_W5_KEY,  "80" },
    { BANDMON_MU_RESERVE_W5_KEY,                   "0"  },
    { BANDMON_RSSI_MAX_AGE_KEY,                    "5"  },
    { BANDMON_RSSI_THRESHOLD_W5_KEY,               "30" },
    { BANDMON_PROBE_COUNT_THRESHOLD_KEY,            "1" },
    { NULL, NULL }
};

#define BANDMON_UTILIZATION_INVALID 255

// ====================================================================
// Public API
// ====================================================================

LBD_STATUS bandmon_init(void) {
    bandmonState.dbgModule = dbgModuleFind("bandmon");
    bandmonState.dbgModule->Level = DBGINFO;

    // Start with no reading on either band.
    bandmonState.utilizationsState = 0;
    memset(bandmonState.previousOverloads, 0,
           sizeof(bandmonState.previousOverloads));
    memset(bandmonState.currentOverloads, 0,
           sizeof(bandmonState.currentOverloads));

    if (stadb_registerRSSIObserver(bandmonRSSIObserver,
                                   &bandmonState) != LBD_OK) {
        return LBD_NOK;
    }

    // The same observer is used for steering prohibit changes as we need
    // the same behavior (assessing the RSSI and then possibly installing
    // the blacklists).
    if (steerexec_registerSteeringAllowedObserver(
                bandmonSteeringAllowedObserver, &bandmonState) != LBD_OK) {
        return LBD_NOK;
    }

    bandmonReadConfig();

    mdEventTableRegister(mdModuleID_BandMon, bandmon_event_maxnum);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_chan_util,
                          bandmonHandleChanUtilEvent);

    mdListenTableRegister(mdModuleID_WlanIF, wlanif_event_vap_restart,
                          bandmonHandleVAPRestartEvent);

    bandmonMenuInit();
    return LBD_OK;
}

LBD_BOOL bandmon_isOverloaded(wlanif_band_e band) {
    if (band != wlanif_band_invalid) {
        return bandmonState.currentOverloads[band];
    } else {
        return LBD_FALSE;
    }
}

LBD_STATUS bandmon_fini(void) {
    LBD_STATUS status =
        steerexec_unregisterSteeringAllowedObserver(
                bandmonSteeringAllowedObserver, &bandmonState);
    status |=
        stadb_unregisterRSSIObserver(bandmonRSSIObserver,
                                     &bandmonState);

    return status;
}

// ====================================================================
// Private helper functions
// ====================================================================

/**
 * @brief Read the configuration from the file.
 *
 * If there is medium utilization reserved, this will also scale the
 * thresholds accordingly.
 */
static void bandmonReadConfig(void) {
    bandmonState.config.slightOverloadThresholds[wlanif_band_24g] =
        profileGetOptsInt(mdModuleID_BandMon,
                          BANDMON_MU_SLIGHT_OVERLOAD_THRESHOLD_W2_KEY,
                          bandmonElementDefaultTable);
    bandmonState.config.slightOverloadThresholds[wlanif_band_5g] =
        profileGetOptsInt(mdModuleID_BandMon,
                          BANDMON_MU_SLIGHT_OVERLOAD_THRESHOLD_W5_KEY,
                          bandmonElementDefaultTable);

    bandmonState.config.heavyOverloadThresholds[wlanif_band_24g] =
        profileGetOptsInt(mdModuleID_BandMon,
                          BANDMON_MU_HEAVY_OVERLOAD_THRESHOLD_W2_KEY,
                          bandmonElementDefaultTable);
    bandmonState.config.heavyOverloadThresholds[wlanif_band_5g] =
        profileGetOptsInt(mdModuleID_BandMon,
                          BANDMON_MU_HEAVY_OVERLOAD_THRESHOLD_W5_KEY,
                          bandmonElementDefaultTable);

    bandmonState.config.reservedUtilizations[wlanif_band_5g] =
        profileGetOptsInt(mdModuleID_BandMon,
                          BANDMON_MU_RESERVE_W5_KEY,
                          bandmonElementDefaultTable);

    if (bandmonState.config.reservedUtilizations[wlanif_band_5g]) {
        // Scale each of the configured thresholds to be the remaining after
        // the reserved utilization is taken into consideration.
        bandmonState.config.slightOverloadThresholds[wlanif_band_5g] =
            (100 - bandmonState.config.reservedUtilizations[wlanif_band_5g]) *
            bandmonState.config.slightOverloadThresholds[wlanif_band_5g] / 100;

        bandmonState.config.heavyOverloadThresholds[wlanif_band_5g] =
            (100 - bandmonState.config.reservedUtilizations[wlanif_band_5g]) *
            bandmonState.config.heavyOverloadThresholds[wlanif_band_5g] / 100;
    }

    bandmonState.config.rssiMaxAge =
        profileGetOptsInt(mdModuleID_BandMon,
                          BANDMON_RSSI_MAX_AGE_KEY,
                          bandmonElementDefaultTable);
    bandmonState.config.rssiThresholdW5 =
        profileGetOptsInt(mdModuleID_BandMon,
                          BANDMON_RSSI_THRESHOLD_W5_KEY,
                          bandmonElementDefaultTable);

    bandmonState.config.probeCountThreshold =
        profileGetOptsInt(mdModuleID_BandMon,
                          BANDMON_PROBE_COUNT_THRESHOLD_KEY,
                          bandmonElementDefaultTable);
}

/**
 * @brief Callback function invoked by the station database module when
 *        the RSSI for a specific STA has been updated.
 *
 * @param [in] entry  the entry that was updated
 * @param [in] reason  the reason for the RSSI update
 * @param [in] cookie  the pointer to our internal state
 */
static void bandmonRSSIObserver(stadbEntry_handle_t entry,
                                stadb_rssiUpdateReason_e reason,
                                void *cookie) {
    // We ignore the cookie here, since our state is static anyways.
    // We also ignore the reason for the RSSI update as in all cases, we want
    // to do the same thing.
    bandmonUpdatePreAssocSteering(entry, LBD_FALSE /* abortRequired */);
}

/**
 * @brief Callback function invoked by the steering executor when steering
 *        becomes possible again for a given STA.
 *
 * @param [in] entry  the entry that is now steerable
 * @param [in] cookie  the pointer to our internal state
 */
static void bandmonSteeringAllowedObserver(stadbEntry_handle_t entry, void *cookie) {
    // We ignore the cookie here, since our state is static anyways.
    bandmonUpdatePreAssocSteering(entry, LBD_FALSE /* abortRequired */);
}

/**
 * @brief Determine whether the provided band is overloaded or not.
 *
 * Note that currently this only takes into consideration a reserved
 * utilization on 5 GHz.
 *
 * @return LBD_TRUE if the band is overloaded; otherwise LBD_FALSE
 */
static LBD_BOOL bandmonIsBandOverloaded(wlanif_band_e band) {
    if (wlanif_band_24g == band) {
        // Overloaded if either the 2.4 GHz utilization is above its heavy
        // overload threshold or the follow inequality holds:
        //
        //   L2' * util_5g < L5' * util_24g - L5' * L2
        //
        //   where L2 = light overload on 2.4 GHz
        //         L2' = heavy overload on 2.4 GHz
        //         L5' = heavy overload on 5 GHz
        //         util_24g = current utilization measurement on 2.4 GHz
        //         util_5g = current utilization measurement on 5 GHz
        if (bandmonState.utilizations[wlanif_band_24g] >
                bandmonState.config.heavyOverloadThresholds[wlanif_band_24g]) {
            return LBD_TRUE;
        } else {
            u_int8_t l2Prime =
                bandmonState.config.heavyOverloadThresholds[wlanif_band_24g];
            int32_t lhs =
                l2Prime * bandmonState.utilizations[wlanif_band_5g];

            u_int8_t l2 =
                bandmonState.config.slightOverloadThresholds[wlanif_band_24g];
            u_int8_t l5Prime =
                bandmonState.config.heavyOverloadThresholds[wlanif_band_5g];
            int32_t rhs =
                l5Prime * bandmonState.utilizations[wlanif_band_24g] -
                l5Prime * l2;

            return lhs < rhs;
        }
    } else {  // must be 5 GHz
        // Overloaded if either the 5 GHz utilization is above its heavy
        // overload threshold or the follow inequality holds:
        //
        //   L2' * util_5g > L5' * util_24g + L5 * L2'
        //
        //   where L2' = heavy overload on 2.4 GHz
        //         L5 = light overload on 5 GHz
        //         L5' = heavy overload on 5 GHz
        //         util_24g = current utilization measurement on 2.4 GHz
        //         util_5g = current utilization measurement on 5 GHz
        if (bandmonState.utilizations[wlanif_band_5g] >
                bandmonState.config.heavyOverloadThresholds[wlanif_band_5g]) {
            return LBD_TRUE;
        } else {
            u_int8_t l2Prime =
                bandmonState.config.heavyOverloadThresholds[wlanif_band_24g];
            int32_t lhs =
                l2Prime * bandmonState.utilizations[wlanif_band_5g];

            u_int8_t l5 =
                bandmonState.config.slightOverloadThresholds[wlanif_band_5g];
            u_int8_t l5Prime =
                bandmonState.config.heavyOverloadThresholds[wlanif_band_5g];
            int32_t rhs =
                l5Prime * bandmonState.utilizations[wlanif_band_24g] +
                l5 * l2Prime;

            return lhs > rhs;
        }
    }
}

/**
 * @brief Determine whether the system is operating in one of the overload
 *        regions or is operating in the normal region.
 *
 * @param [out] overloads  array of band overload flags that will be filled in
 *                         if this function returns LBD_TRUE
 *
 * @return LBD_TRUE if a new band overload assessment was done; otherwise
 *         LBD_FALSE
 */
static LBD_BOOL bandmonDetermineOperatingRegion(
        LBD_BOOL overloads[wlanif_band_invalid]) {
    // Both values must have been updated before we can say there has been
    // a change in the overload state.
    if (bandmonState.utilizationsState == 0 ||
        bandmonState.utilizationsState == (1 << wlanif_band_invalid) - 1) {
        dbgf(bandmonState.dbgModule, DBGINFO,
             "%s: 2.4 GHz [%u%%] 5 GHz [%u%%]",
             __func__, bandmonState.utilizations[wlanif_band_24g],
             bandmonState.utilizations[wlanif_band_5g]);

        size_t i;
        for (i = 0; i < wlanif_band_invalid; ++i) {
            overloads[i] = bandmonIsBandOverloaded((wlanif_band_e) i);
        }

        return LBD_TRUE;
    }

    return LBD_FALSE;
}

/**
 * @brief React to an event providing updated channel utilization information
 *        for a single band.
 */
static void bandmonHandleChanUtilEvent(struct mdEventNode *event) {
    const wlanif_chanUtilEvent_t *utilEvent =
        (const wlanif_chanUtilEvent_t *) event->Data;

#ifdef LBD_DBG_MENU
    if (!bandmonState.debugModeEnabled)
#endif
    {
        bandmonHandleChanUtil(utilEvent->band, utilEvent->utilization);
    }
}

/**
 * @brief React to an event that one of the VAPs on a band was restarted.
 */
static void bandmonHandleVAPRestartEvent(struct mdEventNode *event) {
    const wlanif_vapRestartEvent_t *restartEvent =
        (const wlanif_vapRestartEvent_t *) event->Data;

#ifdef LBD_DBG_MENU
    if (!bandmonState.debugModeEnabled)
#endif
    {
        dbgf(bandmonState.dbgModule, DBGINFO,
             "%s: Resetting utilization information due to VAP restart "
             "on band %u", __func__, restartEvent->band);

        // Reset the utilizations state information so we can wait for two
        // new events.
        bandmonState.utilizationsState = 0;
        memset(&bandmonState.utilizations, 0,
               sizeof(bandmonState.utilizations));

        LBD_BOOL overloads[wlanif_band_invalid];
        memset(&overloads, 0, sizeof(overloads));
        bandmonProcessOperatingRegion(overloads);
    }
}

/**
 * @brief React to a utilization measurement on the provided band.
 *
 * @param [in] band  the band on which the utilization was measured
 * @param [in] utilization   the utilization value (0-100)
 */
static void bandmonHandleChanUtil(wlanif_band_e band, u_int8_t utilization) {
    // This should always be the case, but we are defensive anyways.
    if (band < wlanif_band_invalid) {
        // This is toggling a bit for the band so that only when the bit
        // values match across the bands is a new check done for
        // overload (see bandmonDetermineOperatingRegion for where this
        // occurs).
        bandmonState.utilizationsState ^= (1 << band);
        bandmonState.utilizations[band] = utilization;

        if (diaglog_startEntry(mdModuleID_BandMon,
                               bandmon_msgId_utilization,
                               diaglog_level_info)) {
            diaglog_write8(band);
            diaglog_write8(utilization);
            diaglog_finishEntry();
        }

        LBD_BOOL overloads[wlanif_band_invalid];
        if (bandmonDetermineOperatingRegion(overloads)) {
            bandmonProcessOperatingRegion(overloads);
        }
    }
}

/**
 * @brief Convert the overload status for a given band to a character (for use
 *        in logging).
 *
 * @param [in] overloads  the current overload status on the supported bands
 * @param [in] band  the band for which the overload character is desired
 *
 * @return  the character to represent whether that band is overloaded or not
 */
static inline char bandmonOverloadToChar(
        const LBD_BOOL overloads[wlanif_band_invalid],
        wlanif_band_e band) {
    char overloadChar;
    switch (band) {
        case wlanif_band_24g:
            overloadChar = '2';
            break;

        case wlanif_band_5g:
            overloadChar = '5';
            break;

        default:
            // This is to keep the compiler happy. It should never happen
            // in practice.
            return ' ';
    }

    return overloads[band] ? overloadChar : ' ';
}

/**
 * @brief Check whether the operating region has changed and if it has,
 *        do the necessary ACL operations.
 *
 * @param [in] overloadState  the current overload condition
 */
static void bandmonProcessOperatingRegion(
        const LBD_BOOL overloads[wlanif_band_invalid]) {
    if (memcmp(&overloads[0], bandmonState.currentOverloads,
               sizeof(bandmonState.currentOverloads)) != 0) {
        dbgf(bandmonState.dbgModule, DBGINFO,
             "%s: Overload state change: Previous [%c%c] Current [%c%c]",
             __func__,
             bandmonOverloadToChar(bandmonState.currentOverloads,
                                   wlanif_band_24g),
             bandmonOverloadToChar(bandmonState.currentOverloads,
                                   wlanif_band_5g),
             bandmonOverloadToChar(overloads, wlanif_band_24g),
             bandmonOverloadToChar(overloads, wlanif_band_5g));

        if (bandmonUpdateOverload(overloads) != LBD_OK) {
            return;
        }

        memcpy(bandmonState.previousOverloads, bandmonState.currentOverloads,
               sizeof(bandmonState.previousOverloads));
        memcpy(bandmonState.currentOverloads, overloads,
               sizeof(bandmonState.currentOverloads));

        bandmon_overloadChangeEvent_t event;
        event.overload24g = bandmonState.currentOverloads[wlanif_band_24g];
        event.overload5g = bandmonState.currentOverloads[wlanif_band_5g];

        mdCreateEvent(mdModuleID_BandMon, mdEventPriority_Low,
                      bandmon_event_overload_change, &event, sizeof(event));

        if (diaglog_startEntry(mdModuleID_BandMon,
                               bandmon_msgId_overloadChange,
                               diaglog_level_demo)) {
            u_int8_t previousOverloads = 0, currentOverloads = 0;
            size_t i;
            for (i = 0; i < wlanif_band_invalid; ++i) {
                previousOverloads |= bandmonState.previousOverloads[i] << i;
                currentOverloads |= bandmonState.currentOverloads[i] << i;
            }

            diaglog_write8(previousOverloads);
            diaglog_write8(currentOverloads);
            diaglog_finishEntry();
        }

        if (stadb_iterate(bandmonStaDBIterateCB, NULL) != LBD_OK) {
            dbgf(bandmonState.dbgModule, DBGERR,
                 "%s: Failed to iterate over STA DB; will wait for RSSI "
                 "updates", __func__);
            return;
        }
    }
}

/**
 * @brief Inform the lower layers of a change in the overload state.
 *
 * @param [in] overloadState  the new overload state
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
static LBD_STATUS bandmonUpdateOverload(
        const LBD_BOOL overloads[wlanif_band_invalid]) {
    size_t i;
    for (i = 0; i < wlanif_band_invalid; ++i) {
        if (bandmonState.currentOverloads[i] != overloads[i]) {
            if (wlanif_setOverload((wlanif_band_e) i,
                                   overloads[i]) != LBD_OK) {
                dbgf(bandmonState.dbgModule, DBGERR,
                     "%s: Leaving state unchanged due to wlanif failure "
                     "for band %u", __func__, i);

                return LBD_NOK;
            }
        }
    }

    return LBD_OK;
}

/**
 * @brief Perform the necessary updates on the provided entry for
 *        pre-association steering based on the current overload state.
 *
 * @param [in] entry  the entry for whcih to update the steering decision
 * @param [in] abortRequired  LBD_TRUE if steering should be aborted if
 *                            the criteria for steering to the non-overloaded
 *                            band are not met; otherwise LBD_FALSE
 */
static void bandmonUpdatePreAssocSteering(stadbEntry_handle_t entry,
                                          LBD_BOOL abortRequired) {
    // We only care about entries that are dual band capable, are in network
    // (meaning they've previously associated), and that are not currently
    // associated.
    if (!stadbEntry_isDualBand(entry) || !stadbEntry_isInNetwork(entry) ||
        stadbEntry_getAssociatedBand(entry, NULL) != wlanif_band_invalid) {
        return;
    }

    if (bandmonState.currentOverloads[wlanif_band_24g]) {
        u_int8_t rssi;
        time_t rssiAgeSecs = 0xFF;
        u_int8_t rssiCount = 0;
        rssi = stadbEntry_getProbeRSSI(entry, wlanif_band_5g,
                                       &rssiAgeSecs, &rssiCount);
        if (rssi != LBD_INVALID_RSSI &&
            rssiAgeSecs < bandmonState.config.rssiMaxAge &&
            rssiCount >= bandmonState.config.probeCountThreshold &&
            rssi > bandmonState.config.rssiThresholdW5) {
            LBD_BOOL ignored;
            if (steerexec_steerToBand(entry, wlanif_band_5g,
                                      &ignored) == LBD_OK && !ignored) {
                const struct ether_addr *addr = stadbEntry_getAddr(entry);

                dbgf(bandmonState.dbgModule, DBGINFO,
                     "%s: Pre-association steer " lbMACAddFmt(":")
                     " to 5 GHz", __func__,
                     lbMACAddData(addr->ether_addr_octet));
            }
        } else if (abortRequired) {
            LBD_BOOL ignored;
            if (steerexec_abortSteerToBand(entry, wlanif_band_invalid,
                                           &ignored) == LBD_OK && !ignored) {
                const struct ether_addr *addr = stadbEntry_getAddr(entry);
                dbgf(bandmonState.dbgModule, DBGINFO,
                     "%s: Cancelled pre-association steer " lbMACAddFmt(":")
                     " to 5 GHz", __func__,
                     lbMACAddData(addr->ether_addr_octet));
            }
        } else {
            const struct ether_addr *addr = stadbEntry_getAddr(entry);
            dbgf(bandmonState.dbgModule, DBGDUMP,
                 "%s: No pre-association steering to 5 GHz for "
                 lbMACAddFmt(":") " (RSSI = %u, age=%ld)", __func__,
                 lbMACAddData(addr->ether_addr_octet), rssi, rssiAgeSecs);
        }
    } else if (bandmonState.currentOverloads[wlanif_band_5g]) {
        // Always steer dual band clients to 2.4 GHz if the 5 GHz band is
        // overloaded. Much of the time this will be a nop (since the
        // blacklist will already be in place), but since bandmon does
        // not keep track of which clients have been steered, this is
        // acceptable.
        LBD_BOOL ignored;
        if (steerexec_steerToBand(entry, wlanif_band_24g,
                                  &ignored) == LBD_OK && !ignored) {
            const struct ether_addr *addr = stadbEntry_getAddr(entry);
            dbgf(bandmonState.dbgModule, DBGINFO,
                 "%s: Pre-association steer " lbMACAddFmt(":")
                 " to 2.4 GHz", __func__,
                 lbMACAddData(addr->ether_addr_octet));
        }
    }
    // else no overload
}

/**
 * @brief Determine whether pre-association steering is permitted under the
 *        current overoad conditions or not.
 *
 *
 * @return LBD_TRUE if steering is permitted; otherwise LBD_FALSE
 */
static LBD_BOOL bandmonIsPreAssocSteeringPermitted(void) {
    // Pre-association steering is not permitted if all of the bands are
    // overloaded or none of the bands are overloaded.
    size_t numOverloaded = 0;
    size_t i;
    for (i = 0; i < wlanif_band_invalid; ++i) {
        if (bandmonState.currentOverloads[i]) {
            numOverloaded++;
        }
    }

    return !(numOverloaded == 0 || numOverloaded == wlanif_band_invalid);
}

/**
 * @brief Handler for each entry in the station database.
 *
 * @param [in] entry  the current entry being examined
 * @param [in] cookie  the parameter provided in the stadb_iterate call
 */
static void bandmonStaDBIterateCB(stadbEntry_handle_t entry, void *cookie) {
    // If we are no longer overloaded or are overloaded on both bands, for
    // all unassociated STAs that are dual band capable and in network
    // (meaning they've been associated before), abort any steering that
    // may be in progress.
    if (!bandmonIsPreAssocSteeringPermitted() &&
        stadbEntry_isDualBand(entry) && stadbEntry_isInNetwork(entry) &&
        stadbEntry_getAssociatedBand(entry, NULL) == wlanif_band_invalid) {
        LBD_BOOL ignored;
        LBD_STATUS result = steerexec_abortSteerToBand(entry,
                                                       wlanif_band_invalid,
                                                       &ignored);
        if (result == LBD_NOK) {
            const struct ether_addr *addr = stadbEntry_getAddr(entry);

            dbgf(bandmonState.dbgModule, DBGERR,
                 "%s: Failed to abort pre-association steering for "
                 lbMACAddFmt(":"), __func__,
                 lbMACAddData(addr->ether_addr_octet));
        } else if (!ignored) {
            const struct ether_addr *addr = stadbEntry_getAddr(entry);

            dbgf(bandmonState.dbgModule, DBGINFO,
                 "%s: Cancelled pre-association steering for "
                 lbMACAddFmt(":"), __func__,
                 lbMACAddData(addr->ether_addr_octet));
        }
    } else {
        // When moving from 5 GHz being overloaded to 2.4 GHz being
        // overloaded, any devices that do not meet the criteria for steering
        // to 5 GHz should have any steering that is currently installed
        // aborted.
        LBD_BOOL abortRequired =
            (bandmonState.currentOverloads[wlanif_band_24g] &&
             bandmonState.previousOverloads[wlanif_band_5g]);
        bandmonUpdatePreAssocSteering(entry, abortRequired);
    }
}

#ifdef LBD_DBG_MENU

static const char *bandmonMenuStatusHelp[] = {
    "s -- print station database contents",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void bandmonMenuStatusHandler(struct cmdContext *context, const char *cmd) {
    cmdf(context, "Utilization 2.4 GHz: %u%%%c%s\n",
         bandmonState.utilizations[wlanif_band_24g],
         bandmonState.utilizationsState & (1 << wlanif_band_24g) ? '*' : ' ',
         bandmonState.currentOverloads[wlanif_band_24g] ? " (overloaded)" : "");
    cmdf(context, "Utilization 5 GHz: %u%%%c%s\n",
         bandmonState.utilizations[wlanif_band_5g],
         bandmonState.utilizationsState & (1 << wlanif_band_5g) ? '*' : ' ',
         bandmonState.currentOverloads[wlanif_band_5g] ? " (overloaded)" : "");
    cmdf(context, "Debug mode: %s\n",
         bandmonState.debugModeEnabled ? "yes" : "no");
}

static const char *bandmonMenuDebugHelp[] = {
    "d -- enable/disable band monitor debug mode",
    "Usage:",
    "\td on: enable debug mode (ignoring utilization measurements)",
    "\td off: disable debug mode (handling utilization measurements)",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void bandmonMenuDebugHandler(struct cmdContext *context, const char *cmd) {
    LBD_BOOL isOn = LBD_FALSE;
    const char *arg = cmdWordFirst(cmd);

    if (!arg) {
        cmdf(context, "bandmon 'd' command requires on/off argument\n");
        return;
    }

    if (cmdWordEq(arg, "on")) {
        isOn = LBD_TRUE;
    } else if (cmdWordEq(arg, "off")) {
        isOn = LBD_FALSE;
    } else {
        cmdf(context, "bandmon 'd' command: invalid arg '%s'\n", arg);
        return;
    }

    dbgf(bandmonState.dbgModule, DBGINFO, "%s: Setting debug mode to %u",
         __func__, isOn);
    bandmonState.debugModeEnabled = isOn;
}

static const char *bandmonMenuUtilHelp[] = {
    "util -- inject a utilization measurement",
    "Usage:",
    "\tutil <2|5> <value>: inject utilization <value> on the band",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void bandmonMenuUtilHandler(struct cmdContext *context, const char *cmd) {
    wlanif_band_e band = wlanif_band_invalid;
    const char *arg = cmdWordFirst(cmd);

    if (!bandmonState.debugModeEnabled) {
        cmdf(context, "bandmon 'util' command not allowed unless debug mode "
                      "is enabled\n");
        return;
    }

    if (!arg || cmdWordLen(arg) != 1) {
        cmdf(context, "bandmon 'util' command invalid band - "
                      "expected '2|5'\n");
        return;
    }

    char bandChar = arg[0];
    switch (bandChar) {
        case '2':
            band = wlanif_band_24g;
            break;

        case '5':
            band = wlanif_band_5g;
            break;

        default:
            cmdf(context, "bandmon 'util' command invalid band '%c'\n",
                 bandChar);
            return;
    }

    arg = cmdWordNext(arg);
    if (!cmdWordDigits(arg)) {
        cmdf(context, "bandmon 'util' command invalid utilization '%s'\n",
             arg);
        return;
    }

    // The cmdWordDigits above does not allow a negative number, so we only
    // need to check for too large of values here.
    int utilization = atoi(arg);
    if (utilization > 100) {
        cmdf(context, "bandmon 'util' command: utilization must be "
                      "percentage\n");
        return;
    }

    dbgf(bandmonState.dbgModule, DBGINFO, "%s: Spoofing utilization of [%u%%] "
                                          "on band %u",
         __func__, utilization, band);

    // Now inject the utilization event as if it came from wlanif.
    bandmonHandleChanUtil(band, utilization);
}

static const struct cmdMenuItem bandmonMenu[] = {
    CMD_MENU_STANDARD_STUFF(),
    { "s", bandmonMenuStatusHandler, NULL, bandmonMenuStatusHelp },
    { "d", bandmonMenuDebugHandler, NULL, bandmonMenuDebugHelp },
    { "util", bandmonMenuUtilHandler, NULL, bandmonMenuUtilHelp },
    CMD_MENU_END()
};

static const char *bandmonMenuHelp[] = {
    "bandmon -- Band Monitor",
    NULL
};

static const struct cmdMenuItem bandmonMenuItem = {
    "bandmon",
    cmdMenu,
    (struct cmdMenuItem *) bandmonMenu,
    bandmonMenuHelp
};

#endif /* LBD_DBG_MENU */

static void bandmonMenuInit(void) {
#ifdef LBD_DBG_MENU
    cmdMainMenuAdd(&bandmonMenuItem);
#endif /* LBD_DBG_MENU */
}
