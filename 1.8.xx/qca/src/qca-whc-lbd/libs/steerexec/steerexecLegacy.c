// vim: set et sw=4 sts=4 cindent:
/*
 * @File: steerexecLegacy.c
 *
 * @Abstract: Package-level implementation of the steering executor for
 *            legacy clients
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
#include <assert.h>
#include <time.h>

#include <dbg.h>
#include <evloop.h>

#ifdef LBD_DBG_MENU
#include <cmd.h>
#endif

#include "internal.h"
#include "module.h"
#include "profile.h"

#include "steerexecLegacy.h"
#include "steerexecDiaglogDefs.h"
#include "stadb.h"
#include "diaglog.h"

// For now, we are only permitting 2 observers, as it is likely that the
// following will need to observe steering allowed changes:
//
// 1. Pre-association steering decision maker
// 2. Post-association steering decisoin maker
#define MAX_STEERING_ALLOWED_OBSERVERS 2

/**
 * @brief Internal state for the steering executor for legacy clients.
 */
struct steerexecLegacyPriv_t {
    steerexecLegacyConfig_t config;

    /// Observer for all RSSI updates to entries in the database.
    struct steerexecLegacySteeringAllowedObserver {
        LBD_BOOL isValid;
        steerexec_steeringAllowedObserverCB callback;
        void *cookie;
    } steeringAllowedObservers[MAX_STEERING_ALLOWED_OBSERVERS];

    /// Timer used to age out steering prohibitions.
    struct evloopTimeout prohibitTimer;

    /// Number of entries which are currently under a steering prohibition.
    size_t numSteeringProhibits;

    /// The time (in seconds) at which the steering prohibit timer should
    /// next expire.
    time_t nextProhibitExpiry;

    /// Timer used to age out devices marked as steering unfriendly.
    struct evloopTimeout unfriendlyTimer;

    /// The number of entries which are currently marked as steering
    /// unfriendly.
    size_t numSteeringUnfriendly;

    /// The time (in seconds) at which the steering unfriendly timer should
    /// next expire.
    time_t nextUnfriendlyExpiry;

    /// TImer used to age out devices blacklisted.
    struct evloopTimeout blacklistTimer;

    /// The number of entries which are blacklisted on one band.
    size_t numBlacklist;

    /// The time (in seconds) at which the clear blacklist timer should
    /// next expire.
    time_t nextBlacklistExpiry;

    struct dbgModule *dbgModule;
};

/**
 * @brief State information that the legacy steering executor stores with
 *        each STA that has been steered.
 */
typedef struct steerexecLegacySteeringState_t {
    /// Legacy steering context
    steerexecLegacyHandle_t context;

    /// The current band the STA is trying to be steered to.
    wlanif_band_e targetBand;

    /// The time the last steering was attempted or completed (whichever is
    /// more recent).
    time_t lastSteeringTime;

    /// The number of consecutive authentication rejects seen so far
    /// for the current attempt at steering.
    u_int32_t numAuthRejects;

    /// Whether steering for this device is currently prohibited or not.
    LBD_BOOL steeringProhibited;

    /// Flag indicating if this device is steering unfriendly
    LBD_BOOL steeringUnfriendly;

    /// Timer used to track T-steering period
    struct evloopTimeout tSteerTimer;

    /// The band the STA is blacklisted.
    wlanif_band_e blacklistBand;
} steerexecLegacySteeringState_t;

// ====================================================================
// Forward decls for internal "private" functions
// ====================================================================

static steerexecLegacySteeringState_t *steerexecLegacyGetOrCreateSteeringState(
        steerexecLegacyHandle_t exec, stadbEntry_handle_t entry);

static void steerexecLegacyDestroySteeringState(void *state);

static LBD_BOOL steerexecLegacyIsInSteeringQuietPeriod(
        steerexecLegacyHandle_t exec,
        stadbEntry_handle_t entry,
        steerexecLegacySteeringState_t *state,
        LBD_BOOL notifyObservers);
static void steerexecLegacyStartSteeringProhibit(
        steerexecLegacyHandle_t exec, steerexecLegacySteeringState_t *state,
        const struct ether_addr *staAddr);
static void steerexecLegacyProhibitTimeoutHandler(void *cookie);
static void steerexecLegacyProhibitIterateCB(stadbEntry_handle_t entry,
                                             void *cookie);

static void steerexecLegacyStartSteeringUnfriendly(
        steerexecLegacyHandle_t exec, steerexecLegacySteeringState_t *state,
        const struct ether_addr *staAddr);
static void steerexecLegacyUnfriendlyTimeoutHandler(void *cookie);
static void steerexecLegacyUnfriendlyIterateCB(stadbEntry_handle_t entry,
                                               void *cookie);

static void steerexecLegacyBlacklistTimeoutHandler(void *cookie);
static void steerexecLegacyBlacklistIterateCB(stadbEntry_handle_t entry,
                                              void *cookie);

static void steerexecLegacyNotifySteeringAllowedObservers(
        steerexecLegacyHandle_t exec, stadbEntry_handle_t entry);

static LBD_STATUS steerexecLegacyAbortSteerImpl(
        steerexecLegacyHandle_t exec, stadbEntry_handle_t entry,
        steerexecLegacySteeringState_t *state);

static wlanif_band_e steerexecLegacyDetermineBlacklistBand(
        steerexecLegacySteeringState_t *state);
static inline wlanif_band_e steerexecLegacyDetermineNonTargetBand(
        wlanif_band_e targetBand);

static void steerexecLegacyLowRSSIObserver(stadbEntry_handle_t entry, void *cookie);

static void steerexecLegacyTSteeringTimeoutHandler(void *cookie);

static void steerexecLegacyRSSIObserver(stadbEntry_handle_t entry,
                                        stadb_rssiUpdateReason_e reason,
                                        void *cookie);

static void steerexecLegacyMarkBlacklist(steerexecLegacyHandle_t exec,
        steerexecLegacySteeringState_t *state, wlanif_band_e disableBand);

static void steerexecLegacyDiagLogSteeringUnfriendly(
        const struct ether_addr *staAddr, LBD_BOOL isUnfriendly);
static void steerexecLegacyDiagLogSteeringProhibited(
        const struct ether_addr *staAddr, LBD_BOOL isProhibited);

