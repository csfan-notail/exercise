// vim: set et sw=4 sts=4 cindent:
/*
 * @File: wlanifBSteerControl.h
 *
 * @Abstract: Load balancing daemon band steering control interface
 *
 * @Notes: This header should not be included directly by other components
 *         within the load balancing daemon. It should be considered
 *         private to the wlanif module.
 *
 * @@-COPYRIGHT-START-@@
 *
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 * @@-COPYRIGHT-END-@@
 */

#ifndef wlanifBSteerControl__h
#define wlanifBSteerControl__h

#include "lbd.h"
#include "wlanif.h" //wlanif_band_e

#if defined(__cplusplus)
extern "C" {
#endif

// Out of package forward decls.
struct dbgModule;

/* package API */

struct wlanifBSteerControlPriv_t;  // opaque forward declaration
typedef struct wlanifBSteerControlPriv_t * wlanifBSteerControlHandle_t;


/**
 * @brief Initialize the band steering request interface, by resolving
 *        wlan interfaces and parsing the configuration file
 *
 * @param [in] dbgModule  the handle to use for logging
 *
 * @return a handle to the state for this instance, or NULL if it
 *         could not be created
 */
wlanifBSteerControlHandle_t wlanifBSteerControlCreate(struct dbgModule *dbgModule);

/**
 * @brief Enable the band steering feature, but only if both bands indicate
 *        that the VAPs are not active in doing a scan or DFS wait.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to enable the band steering feature
 * @param [out] enabled  whether band steering was enabled
 *
 * @return LBD_OK if band steering was enabled or a timer was started to
 *         enable it later; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlEnableWhenReady(
        wlanifBSteerControlHandle_t state, LBD_BOOL *enabled);

/**
 * @brief Disable band steering feature on all bands.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to disable band steering feature
 *
 * @return LBD_OK on successfully enable/disable; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlDisable(wlanifBSteerControlHandle_t handle);

/**
 * @brief Destroy the band steering control interface
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to destroy
 *
 * @return LBD_OK if it was successfully destroyed; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlDestroy(wlanifBSteerControlHandle_t handle);

 /**
 * @brief Set overload status on a band
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to set overload status
 * @param [in] band  The band on which to set overload status
 * @param [in] overload  LBD_TRUE for overload, LBD_FALSE for not overload
 *
 * @return LBD_OK on successfully set overload; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlSetOverload(wlanifBSteerControlHandle_t handle,
                                           wlanif_band_e band,
                                           LBD_BOOL overload);

/**
 * @brief Convert the system index from the driver into a band.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to resolve band from system index
 * @param [in] index  the system interface index
 *
 * @return  the resolved band, or wlanif_band_invalid
 */
wlanif_band_e wlanifBSteerControlResolveBandFromSystemIndex(wlanifBSteerControlHandle_t handle,
                                                            int index);

/**
 * @brief Update the link status for the provided interface and indicate
 *        whether it has changed.
 *
 * @pre changed is a valid pointer.
 * @pre The sysIndex parameter corresponds to a VAP being managed by this.
 *      If it does not, this will be a nop.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to update the internal state
 * @param [in] sysIndex  the system interface index
 * @param [in] ifaceUp  whether the interface is currently considered up
 * @param [out] changed  whether the link state was changed or not
 */
void wlanifBSteerControlUpdateLinkState(wlanifBSteerControlHandle_t handle,
                                        int sysIndex, LBD_BOOL ifaceUp,
                                        LBD_BOOL *changed);

/**
 * @brief For each of the VAPs, dump the associated STAs and invoke the
 *        callback with each STA MAC address and the band on which it
 *        is associated.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to dump the associated STAs
 * @param [in] callback  the callback to invoke with the associated STA
 *                       information
 * @param [in] cookie  the parameter to provide in the callback (in addition
 *                     to the STA information) for use by the caller of this
 *                     function
 *
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlDumpAssociatedSTAs(wlanifBSteerControlHandle_t handle,
                                                 wlanif_associatedSTAsCB callback,
                                                 void *cookie);

/**
 * @brief Request real-time RSSI measurement of a specific station
 *
 * The RSSI measurement will be reported back in wlanif_event_rssi_measurement.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to request RSSI measurement
 * @param [in] band  the band that the client is associated with
 * @param [in] staAddr  the MAC address of the specific station
 * @param [in] numSamples  number of RSSI samples to average before reporting RSSI back
 *
 * @return  LBD_OK if the request is sent successfully; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlRequestStaRSSI(wlanifBSteerControlHandle_t handle,
                                             wlanif_band_e band,
                                             const struct ether_addr * staAddr,
                                             u_int8_t numSamples);

/**
 * @brief Enable/disable probe response withholding for a specific STA.
 *
 * @pre The band must have been disabled for the STA first before this is
 *      called.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use to request RSSI measurement
 * @param [in] band  the band on which to withhold probe responses
 * @param [in] staAddr  the MAC address of the specific station
 * @param [in] withholdProbes  the flag for whether to withhold probe
 *                             responses or not
 *
 * @return  LBD_OK if the request is sent successfully; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlSetProbeRespWHForSTA(
        wlanifBSteerControlHandle_t handle, wlanif_band_e band,
        const struct ether_addr * staAddr, LBD_BOOL withholdProbes);

/**
 * @brief Record that the STA should not be allowed to associated on the
 *        provided band.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 * @param [in] band  the band on which the STA should not be allowed to
 *                   associate
 * @param [in] staAddr  the MAC address of the STA
 *
 * @return LBD_OK if the prohibition for the STA on the band was added;
 *         otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlDisableBandForSTA(
        wlanifBSteerControlHandle_t handle, wlanif_band_e band,
        const struct ether_addr *staAddr);

/**
 * @brief Record that the STA should be allowed to associated on the
 *        provided band.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 * @param [in] band  the band on which the STA should be allowed to
 *                   associate
 * @param [in] staAddr  the MAC address of the STA
 *
 * @return LBD_OK if the band was updated to allow the STA to associated;
 *         otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlEnableBandForSTA(
        wlanifBSteerControlHandle_t handle, wlanif_band_e band,
        const struct ether_addr *staAddr);

/**
 * @brief Kick the STA out of the provided band, forcing disassociation.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 * @param [in] band  the band on which the STA should be disassociated
 * @param [in] staAddr the MAC address of the STA to disassociate
 *
 * @return LBD_OK if the request to disassociate was successfully handled;
 *         otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlDisassociateStaOnBand(
        wlanifBSteerControlHandle_t handle, wlanif_band_e band,
        const struct ether_addr *staAddr);

/**
 * @brief Restart the process in the driver that monitors the channel
 *        utilization on each band.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 *
 * @return LBD_OK if the overall restart was successful; otherwise LBD_NOK
 */
LBD_STATUS wlanifBSteerControlRestartChannelUtilizationMonitoring(
        wlanifBSteerControlHandle_t state);

/**
 * @brief Handle RSSI measurement update.
 *
 * If it is for the pending STA, the STA will be removed from pending
 * list and the next queued STA can send the request. Otherwise, it
 * will be ignored.
 *
 * @param [in] handle  the handle returned from wlanifBSteerControlCreate()
 *                     to use for this operation
 * @param [in] band  the band on which the RSSI measurement is received
 * @param [in] staAddr  the MAC address of the STA
 */
void wlanifBSteerControlHandleRSSIMeasurement(
        wlanifBSteerControlHandle_t handle, wlanif_band_e band,
        const struct ether_addr *staAddr);

/*========================================================================*/
/*============ Constants needed by test cases ============================*/
/*========================================================================*/

// Note that these are #define's instead of global constants, as the latter
// cannot be used in the initializer list for the profileElement array that
// specifies default values.
#define WLANIFBSTEERCONTROL_WLAN_INTERFACES "WlanInterfaces"
#define WLANIFBSTEERCONTROL_INACT_IDLE_THRESHOLD "InactIdleThreshold"
#define WLANIFBSTEERCONTROL_INACT_OVERLOAD_THRESHOLD "InactOverloadThreshold"
#define WLANIFBSTEERCONTROL_INACT_CHECK_INTERVAL "InactCheckInterval"
#define WLANIFBSTEERCONTROL_INACT_RSSI_XING_THRESHOLD "InactRSSIXingThreshold"
#define WLANIFBSTEERCONTROL_LOW_RSSI_XING_THRESHOLD "LowRSSIXingTHreshold"
#define WLANIFBSTEERCONTROL_MU_AVG_PERIOD "MUAvgPeriod"
#define WLANIFBSTEERCONTROL_MU_CHECK_INTERVAL "MUCheckInterval"

// When VAPs are not ready upon first check, how frequently (in seconds) to
// check them for becoming ready.
#define VAP_READY_CHECK_PERIOD 10

#if defined(__cplusplus)
}
#endif

#endif  // wlanifBSteerControl__h
