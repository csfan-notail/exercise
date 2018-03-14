// vim: set et sw=4 sts=4 cindent:
/*
 * @File: stadbEntry.h
 *
 * @Abstract: A single entry in the station database, corresponding to a known
 *            Wi-Fi STA
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

#ifndef stadbEntry__h
#define stadbEntry__h

#include "lbd.h"  // for LBD_STATUS
#include "wlanif.h"  // for wlanif_band_e

#if defined(__cplusplus)
extern "C" {
#endif

struct stadbEntryPriv_t;  // opaque forward declaration
typedef struct stadbEntryPriv_t *stadbEntry_handle_t;

/**
 * @brief Function to invoke to request that the steering state be destroyed
 *        prior to the destruction of the entry itself.
 *
 * @param [in] state  the steering state corresponding to the entry being
 *                    destroyed
 */
typedef void (*stadbEntry_steeringStateDestructor_t)(void *state);

/**
 * @brief Obtain the MAC address for the provided station entry.
 *
 * @param [in] handle  the handle to the entry for which to obtain the address
 *
 * @return  the MAC address of the entry, or NULL if the entry is invalid
 */
const struct ether_addr *stadbEntry_getAddr(const stadbEntry_handle_t handle);

/**
 * @brief Determine if the provided entry matches the MAC address given.
 *
 * @param [in] handle  the handle to the entry to compare
 * @param [in] addr  the MAC address to compare to
 *
 * @return LBD_TRUE if the addresses match; otherwise LBD_FALSE
 */
LBD_BOOL stadbEntry_isMatchingAddr(const stadbEntry_handle_t handle,
                                   const struct ether_addr *addr);

/**
 * @brief Determine whether the band provided is supported or not for the
 *        given STA.
 *
 * @param [in] handle  the handle to the entry to check
 * @param [in] band  the band for which to check
 *
 * @return LBD_TRUE if the band is supported; otherwise LBD_FALSE
 */
LBD_BOOL stadbEntry_isBandSupported(const stadbEntry_handle_t handle,
                                    wlanif_band_e band);

/**
 * @brief Determine whether the entry provided supports both the 2.4 GHz
 *        and 5 GHz bands.
 *
 * @param [in] handle  the handle to the entry to check
 *
 * @return LBD_TRUE if both bands are supported; otherwise LBD_FALSE
 */
LBD_BOOL stadbEntry_isDualBand(const stadbEntry_handle_t handle);

/**
 * @brief Determine the band on which the device is associated, and
 *        optionally how long ago that occurred.
 *
 * @param [in] handle  the handle of the entry to query
 * @param [out] deltaSecs  the number of seconds that have elapsed since
 *                         the device last associated
 *
 * @return the band on which it is associated, or wlanif_band_invalid if the
 *         device is not currently associated or the handle is invalid
 */
wlanif_band_e stadbEntry_getAssociatedBand(const stadbEntry_handle_t handle,
                                           time_t *deltaSecs);

/**
 * @brief Determine if the device ever associated (and thus should be
 *        considered an in-network device).
 *
 * @param [in] handle  the handle of the entry to query
 *
 * @return LBD_TRUE if the device has been associated; otherwise LBD_FALSE
 */
LBD_BOOL stadbEntry_isInNetwork(const stadbEntry_handle_t handle);

/**
 * @brief Obtain the latest RSSI reading for the given STA on the given
 *        band.
 *
 * Optionally also get the number of seconds that have elapsed since the
 * last RSSI measurement.
 *
 * @param [in] handle  the handle of the entry from which to retrieve the RSSI
 * @param [in] band  the band on which to obtain the RSSI
 * @param [out] deltaSecs  if non-NULL, on success this will contain the number
 *                         of seconds that have elapsed since the last RSSI
 *                         measurement
 *
 * @return the last RSSI measurement, or INVALID_RSSI on failure
 */
u_int8_t stadbEntry_getRSSI(const stadbEntry_handle_t handle,
                            wlanif_band_e band, time_t *deltaSecs);

/**
 * @brief Determine how old the entry is (where age is defined as the number
 *        of seconds since it was last updated).
 *
 * @param [in] handle  the handle for which to obtain the age
 * @param [out] ageSecs  the age of the entry, in seconds
 *
 * @return LBD_OK if the entry was found and the age is valid; otherwise
 *         LBD_NOK
 */
LBD_STATUS stadbEntry_getAge(const stadbEntry_handle_t handle, time_t *ageSecs);

/**
 * @brief Obtain the opaque handle stored previously as the steering state
 *        (if there was one).
 *
 * @param [in] handle  the handle of the entry for which to get the steering
 *                     state
 *
 * @return the steering state, or NULL if none has been stored (or the entry
 *         handle is invalid)
 */
void *stadbEntry_getSteeringState(stadbEntry_handle_t handle);

/**
 * @brief Store an opaque steering state pointer in the entry for later lookup.
 *
 * @param [in] handle  the handle of the entry for which to get the steering
 *                     state
 * @param [in] state  the state to store
 * @param [in] destructor  the destructor function to use when cleaning up
 *                         the entry
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS stadbEntry_setSteeringState(
        stadbEntry_handle_t handle, void *state,
        stadbEntry_steeringStateDestructor_t destructor);

/**
 * @brief Get activity status of a STA
 *
 * @param [in] handle  the handle for which to check idle status
 * @param [out] active  on success this will contain the activity status of this STA
 * @param [out] deltaSecs  if non-NULL, on success this will contain the number
 *                         of seconds that have elapsed since the last time activity
 *                         status is recorded
 *
 * @return LBD_NOK if the parameters are invalid or the STA is not associated;
 *         otherwise LBD_OK
 */
LBD_STATUS stadbEntry_getActStatus(const stadbEntry_handle_t entry, LBD_BOOL *active, time_t *deltaSecs);

/**
 * @brief Obtain the averaged RSSI reading of probe requests for the given STA
 *        on the given band.
 *
 * Optionally also get the number of seconds that have elapsed since the
 * last probe request RSSI measurement and the number of probes averaged.
 *
 * @param [in] handle  the handle of the entry from which to retrieve the RSSI
 * @param [in] band  the band on which to obtain the RSSI
 * @param [out] deltaSecs  if non-NULL, on success this will contain the number
 *                         of seconds that have elapsed since the last RSSI
 *                         measurement
 * @param [out] probeCount  if non-NULL, on success this will contain the number
 *                          of probes avaraged.
 *
 * @return the averaged RSSI measurement, or INVALID_RSSI on failure
 */
u_int8_t stadbEntry_getProbeRSSI(const stadbEntry_handle_t handle,
                                 wlanif_band_e band, time_t *deltaSecs,
                                 u_int8_t *probeCount);

#if defined(__cplusplus)
}
#endif

#endif