// ====================================================================
// Public API
// ====================================================================

steerexecLegacyHandle_t steerexecLegacyCreate(const steerexecLegacyConfig_t *config,
                                              struct dbgModule *dbgModule) {
    steerexecLegacyHandle_t exec =
        calloc(1, sizeof(struct steerexecLegacyPriv_t));
    if (exec) {
        memcpy(&exec->config, config, sizeof(*config));

        exec->dbgModule = dbgModule;

        if (stadb_registerLowRSSIObserver(steerexecLegacyLowRSSIObserver, exec) != LBD_OK ||
            stadb_registerRSSIObserver(steerexecLegacyRSSIObserver, exec) != LBD_OK) {
            free(exec);
            return NULL;
        }

        evloopTimeoutCreate(&exec->prohibitTimer,
                            "steerexecLegacyProhibitTimeout",
                            steerexecLegacyProhibitTimeoutHandler,
                            exec);

        evloopTimeoutCreate(&exec->unfriendlyTimer,
                            "steerexecLegacyUnfriendlyTimeout",
                            steerexecLegacyUnfriendlyTimeoutHandler,
                            exec);

        evloopTimeoutCreate(&exec->blacklistTimer,
                            "steerexecLegacyClearBlacklistTimeout",
                            steerexecLegacyBlacklistTimeoutHandler,
                            exec);
    }

    return exec;
}

LBD_STATUS steerexecLegacySteerToBand(steerexecLegacyHandle_t exec,
                                      stadbEntry_handle_t entry,
                                      wlanif_band_e targetBand,
                                      LBD_BOOL *ignored) {
    if (!exec || !entry || targetBand >= wlanif_band_invalid) {
        return LBD_NOK;
    }

    steerexecLegacySteeringState_t *state =
        steerexecLegacyGetOrCreateSteeringState(exec, entry);
    if (!state) {
        return LBD_NOK;
    }

    const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
    assert(staAddr);

    // If there is any steering in progress, we need to handle that specially.
    if (state->targetBand != wlanif_band_invalid) {
        if (ignored) {
            *ignored = LBD_TRUE;
        }

        if (state->targetBand == targetBand) {
            // Nop. Already being steered to the target band.
            return LBD_OK;
        } else {
            dbgf(exec->dbgModule, DBGINFO,
                 "%s: Aborting steer to band %u for " lbMACAddFmt(":")
                 " due to request to steer to band %u",
                 __func__, state->targetBand,
                 lbMACAddData(staAddr->ether_addr_octet), targetBand);
            return steerexecLegacyAbortSteerImpl(exec, entry, state);
        }
    }

    // Steering unfriendly devices cannot be steered until a timer expires
    // that lets us try steering them again.
    if (state->steeringUnfriendly) {
        if (ignored) {
            *ignored = LBD_TRUE;
        }

        dbgf(exec->dbgModule, DBGDEBUG,
             "%s: Cannot steer " lbMACAddFmt(":") " to band %u due "
             "to being marked as steering unfriendly", __func__,
             lbMACAddData(staAddr->ether_addr_octet), targetBand);
        return LBD_OK;
    }

    // If the device was steered too recently, remove any blacklists that
    // may exist for the device.
    if (steerexecLegacyIsInSteeringQuietPeriod(
                exec, entry, state, LBD_FALSE /* notifyObservers */)) {
        if (ignored) {
            *ignored = LBD_TRUE;
        }

        dbgf(exec->dbgModule, DBGDEBUG,
             "%s: Cannot steer "  lbMACAddFmt(":") " to band %u due "
             "to quiet period", __func__,
             lbMACAddData(staAddr->ether_addr_octet), targetBand);
        return LBD_OK;
    }

    wlanif_band_e disableBand =
        steerexecLegacyDetermineNonTargetBand(targetBand);

    // We enable on the other band first to ensure that if the client itself
    // chooses to disassociate, it has somewhere to go.
    if (wlanif_enableBandForSTA(targetBand, staAddr) != LBD_OK) {
        dbgf(exec->dbgModule, DBGERR,
             "%s: Failed to enable association on band %u for "
             lbMACAddFmt(":"), __func__, targetBand,
             lbMACAddData(staAddr->ether_addr_octet));
        return LBD_NOK;
    }

    if (wlanif_disableBandForSTA(disableBand, staAddr) != LBD_OK) {
        dbgf(exec->dbgModule, DBGERR,
             "%s: Failed to disable association on band %u for "
             lbMACAddFmt(":"), __func__, disableBand,
             lbMACAddData(staAddr->ether_addr_octet));
        return LBD_NOK;
    }

    // Mark this entry as not allowing steering for the configured time.
    steerexecLegacyStartSteeringProhibit(exec, state, staAddr);

    // If the device is currently associated on the other band (the one being
    // disallowed), kick it out. If it is already associated on the target
    // band, we do not disassociate as this may be an attempt to lock the
    // client to the band.
    wlanif_band_e assocBand = stadbEntry_getAssociatedBand(entry, NULL);
    if (assocBand != wlanif_band_invalid && assocBand != targetBand) {
        if (wlanif_setProbeRespWHForSTA(assocBand, staAddr,
                                        LBD_TRUE) != LBD_OK) {
            // Even if we fail to enable probe response withholding (which
            // should not happen since the blacklist was installed properly
            // above), we proceed with the steering. Without the withholding,
            // the success rate will be reduced but it should still work
            // a good portion of the time.
            dbgf(exec->dbgModule, DBGERR,
                 "%s: Failed to disable probe responses for "
                 lbMACAddFmt(":") " on band %u", __func__,
                 lbMACAddData(staAddr->ether_addr_octet), assocBand);
        } else {
            dbgf(exec->dbgModule, DBGDEBUG,
                 "%s: Probe responses are being withheld for "
                 lbMACAddFmt(":") " on band %u", __func__,
                 lbMACAddData(staAddr->ether_addr_octet), assocBand);
        }

        if (wlanif_disassociateStaOnBand(assocBand, staAddr) != LBD_OK) {
            dbgf(exec->dbgModule, DBGERR,
                 "%s: Failed to force " lbMACAddFmt(":") " to disassociate "
                 "on band %u", __func__,
                 lbMACAddData(staAddr->ether_addr_octet), assocBand);

            // Should we remove the blacklist we installed above? For now,
            // we do not as the hope is if the client does disassociate, it
            // should go to the target band as desired.
            return LBD_NOK;
        }
    }

    if (ignored) {
        *ignored = LBD_FALSE;
    }

    state->targetBand = targetBand;

    if (diaglog_startEntry(mdModuleID_SteerExec,
                           steerexec_msgId_steerToBand,
                           diaglog_level_demo)) {
        diaglog_writeMAC(staAddr);
        diaglog_write8(assocBand);
        diaglog_write8(targetBand);
        diaglog_finishEntry();
    }

    return LBD_OK;
}

