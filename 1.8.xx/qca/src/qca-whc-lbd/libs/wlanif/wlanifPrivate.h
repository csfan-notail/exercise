// vim: set et sw=4 sts=4 cindent:
/*
 * @File: wlanifPrivate.h
 *
 * @Abstract: Private helpers for load balancing daemon WLAN interface
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

#ifndef wlanifPrivate__h
#define wlanifPrivate__h

#include "wlanif.h"

#include <ieee80211_band_steering_api.h>

// Forward decls
struct dbgModule;

// ====================================================================
// Helper functions private to this wlanif module
// ====================================================================

/**
 * @brief Convert the band index received from the driver into the enum
 *        value used within LBD.
 *
 * @param [in] dbgModule  the handle to use when logging errors
 * @param [in] bandIndex  the index of the band as indicated by the driver
 *
 * @return  the mapped band index
 */
wlanif_band_e wlanifMapToBand(struct dbgModule *dbgModule,
                              BSTEERING_BAND bandIndx);

/**
 * @brief Map a frequency value (as defined by the kernel) to a band.
 *
 * @param [in] freq  the frequency value given by the kernel
 */
wlanif_band_e wlanifMapFreqToBand(int32_t freq);

/**
 * @brief Convert the system index from the driver into a band.
 *
 * This will be based on the last detected band of operation for that
 * interface.
 *
 * @param [in] index  the system interface index
 *
 * @return  the resolved band, or wlanif_band_invalid
 */
wlanif_band_e wlanifResolveBandFromSystemIndex(int index);

/**
 * @brief Convert the RSSI crossing direction received from driver into
 *        the enum valus used within LBD
 *
 * @param [in] dbgModule  the handle to use when logging errors
 * @param [in] direction  the RSSI crossing direction as indicated by the driver
 *
 * @return  the mapped RSSI crossing direction
 */
wlanif_rssiXingDirection_e wlanifMapToRSSIXingDirection(struct dbgModule *dbgModule,
                                                        BSTEERING_RSSI_XING_DIRECTION direction);

/**
 * @brief Convert the RSSI measurement from driver into values used in LBD
 *
 * @param [in] rssi  the RSSI measurement received from driver
 *
 * @return the mapped RSSI value
 */
u_int8_t wlanifMapToRSSIMeasurement(u_int8_t rssi);
#endif


