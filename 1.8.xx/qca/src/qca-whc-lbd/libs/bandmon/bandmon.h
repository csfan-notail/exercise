// vim: set et sw=4 sts=4 cindent:
/*
 * @File: bandmon.h
 *
 * @Abstract: Public interface for the band monitor
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

#ifndef bandmon__h
#define bandmon__h

#include "lbd.h"  // for LBD_STATUS
#include "wlanif.h"  // for wlanif_band_e

#if defined(__cplusplus)
extern "C" {
#endif

// ====================================================================
// Common types exported to other modules
// ====================================================================

/**
 * @brief IDs of events that are generated from this module.
 */
typedef enum bandmon_event_e {
    bandmon_event_overload_change,  ///< Overload status change

    bandmon_event_maxnum
} bandmon_event_e;

/**
 * @brief The format of the bandmon_event_overload_change event
 */
typedef struct bandmon_overloadChangeEvent_t {
    /// Overload status for 2.4 GHz band
    LBD_BOOL overload24g;

    /// Overload status for 5 GHz band
    LBD_BOOL overload5g;
} bandmon_overloadChangeEvent_t;

// ====================================================================
// Public API
// ====================================================================

/**
 * @brief Initialize the band monitor module.
 *
 * @pre stadb must have been initialized first
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS bandmon_init(void);

/**
 * @brief Query whether the band is overloaded or not.
 *
 * @param [in] band  the band for which to check for overload
 *
 * @return LBD_TRUE if the band is overloaded; otherwise LBD_FALSE
 */
LBD_BOOL bandmon_isOverloaded(wlanif_band_e band);

/**
 * @brief Terminate the band monitor module.
 *
 * @pre stadb must still be initialized
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
LBD_STATUS bandmon_fini(void);

// ====================================================================
// Constants needed by test cases
// ====================================================================

// These need not be exposed but it is useful to do so for unit tests to
// avoid duplicating the strings.

#define BANDMON_MU_SLIGHT_OVERLOAD_THRESHOLD_W2_KEY "MUSlightOverloadThreshold_W2"
#define BANDMON_MU_HEAVY_OVERLOAD_THRESHOLD_W2_KEY  "MUHeavyOverloadThreshold_W2"
#define BANDMON_MU_SLIGHT_OVERLOAD_THRESHOLD_W5_KEY "MUSlightOverloadThreshold_W5"
#define BANDMON_MU_HEAVY_OVERLOAD_THRESHOLD_W5_KEY  "MUHeavyOverloadThreshold_W5"
#define BANDMON_MU_RESERVE_W5_KEY                   "MUReserve_W5"
#define BANDMON_RSSI_MAX_AGE_KEY                    "RSSIMaxAge"
#define BANDMON_RSSI_THRESHOLD_W5_KEY               "RSSIThreshold_W5"
#define BANDMON_PROBE_COUNT_THRESHOLD_KEY           "ProbeCountThreshold"

#if defined(LBD_DBG_MENU) && defined(GMOCK_UNIT_TESTS)
struct cmdContext;

/**
 * @brief Print the status of the band monitor module.
 *
 * @param [in] context  the output context
 * @param [in] cmd  the command in the debug CLI
 */
void bandmonMenuStatusHandler(struct cmdContext *context, const char *cmd);

/**
 * @brief Enable/disable the debug mode from the debug CLI.
 *
 * @param [in] context  the output context
 * @param [in] cmd  the command in the debug CLI
 */
void bandmonMenuDebugHandler(struct cmdContext *context, const char *cmd);

/**
 * @brief Inject a utilization measurement from the debug CLI.
 *
 * @param [in] context  the output context
 * @param [in] cmd  the command in the debug CLI
 */
void bandmonMenuUtilHandler(struct cmdContext *context, const char *cmd);

#endif /* LBD_DBG_MENU */

#if defined(__cplusplus)
}
#endif

#endif