LBD_STATUS steerexecLegacyAbortSteerToBand(steerexecLegacyHandle_t exec,
                                           stadbEntry_handle_t entry,
                                           wlanif_band_e targetBand,
                                           LBD_BOOL *ignored) {
    if (!exec || !entry || targetBand > wlanif_band_invalid) {
        return LBD_NOK;
    }

    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (!state) {
        // There must not be any steering operation in progress, so silently
        // succeed.
        if (ignored) {
            *ignored = LBD_TRUE;
        }

        return LBD_OK;
    }

    // Confirm that steering is in progress to the specified band.
    if (state->targetBand != wlanif_band_invalid &&
        (targetBand == wlanif_band_invalid ||
         state->targetBand == targetBand)) {
        if (ignored) {
            *ignored = LBD_FALSE;
        }

        return steerexecLegacyAbortSteerImpl(exec, entry, state);
    }

    return LBD_NOK;
}

void steerexecLegacyHandleAssocUpdate(steerexecLegacyHandle_t exec,
                                      stadbEntry_handle_t entry) {
    if (!exec || !entry) {
        return;
    }

    // Ignore any devices that do not have a valid association state. They
    // must be brand new ones.
    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (!state) {
        return;
    }

    // Only if the state indicates it is associated does the executor need
    // to do anything.
    wlanif_band_e assocBand = stadbEntry_getAssociatedBand(entry, NULL);
    if (assocBand != wlanif_band_invalid && assocBand == state->targetBand) {
        // Steering completed.

        const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
        assert(staAddr);

        dbgf(exec->dbgModule, DBGINFO,
             "%s: Steering " lbMACAddFmt(":") " to band %u is complete",
             __func__, lbMACAddData(staAddr->ether_addr_octet),
             state->targetBand);

        state->targetBand = wlanif_band_invalid;
        evloopTimeoutUnregister(&state->tSteerTimer);

        // Special case for 5 GHz. We need to clear the blacklist immediately
        // as we want to make sure the client can associate to 2.4 GHz if
        // it wanders out of 5 GHz range (and we cannot react fast enough).
        if (assocBand == wlanif_band_5g) {
            if (wlanif_enableBandForSTA(wlanif_band_24g, staAddr) != LBD_OK) {
                // This generally should not happen, so we just log an error.
                // We could potentially try to clean up the blacklist later
                // if we wanted, but for now we do not.
                dbgf(exec->dbgModule, DBGERR,
                     "%s: Failed to clear blacklist for " lbMACAddFmt(":")
                     " on band %u.", __func__,
                     lbMACAddData(staAddr->ether_addr_octet),
                     wlanif_band_24g);
            }

            state->blacklistBand = wlanif_band_invalid;
        }
        else {
            // Record this entry as blacklisted on the disabled band,
            // and keep it for the configured time
            steerexecLegacyMarkBlacklist(exec, state, wlanif_band_5g);

            // To avoid confusing the user (if s/he happens to be looking
            // at a screen that might be affected by seeing beacons but not
            // probe responses, re-enable probe resposnes here. If the client
            // happens to try the 5 GHz band again, it will still get rejected.
            if (wlanif_setProbeRespWHForSTA(wlanif_band_5g, staAddr,
                                            LBD_FALSE) != LBD_OK) {
                // This should not happen unless the blacklist entry was
                // removed out from under us, so we just log an error.
                dbgf(exec->dbgModule, DBGERR,
                     "%s: Failed to enable probe responses for "
                     lbMACAddFmt(":") " on band %u", __func__,
                     lbMACAddData(staAddr->ether_addr_octet),
                     wlanif_band_5g);
            } else {
                dbgf(exec->dbgModule, DBGDEBUG,
                     "%s: Probe responses are enabled for "
                     lbMACAddFmt(":") " on band %u", __func__,
                     lbMACAddData(staAddr->ether_addr_octet),
                     wlanif_band_5g);
            }

        }

        steerexecLegacyStartSteeringProhibit(exec, state, staAddr);
    } else if (assocBand != wlanif_band_invalid &&
               state->targetBand == wlanif_band_invalid) {
        // If we had started T_Steering due to an auth reject and now the
        // STA has associated, we want to stop the timer.
        evloopTimeoutUnregister(&state->tSteerTimer);
    }
}

