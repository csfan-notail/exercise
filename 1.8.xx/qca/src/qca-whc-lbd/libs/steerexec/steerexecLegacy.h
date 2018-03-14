// vim: set et sw=4 sts=4 cindent:
/*
 * @File: steerexecLegacy.h
 *
 * @Abstract: Package level interface to the steering executor for legacy
 *            clients
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
 */

#ifndef steerexecLegacy__h
#define steerexecLegacy__h

#include "lbd.h"  // for LBD_STATUS

#include "steerexec.h"
#include "stadbEntry.h"
#include "wlanif.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct steerexecLegacyConfig_t {
    /// Amount of time between successive steerings for the legacy
    /// steering mechanism.
    u_int32_t steeringProhibitTime;

    /// How long to allow for the STA to associate on the target band
    /// before aborting the steering.
    u_int32_t tSteering;

    /// The window during which repeated authentication rejects are counted
    /// as only a single one.
    u_int32_t initialAuthRejCoalesceTime;

    /// The point at which repeated authentications result in the blacklist
    /// being cleared and steering aborted.
    u_int32_t authRejMax;

    /// The amount of time a device should be considered as steering
    /// unfriendly before being attempted again.
    u_int32_t steeringUnfriendlyTime;

    /// The amount of time a device should be blacklisted on one band
    /// before being removed.
    u_int32_t blacklistTime;

    /// RSSI threshold indicating poor signal strength
    u_int8_t lowRSSIXingThreshold[wlanif_band_invalid];

    /// RSSI threshold indicating the target band is not strong enough
    /// for association
    u_int8_t targetLowRSSIThreshold[wlanif_band_invalid];
} steerexecLegacyConfig_t;

struct steerexecLegacyPriv_t;
typedef struct steerexecLegacyPriv_t *steerexecLegacyHandle_t;

/**
 * @brief Create the steering executor for legacy clients.
 *
 * @param [in] config  the configuration parameters needed by legacy
 *                     steering executor
 * @param [in] dbgModule  the area to use for log messages
 *
 * @return a handle to the executor instance, or NULL if creation failed
 */
steerexecLegacyHandle_t steerexecLegacyCreate(const steerexecLegacyConfig_t *config,
                                              struct dbgModule *dbgModule);

/**
 * @brief Orchestrate the steering to the specified band for the given STA.
 *
 * This may be called for both pre-association and post-association steering.
 * It will perform the necessary actions to do the steering in both cases.
 *
 * If steering is already being performed and the target is the same, this
 * will be a nop. If the target is not the same, the in progress steering
 * will be terminated and no steering will be performed.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] entry  the handle to the STA to steer
 * @param [in] targetBand  the target band for the steering
 * @param [out] ignored  if the request was ignored by the executor, this
 *                       will be set to LBD_TRUE; otherwise it will be set
 *                       to LBD_FALSE indicating it was acted upon by the
 *                       executor; this parameter may be NULL if the caller
 *                       does not care to distinguish between ignored and
 *                       non-ignored requests
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexecLegacySteerToBand(steerexecLegacyHandle_t exec,
                                      stadbEntry_handle_t entry,
                                      wlanif_band_e targetBand,
                                      LBD_BOOL *ignored);

/**
 * @brief Abort any steering operation which may be in progress for the STA.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] entry  the handle to the STA for which to abort
 * @param [in] targetBand  the band that had been targeted for steering;
 *                         wlanif_band_invalid may be specified to abort
 *                         steering for any band
 * @param [out] ignored  if the request was ignored by the executor, this
 *                       will be set to LBD_TRUE; otherwise it will be set
 *                       to LBD_FALSE indicating it was acted upon by the
 *                       executor; this parameter may be NULL if the caller
 *                       does not care to distinguish between ignored and
 *                       non-ignored requests
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexecLegacyAbortSteerToBand(steerexecLegacyHandle_t exec,
                                           stadbEntry_handle_t entry,
                                           wlanif_band_e targetBand,
                                           LBD_BOOL *ignored);

/**
 * @brief Inform the legacy client steering executor of an update on the
 *        association for a given STA.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] entry  the handle to the STA which was updated
 */
void steerexecLegacyHandleAssocUpdate(steerexecLegacyHandle_t exec,
                                      stadbEntry_handle_t entry);

/**
 * @brief Register a function to get called back when an entry can be
 *        steered again (after previously not being allowed).
 *
 * @param [in] exec  the executor instance to use
 * @param [in] callback  the callback function to invoke
 * @param [in] cookie  the parameter to pass to the callback function
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexecLegacyRegisterSteeringAllowedObserver(
        steerexecLegacyHandle_t exec,
        steerexec_steeringAllowedObserverCB callback,
        void *cookie);

/**
 * @brief Unregister the observer callback function.
 *
 * @param [in] exec  the executor instance to use
 * @param [in] callback  the callback function to unregister
 * @param [in] cookie  the registered parameter to pass to the callback
 *                     function
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexecLegacyUnregisterSteeringAllowedObserver(
        steerexecLegacyHandle_t exec,
        steerexec_steeringAllowedObserverCB callback,
        void *cookie);

/**
 * @brief Destroy the legacy steering executor.
 *
 * @param [in] exec  the legacy steering executor instance to destroy
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
void steerexecLegacyDestroy(steerexecLegacyHandle_t exec);

#ifdef GMOCK_UNIT_TESTS
/**
 * @brief Check if a device is steering unfriendly
 */
LBD_BOOL steerexecLegacyIsSTASteeringUnfriendly(stadbEntry_handle_t entry);
#endif

#ifdef LBD_DBG_MENU
struct cmdContext;

/**
 * @brief Dump the overall legacy executor information along with the
 *        header for the individual entries.
 *
 * @param [in] context  the output context
 * @param [in] exec  the executor instance to use
 */
void steerexecLegacyDumpHeader(struct cmdContext *context,
                               steerexecLegacyHandle_t exec);

/**
 * @brief Dump the legacy steering state for a single entry.
 *
 * @param [in] context  the output context
 * @param [in] exec  the executor instance to use
 * @param [in] entry  the entry to dump
 * @param [in] inProgressOnly  flag indicating whether to only dump entries
 *                             that are currently being steered
 */
void steerexecLegacyDumpEntryState(struct cmdContext *context,
                                   steerexecLegacyHandle_t exec,
                                   stadbEntry_handle_t entry,
                                   LBD_BOOL inProgressOnly);

#endif /* LBD_DBG_MENU */

#if defined(__cplusplus)
}
#endif

#endif
