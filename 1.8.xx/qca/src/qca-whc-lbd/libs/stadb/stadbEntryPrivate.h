// vim: set et sw=4 sts=4 cindent:
/*
 * @File: stadbEntryPrivate.h
 *
 * @Abstract: Definition for the STA database entry type. This file should not
 *            be used outside of the stadb module.
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

#ifndef stadbEntryPrivate__h
#define stadbEntryPrivate__h

#include <net/ethernet.h>

#include "list.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief All of the data that is stored for a specific station.
 */
typedef struct stadbEntryPriv_t {
    /// Doubly-linked list for use in a given hash table bucket.
    list_head_t hashChain;

    /// The MAC address of the station
    struct ether_addr addr;

    /// The bands on which the STA has been known to operate.
    u_int8_t operatingBands;

    /// Record of the RSSI values seen by the AP for this device on each of
    /// the supported bands.
    struct {
        /// The last RSSI value, or STADBENTRY_INVALID_RSSI if nothing was
        /// obtained yet.
        u_int8_t rssi;

        /// The time value corresponding to the last point at which the RSSI
        /// was updated.
        time_t lastUpdateSecs;
    } latestRSSI[wlanif_band_invalid];

    /// Record of the RSSI values seen by the AP for this device from its
    /// probe requests on each of the supported bands
    struct {
        /// The averaged RSSI value from probe requests, or
        /// STADBENTRY_INVALID_RSSI if nothing was obtained yet
        u_int8_t avgRSSI;

        /// The time value corresponding to the last point at which the RSSI
        /// was updated.
        time_t lastUpdateSecs;

        /// The number of probe RSSI being averaged for the avgRSSI
        u_int8_t rssiCount;
    } probeRSSI[wlanif_band_invalid];

    // Association state.
    struct {
        /// The time of the last association.
        time_t lastAssoc;

        /// Whether the device has been associated ever or not.
        LBD_BOOL hasAssoc;

        /// The band on which the device is currently associated (if any).
        wlanif_band_e band;
    } assoc;

    /// State information related to steering. This should be considered
    /// opaque to all but the steering executor.
    void *steeringState;

    /// Function to invoke to destroy the steering state prior to destroying
    /// the overall entry.
    stadbEntry_steeringStateDestructor_t steeringStateDestructor;

    // Activity state
    struct {
        /// The time of last time activity status change
        time_t lastActUpdate;

        /// Whether the device is active or not
        LBD_BOOL isAct;
    } activity;

    /// Timestamp of the last time the entry was updated.
    time_t lastUpdateSecs;
} stadbEntryPriv_t;

/**
 * @brief Create a new station entry with the provided MAC address.
 *
 * @param [in] addr  the MAC address for the new entry
 *
 * @return  the handle to the new entry, or NULL if it could not be created
 */
stadbEntry_handle_t stadbEntryCreate(const struct ether_addr *addr);

/**
 * @brief Mark whether the STA of the provided entry is associated on the
 *        provided band or not.
 *
 * Also mark this device as being an in network device. If it is disassociated,
 * also mark it as inactive.
 *
 * Note that if the call is for a disassociation and the band does not
 * match what is currently thought to be the associated band, no update
 * is made to the currently associated band.
 *
 * @param [in] handle  the handle of the entry to update
 * @param [in] band  the band on which the assoc/disassoc occurred
 * @param [in] isAssociated  LBD_TRUE if the device is now associated;
 *                           LBD_FALSE if the device is now disassociated
 * @param [in] updateActive  flag indicating if the device should be
 *                           marked as active when it is associated
 * @param [in] emitDiaglog  whether a diagnostic log should be generated
 *                          for this update or not
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS stadbEntryMarkAssociated(stadbEntry_handle_t handle,
                                    wlanif_band_e band,
                                    LBD_BOOL isAssociated,
                                    LBD_BOOL updateActive,
                                    LBD_BOOL emitDiaglog);

/**
 * @brief Mark whether the STA is active or not
 *
 * If it is active, also mark the associated band.
 *
 * @param [in] handle  the handle of the entry to update
 * @param [in] band  the band on which the activity status change occurred
 * @param [in] active  flag indicating if the STA is active or not
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS stadbEntryMarkActive(stadbEntry_handle_t handle,
                                wlanif_band_e band,
                                LBD_BOOL active);

/**
 * @brief Record the latest RSSI value on the given band in the database
 *        entry.
 *
 * @param [in] handle  the handle to the entry to modify
 * @param [in] band  the band on which the RSSI measurement occurred
 * @param [in] rssi  the RSSI measurement
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS stadbEntryRecordRSSI(stadbEntry_handle_t handle,
                                wlanif_band_e band, u_int8_t rssi);
/**
 * @brief Record the latest probe request RSSI value on the given band
 *        in the database entry.
 *
 * @param [in] handle  the handle to the entry to modify
 * @param [in] band  the band on which the RSSI measurement occurred
 * @param [in] rssi  the RSSI measurement
 * @param [in] maxInterval  the number of seconds allowed for this measurement
 *                          to be averaged with previous ones if any
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS stadbEntryRecordProbeRSSI(stadbEntry_handle_t entry,
                                     wlanif_band_e band, u_int8_t rssi,
                                     time_t maxInterval);

/**
 * @brief Destroy the provided entry.
 *
 * @param [in] handle  the handle to the entry to destroy
 */
void stadbEntryDestroy(stadbEntry_handle_t handle);

/**
 * @brief Compute the hash code for the entry.
 *
 * The hash code is derived from the STA's MAC address.
 *
 * @pre addr is valid
 *
 * @param [in] addr  the address for which to compute the hash code
 *
 * @return  the computed hash code
 */
u_int8_t stadbEntryComputeHashCode(const struct ether_addr *addr);

// --------------------------------------------------------------------
// Debug menu dump routines
// --------------------------------------------------------------------

// Optionally include functions for dumping out individual entries in the
// database.
#ifdef LBD_DBG_MENU
struct cmdContext;

/**
 * @brief Print the header corresponding to the entry information that
 *        will be included.
 *
 * @param [in] context  the output context
 */
void stadbEntryPrintHeader(struct cmdContext *context);

/**
 * @brief Print the relevant information for this entry.
 *
 * @param [in] context  the output context
 */
void stadbEntryPrintEntry(const stadbEntry_handle_t handle,
                          struct cmdContext *context);
#endif /* LBD_DBG_MENU */


#if defined(__cplusplus)
}
#endif

#endif