LBD_STATUS steerexecLegacyRegisterSteeringAllowedObserver(
        steerexecLegacyHandle_t exec,
        steerexec_steeringAllowedObserverCB callback,
        void *cookie) {
    if (!exec || !callback) {
        return LBD_NOK;
    }

    struct steerexecLegacySteeringAllowedObserver *freeSlot = NULL;
    size_t i;
    for (i = 0; i < MAX_STEERING_ALLOWED_OBSERVERS; ++i) {
        struct steerexecLegacySteeringAllowedObserver *curSlot =
            &exec->steeringAllowedObservers[i];
        if (curSlot->isValid && curSlot->callback == callback &&
            curSlot->cookie == cookie) {
            dbgf(exec->dbgModule, DBGERR, "%s: Duplicate registration "
                                          "(func %p, cookie %p)",
                 __func__, callback, cookie);
           return LBD_NOK;
        }

        if (!freeSlot && !curSlot->isValid) {
            freeSlot = curSlot;
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

LBD_STATUS steerexecLegacyUnregisterSteeringAllowedObserver(
        steerexecLegacyHandle_t exec,
        steerexec_steeringAllowedObserverCB callback,
        void *cookie) {
    if (!exec || !callback) {
        return LBD_NOK;
    }

    size_t i;
    for (i = 0; i < MAX_STEERING_ALLOWED_OBSERVERS; ++i) {
        struct steerexecLegacySteeringAllowedObserver *curSlot =
            &exec->steeringAllowedObservers[i];
        if (curSlot->isValid && curSlot->callback == callback &&
            curSlot->cookie == cookie) {
            curSlot->isValid = LBD_FALSE;
            curSlot->callback = NULL;
            curSlot->cookie = NULL;
            return LBD_OK;
        }
    }

    // No match found
    return LBD_NOK;
}

void steerexecLegacyDestroy(steerexecLegacyHandle_t exec) {
    if (exec) {
        stadb_unregisterLowRSSIObserver(steerexecLegacyLowRSSIObserver, exec);
        stadb_unregisterRSSIObserver(steerexecLegacyRSSIObserver, exec);
        evloopTimeoutUnregister(&exec->prohibitTimer);
        free(exec);
    }
}

// ====================================================================
// Private helper functions
// ====================================================================

/**
 * @brief Obtain the steering state entry for the STA, creating it if it does
 *        not exist.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] entry  the handle to the STA for which to get the state
 *
 * @return the state entry, or NULL if one could not be created
 */
static steerexecLegacySteeringState_t *steerexecLegacyGetOrCreateSteeringState(
        steerexecLegacyHandle_t exec, stadbEntry_handle_t entry) {
    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (!state) {
        state = calloc(1, sizeof(steerexecLegacySteeringState_t));
        if (!state) {
            return NULL;
        }

        state->context = exec;
        state->targetBand = wlanif_band_invalid;
        state->blacklistBand = wlanif_band_invalid;
        evloopTimeoutCreate(&state->tSteerTimer,
                            "steerexecLegacyTSteeringTimeout",
                            steerexecLegacyTSteeringTimeoutHandler,
                            entry);
        stadbEntry_setSteeringState(entry, state,
                                    steerexecLegacyDestroySteeringState);
    }

    return state;
}

/**
 * @brief Destructor function used to clean up the steering state for the
 *        legacy steering executor.
 */
static void steerexecLegacyDestroySteeringState(void *state) {
    free(state);
}

/**
 * @brief Start the time period during which steering is prohibited for this
 *        entry.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] state  the object that captures the steering state
 * @param [in] staAddr  the address of the STA that is having its prohibited
 *                      time started/updated
 */
static void steerexecLegacyStartSteeringProhibit(
        steerexecLegacyHandle_t exec,
        steerexecLegacySteeringState_t *state,
        const struct ether_addr *staAddr) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    LBD_BOOL generateLog = LBD_FALSE;
    if (!state->steeringProhibited) {
        // New entry being prohibited.
        exec->numSteeringProhibits++;
        generateLog = LBD_TRUE;
    }

    state->steeringProhibited = LBD_TRUE;
    state->lastSteeringTime = ts.tv_sec;

    if (exec->numSteeringProhibits == 1) {
        exec->nextProhibitExpiry =
            ts.tv_sec + exec->config.steeringProhibitTime + 1;

        // Initial timer expiry we let be for the max time. It'll get
        // rescheduled based on the earliest expiry time (if necessary).
        evloopTimeoutRegister(&exec->prohibitTimer,
                              exec->config.steeringProhibitTime + 1, 0);
    }

    if (generateLog) {
        steerexecLegacyDiagLogSteeringProhibited(staAddr,
                                                 LBD_TRUE /* isProhibited */);
    }
}

/**
 * @brief Start the time period during which this entry is marked as
 *        steering unfriendly.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] state  the object that captures the steering state
 */
static void steerexecLegacyStartSteeringUnfriendly(
        steerexecLegacyHandle_t exec,
        steerexecLegacySteeringState_t *state,
        const struct ether_addr *staAddr) {
    assert(!state->steeringUnfriendly);
    state->steeringUnfriendly = LBD_TRUE;
    exec->numSteeringUnfriendly++;

    if (exec->config.steeringUnfriendlyTime > 0 &&
        exec->numSteeringUnfriendly == 1) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);

        exec->nextUnfriendlyExpiry =
            ts.tv_sec + exec->config.steeringUnfriendlyTime + 1;

        // Initial timer expiry we let be for the max time. It'll get
        // rescheduled based on the earliest expiry time (if necessary).
        evloopTimeoutRegister(&exec->unfriendlyTimer,
                              exec->config.steeringUnfriendlyTime + 1, 0);
    }

    steerexecLegacyDiagLogSteeringUnfriendly(staAddr, LBD_TRUE);
}

/**
 * @brief Generate a diagnostic log indicating that the steering unfriendly
 *        state for a given client changed.
 *
 * @param [in] staAddr  the MAC address of the STA whose state changed
 * @param [in] isUnfriendly  flag indicating whether the STA is currently
 *                           considered steering unfriendly
 */
static void steerexecLegacyDiagLogSteeringUnfriendly(
        const struct ether_addr *staAddr, LBD_BOOL isUnfriendly) {
    if (diaglog_startEntry(mdModuleID_SteerExec,
                           steerexec_msgId_steeringUnfriendly,
                           diaglog_level_info)) {
        diaglog_writeMAC(staAddr);
        diaglog_write8(isUnfriendly);
        diaglog_finishEntry();
    }
}

/**
 * @brief Generate a diagnostic log indicating that the steering prohibited
 *        state for a given client changed.
 *
 * @param [in] staAddr  the MAC address of the STA whose state changed
 * @param [in] isProhibited  flag indicating whether the STA is currently
 *                           considered prohibited from steering
 */
static void steerexecLegacyDiagLogSteeringProhibited(
        const struct ether_addr *staAddr, LBD_BOOL isProhibited) {
    if (diaglog_startEntry(mdModuleID_SteerExec,
                           steerexec_msgId_steeringProhibited,
                           diaglog_level_info)) {
        diaglog_writeMAC(staAddr);
        diaglog_write8(isProhibited);
        diaglog_finishEntry();
    }
}

/**
 * @brief Mark the STA being blacklisted on the given band.
 *
 * Start the blacklist timer when the first STA is blacklisted.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] state  the object that captures the steering state
 * @param [in] blacklistBand  the band to be blacklisted on, wlanif_band_invalid
 *                            means it is removed from blacklist
 */
