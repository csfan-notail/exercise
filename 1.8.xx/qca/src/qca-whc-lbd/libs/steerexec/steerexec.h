// vim: set et sw=4 sts=4 cindent:
/*
 * @File: steerexec.h
 *
 * @Abstract: Public interface for the steering executor
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

#ifndef steerexec__h
#define steerexec__h

#include "lbd.h"  // for LBD_STATUS

#include "stadbEntry.h"
#include "wlanif.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Function callback type that other modules can register to be
 *        informed when steering becomes allowed for a given entry.
 *
 * The callback occurs after the entry has been updated.
 *
 * @param [in] handle  the entry that was updated
 * @param [in] cookie  the value provided by the caller when the observer
 *                     callback function was registered
 */
typedef void (*steerexec_steeringAllowedObserverCB)(stadbEntry_handle_t handle,
                                                    void *cookie);

/**
 * @brief Initialize the steering executor module.
 *
 * This should be called prior to intializing the station database so that
 * the steering executor can be notified of associated stations.
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexec_init(void);

/**
 * @brief Orchestrate the steering to the specified band for the given STA.
 *
 * This may be called for both pre-association and post-association steering.
 * It will perform the necessary actions to do the steering in both cases.
 *
 * If steering is already being performed, the behavior of this function will
 * depend on whether the target is the same or not. If the target is the same,
 * this operation will silently succeed. If the target is not the same, the
 * behavior depends on the underlying steering mechanism. Some mechanisms
 * will not support steering to a new band when steering is in progress. In
 * such a case, they may just cancel the steering and let the STA associate
 * wherever it chooses.
 *
 * @param [in] handle  the handle to the STA to steer
 * @param [in] band  the target band for the steering
 * @param [out] ignored  if the request was ignored by the executor, this
 *                       will be set to LBD_TRUE; otherwise it will be set
 *                       to LBD_FALSE indicating it was acted upon by the
 *                       executor; this parameter may be NULL if the caller
 *                       does not care to distinguish between ignored and
 *                       non-ignored requests
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexec_steerToBand(stadbEntry_handle_t handle,
                                 wlanif_band_e band,
                                 LBD_BOOL *ignored);

/**
 * @brief Abort any steering operation which may be in progress for the STA.
 *
 * @param [in] handle  the handle to the STA for which to abort
 * @param [in] band  the band which was the target for an in progress
 *                   steering operation
 * @param [out] ignored  if the request was ignored by the executor, this
 *                       will be set to LBD_TRUE; otherwise it will be set
 *                       to LBD_FALSE indicating it was acted upon by the
 *                       executor; this parameter may be NULL if the caller
 *                       does not care to distinguish between ignored and
 *                       non-ignored requests
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexec_abortSteerToBand(stadbEntry_handle_t handle,
                                      wlanif_band_e band,
                                      LBD_BOOL *ignored);

/**
 * @brief Register a function to get called back when an entry can be
 *        steered again (after previously not being allowed).
 *
 * @param [in] exec  the executor instance to use
 * @param [in] observer  the callback function to invoke
 * @param [in] cookie  the parameter to pass to the callback function
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexec_registerSteeringAllowedObserver(
        steerexec_steeringAllowedObserverCB observer,
        void *cookie);

/**
 * @brief Unregister the observer callback function.
 *
 * @param [in] observer  the callback function to unregister
 * @param [in] cookie  the registered parameter to pass to the callback
 *                     function
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexec_unregisterSteeringAllowedObserver(
        steerexec_steeringAllowedObserverCB observer,
        void *cookie);

/**
 * @brief Tear down the steering executor module.
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS steerexec_fini(void);


// ====================================================================
// Constants needed by test cases
// ====================================================================

// These need not be exposed but it is useful to do so for unit tests to
// avoid duplicating the strings.

#define STEEREXEC_STEERING_PROHIBIT_TIME_KEY   "SteeringProhibitTime"
#define STEEREXEC_T_STEERING_KEY               "TSteering"
#define STEEREXEC_INITIAL_AUTH_REJ_COALESCE_TIME_KEY "InitialAuthRejCoalesceTime"
#define STEEREXEC_AUTH_REJ_MAX_KEY             "AuthRejMax"
#define STEEREXEC_STEERING_UNFRIENDLY_TIME_KEY "SteeringUnfriendlyTime"
#define STEEREXEC_LOW_RSSI_THRESHOLD_W2_KEY "LowRSSIXingThreshold_W2"
#define STEEREXEC_LOW_RSSI_THRESHOLD_W5_KEY "LowRSSIXingThreshold_W5"
#define STEEREXEC_TARGET_LOW_RSSI_THRESHOLD_W2_KEY "TargetLowRSSIThreshold_W2"
#define STEEREXEC_TARGET_LOW_RSSI_THRESHOLD_W5_KEY "TargetLowRSSIThreshold_W5"
#define STEEREXEC_BLACKLIST_TIME_KEY "BlacklistTime"

#if defined(LBD_DBG_MENU) && defined(GMOCK_UNIT_TESTS)
struct cmdContext;

/**
 * @brief Print the status of the steering executor module.
 *
 * @param [in] context  the output context
 * @param [in] cmd  the command in the debug CLI
 */
void steerexecMenuStatusHandler(struct cmdContext *context,
                                const char *cmd);

/**
 * @brief Steer a specific STA to a specified band.
 *
 * @param [in] context  the output context
 * @param [in] cmd  the command in the debug CLI
 */
void steerexecMenuSteerHandler(struct cmdContext *context,
                               const char *cmd);

/**
 * @brief Abort the steering for a specific STA.
 *
 * @param [in] context  the output context
 * @parma [in] cmd  the command in the debug CLI
 */
void steerexecMenuAbortHandler(struct cmdContext *context,
                               const char *cmd);

#endif /* LBD_DBG_MENU && GMOCK_UNIT_TESTS */

#if defined(__cplusplus)
}
#endif

#endif
