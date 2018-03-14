// vim: set et sw=4 sts=4 cindent:
/*
 * @File: wlanif.c
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
 *
 */


#include "wlanifPrivate.h"

#include <dbg.h>

wlanif_band_e wlanifMapToBand(struct dbgModule *dbgModule,
                              BSTEERING_BAND band_index) {
    switch (band_index) {
        case BSTEERING_24G:
            return wlanif_band_24g;

        case BSTEERING_5G:
            return wlanif_band_5g;

        default:
            // Fall through for the error case
            break;
    }

    dbgf(dbgModule, DBGERR, "%s: Invalid band index from driver: %u",
         __func__, band_index);
    return wlanif_band_invalid;
}

wlanif_rssiXingDirection_e wlanifMapToRSSIXingDirection(struct dbgModule *dbgModule,
                                                        BSTEERING_RSSI_XING_DIRECTION direction) {
    switch (direction) {
        case BSTEERING_RSSI_UNCHANGED:
            return wlanif_rssi_unchanged;

        case BSTEERING_RSSI_UP:
            return wlanif_rssi_up;

        case BSTEERING_RSSI_DOWN:
            return wlanif_rssi_down;

        default:
            // Fall through for the error case
            break;
    }

    dbgf(dbgModule, DBGERR, "%s: Invalid RSSI crossing direction from driver: %u",
         __func__, direction);
    return wlanif_rssi_xing_invalid;
}

u_int8_t wlanifMapToRSSIMeasurement(u_int8_t rssi) {
    if (rssi == BSTEERING_INVALID_RSSI) {
        return LBD_INVALID_RSSI;
    }
    return rssi;
}