static void steerexecLegacyMarkBlacklist(steerexecLegacyHandle_t exec,
        steerexecLegacySteeringState_t *state, wlanif_band_e blacklistBand) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    LBD_BOOL newEntry = LBD_FALSE;
    if (state->blacklistBand == wlanif_band_invalid) {
        // New entry being blacklisted.
        newEntry = LBD_TRUE;
        exec->numBlacklist++;
    } else if (state->blacklistBand != blacklistBand) {
        // Entry is blacklisted on the other band, should restart timer
        newEntry = LBD_TRUE;
    }

    state->blacklistBand = blacklistBand;

    if (exec->config.blacklistTime > 0 && newEntry &&
        exec->numBlacklist == 1) {
        exec->nextBlacklistExpiry =
            ts.tv_sec + exec->config.blacklistTime + 1;

        // Initial timer expiry we let be for the max time. It'll get
        // rescheduled based on the earliest expiry time (if necessary).
        evloopTimeoutRegister(&exec->blacklistTimer,
                              exec->config.blacklistTime + 1, 0);
    }
}

/**
 * @brief Examine a single entry to see if its steering prohibition period
 *        has elapsed.
 *
 * @param [in] entry  the entry to examine
 * @param [in] cookie  the executor handle
 */
static void steerexecLegacyProhibitIterateCB(stadbEntry_handle_t entry,
                                             void *cookie) {
    if (!stadbEntry_isDualBand(entry) || !stadbEntry_isInNetwork(entry)) {
        return;
    }

    steerexecLegacyHandle_t exec = (steerexecLegacyHandle_t) cookie;
    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (state &&
        steerexecLegacyIsInSteeringQuietPeriod(exec, entry, state,
                                               LBD_TRUE /* notifyObservers */)) {
        // Determine if the next expiry time is sooner than the one currently
        // set. If so, update the time so that it is used when the timer is
        // next scheduled.
        time_t expiryTime = state->lastSteeringTime +
            exec->config.steeringProhibitTime + 1;
        if (expiryTime < exec->nextProhibitExpiry) {
            exec->nextProhibitExpiry = expiryTime;
        }
    }
}

/**
 * @brief Handle the periodic timer that signals we should check how many
 *        entries are still waiting for steering prohibition to complete.
 *
 * @param [in] cookie  the legacy steering handle
 */
static void steerexecLegacyProhibitTimeoutHandler(void *cookie) {
    steerexecLegacyHandle_t exec = (steerexecLegacyHandle_t) cookie;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    // This is the worst case. The iteration will adjust this based on the
    // actual devices that are still under prohibition.
    exec->nextProhibitExpiry = ts.tv_sec + exec->config.steeringProhibitTime + 1;

    if (stadb_iterate(steerexecLegacyProhibitIterateCB, exec) != LBD_OK) {
        dbgf(exec->dbgModule, DBGERR,
             "%s: Failed to iterate over station database", __func__);

        // For now we are falling through to reschedule the timer.
    }

    if (exec->numSteeringProhibits != 0) {
        evloopTimeoutRegister(&exec->prohibitTimer,
                              exec->nextProhibitExpiry - ts.tv_sec, 0);
    }
}

/**
 * @brief Examine a single entry to see if its steering unfriendly period
 *        has elapsed.
 *
 * @param [in] entry  the entry to examine
 * @param [in] cookie  the executor handle
 */
static void steerexecLegacyUnfriendlyIterateCB(stadbEntry_handle_t entry,
                                               void *cookie) {
    if (!stadbEntry_isDualBand(entry) || !stadbEntry_isInNetwork(entry)) {
        return;
    }

    steerexecLegacyHandle_t exec = (steerexecLegacyHandle_t) cookie;
    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (state && state->steeringUnfriendly) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);

        if (ts.tv_sec - state->lastSteeringTime >
            exec->config.steeringUnfriendlyTime) {
            const struct ether_addr *addr = stadbEntry_getAddr(entry);
            assert(addr);

            dbgf(exec->dbgModule, DBGINFO,
                 "%s: Cleared steering unfriendly flag for " lbMACAddFmt(":"),
                 __func__, lbMACAddData(addr->ether_addr_octet));

            state->steeringUnfriendly = LBD_FALSE;
            exec->numSteeringUnfriendly--;

            steerexecLegacyDiagLogSteeringUnfriendly(addr, LBD_FALSE);

            steerexecLegacyNotifySteeringAllowedObservers(exec, entry);
        } else {
            time_t expiryTime = state->lastSteeringTime +
                exec->config.steeringUnfriendlyTime + 1;
            if (expiryTime < exec->nextUnfriendlyExpiry) {
                exec->nextUnfriendlyExpiry = expiryTime;
            }
        }
    }
}

/**
 * @brief Handle the periodic timer that signals we should check how many
 *        entries are still waiting to have their steering unfriendly
 *        flag cleared.
 *
 * @param [in] cookie  the legacy steering handle
 */
static void steerexecLegacyUnfriendlyTimeoutHandler(void *cookie) {
    steerexecLegacyHandle_t exec = (steerexecLegacyHandle_t) cookie;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    // This is the worst case. The iteration will adjust this based on the
    // actual devices that are still under prohibition.
    exec->nextUnfriendlyExpiry =
        ts.tv_sec + exec->config.steeringUnfriendlyTime + 1;

    if (stadb_iterate(steerexecLegacyUnfriendlyIterateCB, exec) != LBD_OK) {
        dbgf(exec->dbgModule, DBGERR,
             "%s: Failed to iterate over station database", __func__);

        // For now we are falling through to reschedule the timer.
    }

    if (exec->numSteeringUnfriendly != 0) {
        evloopTimeoutRegister(&exec->unfriendlyTimer,
                              exec->nextUnfriendlyExpiry - ts.tv_sec, 0);
    }
}

/**
 * @brief Examine a single entry to see if its blacklist period
 *        has elapsed.
 *
 * @param [in] entry  the entry to examine
 * @param [in] cookie  the executor handle
 */
