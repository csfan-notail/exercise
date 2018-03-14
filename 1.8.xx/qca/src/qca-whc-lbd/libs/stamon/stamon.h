// vim: set et sw=4 sts=4 cindent:
/*
 * @File: stamon.h
 *
 * @Abstract: Public interface for the station monitor
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

#ifndef stamon__h
#define stamon__h

#include "lbd.h"  // for LBD_STATUS
#include "wlanif.h"  // for wlanif_band_e

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Initialize the station monitor module.
 *
 * @pre stadb must have been initialized first
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS stamon_init(void);

/**
 * @brief Deinitialize the station monitor module
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS stamon_fini(void);

// ====================================================================
// Constants needed by test cases
// ====================================================================

// These need not be exposed but it is useful to do so for unit tests to
// avoid duplicating the strings.

#define STAMON_RSSI_MEASUREMENT_NUM_SAMPLES_W2_KEY "RSSIMeasureSamples_W2"
#define STAMON_RSSI_MEASUREMENT_NUM_SAMPLES_W5_KEY "RSSIMeasureSamples_W5"
#define STAMON_INACT_RSSI_THRESHOLD_W2_KEY "InactRSSIXingThreshold_W2"
#define STAMON_INACT_RSSI_THRESHOLD_W5_KEY "InactRSSIXingThreshold_W5"
#define STAMON_RSSI_DIFF_EST_W5_FROM_W2_KEY "RSSIDiff_EstW5FromW2"
#define STAMON_RSSI_DIFF_EST_W2_FROM_W5_KEY "RSSIDiff_EstW2FromW5"
#define STAMON_RSSI_AGE_LIMIT_KEY "RSSIAgeLimit"
#define STAMON_PROBE_COUNT_THRESHOLD_KEY "ProbeCountThreshold"

#if defined(__cplusplus)
}
#endif

#endif // stamon__h