void steerexecLegacyBlacklistIterateCB(stadbEntry_handle_t entry,
                                       void *cookie) {

    steerexecLegacyHandle_t exec = (steerexecLegacyHandle_t) cookie;
    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (state && state->blacklistBand != wlanif_band_invalid) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);

        if (ts.tv_sec - state->lastSteeringTime >
            exec->config.blacklistTime) {
            const struct ether_addr *addr = stadbEntry_getAddr(entry);
            assert(addr);

            if (LBD_OK == wlanif_enableBandForSTA(state->blacklistBand, addr)) {
                dbgf(exec->dbgModule, DBGINFO,
                     "%s: Cleared blacklist on band %u for "lbMACAddFmt(":")" due to aging",
                     __func__, state->blacklistBand, lbMACAddData(addr->ether_addr_octet));

                state->blacklistBand = wlanif_band_invalid;
                exec->numBlacklist--;
            }

            // If enable band error, timer will be rescheduled and it will be retried
            // when timer expires next time.
        } else {
            time_t expiryTime = state->lastSteeringTime +
                exec->config.blacklistTime + 1;
            if (expiryTime < exec->nextBlacklistExpiry) {
                exec->nextBlacklistExpiry = expiryTime;
            }
        }
    }
}

/**
 * @brief Handle the periodic timer that signals we should check how many
 *        entries are still waiting to be removed from blacklist
 *
 * @param [in] cookie  the legacy steering handle
 */
static void steerexecLegacyBlacklistTimeoutHandler(void *cookie) {
    steerexecLegacyHandle_t exec = (steerexecLegacyHandle_t) cookie;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    // This is the worst case. The iteration will adjust this based on the
    // actual devices that are still under prohibition.
    exec->nextBlacklistExpiry =
        ts.tv_sec + exec->config.blacklistTime + 1;

    if (stadb_iterate(steerexecLegacyBlacklistIterateCB, exec) != LBD_OK) {
        dbgf(exec->dbgModule, DBGERR,
             "%s: Failed to iterate over station database", __func__);

        // For now we are falling through to reschedule the timer.
    }

    if (exec->numBlacklist != 0) {
        evloopTimeoutRegister(&exec->blacklistTimer,
                              exec->nextBlacklistExpiry - ts.tv_sec, 0);
    }
}

/**
 * @brief Notify all registered oberservers that the provided entry can
 *        now be steered.
 *
 * @param [in] entry  the entry that was updated
 */
static void steerexecLegacyNotifySteeringAllowedObservers(
        steerexecLegacyHandle_t exec, stadbEntry_handle_t entry) {
    size_t i;
    for (i = 0; i < MAX_STEERING_ALLOWED_OBSERVERS; ++i) {
        struct steerexecLegacySteeringAllowedObserver *curSlot =
            &exec->steeringAllowedObservers[i];
        if (curSlot->isValid) {
            curSlot->callback(entry, curSlot->cookie);
        }
    }
}

/**
 * @brief Determine if the state indicates the entry is eligible for steering
 *        or not.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] entry  the entry of the STA being examined
 * @param [in] state  the object to check for steering or not
 * @param [in] notifyObservers  LBD_TRUE if observers registered for the
 *                              steering prohibit callback should be notified
 *
 * @return LBD_TRUE if the entry is still not allowed to be steered (due to its
 *         last steering being too recently); LBD_FALSE if it is eligible to be
 *         steered
 */
static LBD_BOOL steerexecLegacyIsInSteeringQuietPeriod(
        steerexecLegacyHandle_t exec,
        stadbEntry_handle_t entry,
        steerexecLegacySteeringState_t *state,
        LBD_BOOL notifyObservers) {
    if (state->steeringProhibited) {
        // Check if enough time has elapsed and if so, clear the flag.
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);

        if (ts.tv_sec - state->lastSteeringTime >
            exec->config.steeringProhibitTime) {
            const struct ether_addr *addr = stadbEntry_getAddr(entry);
            assert(addr);

            dbgf(exec->dbgModule, DBGINFO,
                 "%s: " lbMACAddFmt(":") " became eligible for steering",
                 __func__, lbMACAddData(addr->ether_addr_octet));

            state->steeringProhibited = LBD_FALSE;
            exec->numSteeringProhibits--;

            if (notifyObservers && !state->steeringUnfriendly) {
                steerexecLegacyNotifySteeringAllowedObservers(exec, entry);
            }

            steerexecLegacyDiagLogSteeringProhibited(
                    addr, LBD_FALSE /* isProhibited */);
        }
    }

    return state->steeringProhibited;
}

/**
 * @brief The core implementation of the package level
 *        steerexecLegacyAbortSteer() function.
 *
 * This allows the client to associate on the non-target band (where it was
 * previously disallowed).
 *
 * @pre exec, entry, and state are all valid
 *
 * @param [in] exec  the executor instance to use
 * @param [in] entry  the handle to the STA for which to abort
 * @param [in] currentTargetBand  the band to which the device is
 *                                currently trying to be steered
 * @param [in] state  the internal state used by the executor
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
static LBD_STATUS steerexecLegacyAbortSteerImpl(
        steerexecLegacyHandle_t exec, stadbEntry_handle_t entry,
        steerexecLegacySteeringState_t *state) {
    const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
    assert(staAddr);

    wlanif_band_e disabledBand =
        steerexecLegacyDetermineBlacklistBand(state);

    if (disabledBand != wlanif_band_invalid) {
        if (wlanif_enableBandForSTA(disabledBand, staAddr) != LBD_OK) {
            dbgf(exec->dbgModule, DBGERR,
                 "%s: Failed to re-enable band %u for " lbMACAddFmt(":"),
                 __func__, disabledBand, lbMACAddData(staAddr->ether_addr_octet));
            return LBD_NOK;
        }

        state->targetBand = wlanif_band_invalid;

        if (state->blacklistBand != wlanif_band_invalid) {
            state->blacklistBand = wlanif_band_invalid;
            exec->numBlacklist--;
        }

        evloopTimeoutUnregister(&state->tSteerTimer);
        steerexecLegacyStartSteeringProhibit(exec, state, staAddr);

        if (diaglog_startEntry(mdModuleID_SteerExec,
                               steerexec_msgId_abortSteerToBand,
                               diaglog_level_demo)) {
            diaglog_writeMAC(staAddr);
            diaglog_write8(disabledBand);
            diaglog_finishEntry();
        }

        return LBD_OK;
    } else {
        // This should never happen in reality, as we only try to do the
        // abort if the target band or blacklist band is valid (which means
        // that the disabled band will be valid).
        dbgf(exec->dbgModule, DBGERR,
             "%s: No band is disabled for " lbMACAddFmt(":"),
             __func__, lbMACAddData(staAddr->ether_addr_octet));
        return LBD_NOK;
    }
}

/**
 * @brief Determine the band on which a blacklist is currently installed
 *        for the STA.
 *
 * This takes into consideration whether steering is currently in progress
 * or has already completed but there is still a blacklist present.
 *
 * @param [in] state  the internal state used by the executor for the entry
 */
static wlanif_band_e steerexecLegacyDetermineBlacklistBand(
        steerexecLegacySteeringState_t *state) {
    if (state->targetBand != wlanif_band_invalid) {
        return steerexecLegacyDetermineNonTargetBand(state->targetBand);
    } else {
        return state->blacklistBand;
    }
}

/**
 * @brief For a given band, determine what the other band would be.
 *
 * @param [in] band  the target band for which to determine the other band
 */
static inline wlanif_band_e steerexecLegacyDetermineNonTargetBand(
        wlanif_band_e targetBand) {
    return (targetBand == wlanif_band_24g) ?  wlanif_band_5g : wlanif_band_24g;
}

/**
 * @brief Callback function invoked by the station database module when
 *        the RSSI for a specific STA went below the low RSSI threshold
 *
 * For a dual band STA, the blacklist installed on the other band will
 * be removed.
 *
 * @param [in] entry  the entry that was updated
 * @param [in] cookie  the pointer to our internal state
 */
static void steerexecLegacyLowRSSIObserver(stadbEntry_handle_t entry, void *cookie) {
    const struct steerexecLegacyPriv_t *steerexecLegacyState =
        (const struct steerexecLegacyPriv_t *) cookie;
    assert(steerexecLegacyState);

    // We only care about entries that are dual band capable
    if (!stadbEntry_isDualBand(entry)) {
        return;
    }

    wlanif_band_e associatedBand = stadbEntry_getAssociatedBand(entry, NULL);

    if (associatedBand == wlanif_band_invalid) {
        return;
    }

    // Be conservative here by double checking RSSI is below the low threshold
    u_int8_t rssi = stadbEntry_getRSSI(entry, associatedBand, NULL);
    if (rssi != LBD_INVALID_RSSI &&
        rssi < steerexecLegacyState->config.lowRSSIXingThreshold[associatedBand]) {
        const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
        assert(staAddr);

        if (LBD_NOK == wlanif_enableBandForSTA(
                           associatedBand == wlanif_band_5g ? wlanif_band_24g :
                                                          wlanif_band_5g,
                           staAddr)) {
            dbgf(steerexecLegacyState->dbgModule, DBGERR,
                 "%s: Failed to clear blacklist for "lbMACAddFmt(":")" on band %u.",
                 __func__, lbMACAddData(staAddr->ether_addr_octet),
                 associatedBand == wlanif_band_5g ? wlanif_band_24g :
                                                    wlanif_band_5g);
        } else {
            dbgf(steerexecLegacyState->dbgModule, DBGINFO,
                 "%s: Blacklist is cleared for "lbMACAddFmt(":")" on band %u "
                 "due to RSSI going below the low threshold.",
                 __func__, lbMACAddData(staAddr->ether_addr_octet),
                 associatedBand == wlanif_band_5g ? wlanif_band_24g :
                                                    wlanif_band_5g);
        }
    }
}

/**
 * @brief Timeout handler for T-Steering timer
 *
 * It will abort current steering and mark the device as steering unfriendly
 *
 * @param [in] cookie  the legacy steering state
 */
static void steerexecLegacyTSteeringTimeoutHandler(void *cookie) {
    stadbEntry_handle_t entry = (stadbEntry_handle_t) cookie;
    assert(entry);
    const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
    assert(staAddr);

    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    assert(state);

    wlanif_band_e targetBand = state->targetBand;
    steerexecLegacyAbortSteerImpl(state->context, entry, state);

    dbgf(state->context->dbgModule, DBGINFO,
         "%s: "lbMACAddFmt(":")" not associated to band %u within %u seconds; "
         "abort steering and mark the device as steering unfriendly.",
         __func__, lbMACAddData(staAddr->ether_addr_octet),
         targetBand, state->context->config.tSteering);

    steerexecLegacyStartSteeringUnfriendly(state->context, state,
                                           staAddr);
}

/**
 * @brief React to an authentication rejection that was sent, aborting
 *        steering if necessary.
 *
 * Also start the T-Steering timer if the STA has a steering in progress
 * and the timer has not started.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] entry  the entry for which an auth reject was sent
 * @param [in] state  the internal state used by the executor
 *
 * @return LBD_TRUE if the steering was aborted; otherwise LBD_FALSE
 */
static LBD_BOOL steerexecLegacyHandleAuthRej(
        struct steerexecLegacyPriv_t *exec, stadbEntry_handle_t entry,
        steerexecLegacySteeringState_t *state) {
    // If TSteering timer is not running, start it
    unsigned secsRemaining, usecsRemaining;
    if(evloopTimeoutRemaining(&state->tSteerTimer, &secsRemaining,
                              &usecsRemaining)) {
        evloopTimeoutRegister(&state->tSteerTimer, exec->config.tSteering,
                              0 /* USec */);
        state->numAuthRejects = 1;
        return LBD_FALSE;
    } else {
        // Update the authentication reject count, but only if enough time
        // has elapsed from the first one.
        if (exec->config.tSteering - secsRemaining >
                exec->config.initialAuthRejCoalesceTime) {
            state->numAuthRejects++;
        }

        // If there have been too many auth rejects, abort the steering.
        if (state->numAuthRejects == exec->config.authRejMax) {
            const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
            assert(staAddr);

            // Record original target band for log purpose
            wlanif_band_e targetBand = state->targetBand;

            steerexecLegacyAbortSteerImpl(exec, entry, state);

            dbgf(exec->dbgModule, DBGINFO,
                "%s: Aborting steer to band %u for " lbMACAddFmt(":")
                " due to repeated auth rejects",
                __func__, targetBand, lbMACAddData(staAddr->ether_addr_octet));

            steerexecLegacyStartSteeringUnfriendly(state->context, state,
                                                   staAddr);

            return LBD_TRUE;
        }

        // Have not hit the limit yet.
        return LBD_FALSE;
    }
}

/**
 * @brief Callback function invoked by the station database module when
 *        the RSSI for a specific STA got updates
 *
 * If a STA is in steering progress and the target band RSSI goes below
 * the threshold, the steering will be cancelled.
 *
 * In addition, the T_Steering timer will be started on the first auth
 * reject.
 *
 * @param [in] entry  the entry that was updated
 * @param [in] reason  the reason the RSSI value was updated
 * @param [in] cookie  the pointer to our internal state
 */
static void steerexecLegacyRSSIObserver(stadbEntry_handle_t entry,
                                        stadb_rssiUpdateReason_e reason,
                                        void *cookie) {
    struct steerexecLegacyPriv_t *exec =
        (struct steerexecLegacyPriv_t *) cookie;
    assert(exec);
    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (!state ||
        steerexecLegacyDetermineBlacklistBand(state) == wlanif_band_invalid) {
        // Ignore entry that is not currently blacklisted.
        return;
    }

    // If the auth reject handling indicates the abort already happened, we
    // do not even need to examine the RSSI.
    if (reason == stadb_rssiUpdateReason_authrej &&
        steerexecLegacyHandleAuthRej(exec, entry, state)) {
        return;
    }

    // If steering is still in progress, we need to check the RSSI to make
    // sure the target band is not too low.
    if (state->targetBand != wlanif_band_invalid) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);

        time_t rssiAgeSecs = 0xFF;
        u_int8_t targetBandRSSI = stadbEntry_getRSSI(entry, state->targetBand,
                                                     &rssiAgeSecs);

        if (targetBandRSSI != LBD_INVALID_RSSI &&
            rssiAgeSecs <= (ts.tv_sec - state->lastSteeringTime) &&
            targetBandRSSI < exec->config.targetLowRSSIThreshold[state->targetBand]) {
            const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
            assert(staAddr);

            // Record original target band for log purpose
            wlanif_band_e targetBand = state->targetBand;

            steerexecLegacyAbortSteerImpl(exec, entry, state);

            dbgf(exec->dbgModule, DBGINFO,
                "%s: Aborting steer to band %u for " lbMACAddFmt(":")
                " due to target band RSSI being too low (%u dB)",
                __func__, targetBand, lbMACAddData(staAddr->ether_addr_octet),
                targetBandRSSI);
        }
    }
}


#ifdef GMOCK_UNIT_TESTS
LBD_BOOL steerexecLegacyIsSTASteeringUnfriendly(stadbEntry_handle_t entry) {
    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (!state) {
        return LBD_FALSE;
    }

    return state->steeringUnfriendly;
}
#endif

#ifdef LBD_DBG_MENU

void steerexecLegacyDumpHeader(struct cmdContext *context,
                               steerexecLegacyHandle_t exec) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    cmdf(context, "Overall state:\n");
    cmdf(context, "  Current # STAs prohibited from steering: %u\n",
         exec->numSteeringProhibits);

    if (exec->numSteeringProhibits > 0) {
        cmdf(context, "    Next prohibit update: %u seconds\n",
             exec->nextProhibitExpiry - ts.tv_sec);
    }

    cmdf(context, "  Current # STAs marked as steering unfriendly: %u\n",
         exec->numSteeringUnfriendly);

    if (exec->numSteeringUnfriendly > 0 &&
        exec->config.steeringUnfriendlyTime > 0) {
        cmdf(context, "    Next unfriendly update: %u seconds\n",
             exec->nextUnfriendlyExpiry - ts.tv_sec);
    }

    cmdf(context, "  Current # STAs blacklisted: %u\n",
         exec->numBlacklist);

    if (exec->numBlacklist > 0 && exec->config.blacklistTime > 0) {
        cmdf(context, "    Next blacklist update: %u seconds\n",
             exec->nextBlacklistExpiry - ts.tv_sec);
    }

    cmdf(context, "\nPer STA information:\n");
    cmdf(context, "%-20s%-15s%-20s%-15s%-15s%-15s%-15s%-15s\n",
         "MAC", "Target band", "Secs since steered", "# Auth Rej",
         "Prohibited", "Unfriendly", "T_Steer", "Blacklist band");
}

/**
 * @brief Convert band to string for output
 *
 * @param [in] band  the band specified
 *
 * @return  the string expression
 */
static const char *
steerexecLegacyConvertBandToStr(wlanif_band_e band) {
    const char *bandStr;
    switch (band) {
        case wlanif_band_24g:
            bandStr = "2.4 GHz";
            break;

        case wlanif_band_5g:
            bandStr = "5 GHz";
            break;

        default:
            bandStr = "NA";
            break;
    }

    return bandStr;
}

void steerexecLegacyDumpEntryState(struct cmdContext *context,
                                   steerexecLegacyHandle_t exec,
                                   stadbEntry_handle_t entry,
                                   LBD_BOOL inProgressOnly) {
    steerexecLegacySteeringState_t *state = stadbEntry_getSteeringState(entry);
    if (state &&
        (!inProgressOnly || state->targetBand != wlanif_band_invalid)) {
        const struct ether_addr *staAddr = stadbEntry_getAddr(entry);
        assert(staAddr);

        cmdf(context, lbMACAddFmt(":") "   ",
             lbMACAddData(staAddr->ether_addr_octet));

        const char *targetBandStr = steerexecLegacyConvertBandToStr(
                                        state->targetBand);

        cmdf(context, "%-15s", targetBandStr);

        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        cmdf(context, "%-20d%-15u", ts.tv_sec - state->lastSteeringTime,
             state->numAuthRejects);

        cmdf(context, "%-15s%-15s",
             state->steeringProhibited ? "yes" : "no",
             state->steeringUnfriendly ? "yes" : "no");

        unsigned tSteeringRemaining;
        if (evloopTimeoutRemaining(&state->tSteerTimer, &tSteeringRemaining,
                                   NULL) == 0) {
            cmdf(context, "%-15d", tSteeringRemaining);
        } else {
            cmdf(context, "%-15c", ' ');
        }

        const char *blacklistBandStr;
        if (state->targetBand == wlanif_band_invalid) {
            blacklistBandStr = steerexecLegacyConvertBandToStr(state->blacklistBand);
        } else {
            // Do not show blacklist band when steering is in progress to prevent confusion
            blacklistBandStr = "NA";
        }
        cmdf(context, "%-15s", blacklistBandStr);

        cmdf(context, "\n");
    }
}

#endif /* LBD_DBG_MENU */
