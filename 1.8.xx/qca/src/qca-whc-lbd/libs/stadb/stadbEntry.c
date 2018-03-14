// vim: set et sw=4 sts=4 cindent:
/*
 * @File: stadbEntry.c
 *
 * @Abstract: Implementation of accessors and mutators for stadbEntry
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
#include <time.h>
#include <assert.h>

#ifdef LBD_DBG_MENU
#include <cmd.h>
#endif

#include "stadbEntry.h"
#include "stadbEntryPrivate.h"
#include "stadbDiaglogDefs.h"

#include "internal.h"
#include "diaglog.h"

// Forward decls
static void stadbEntryMarkBandSupported(stadbEntry_handle_t handle,
                                        wlanif_band_e band);

static time_t stadbEntryGetTimestamp(void);
static void stadbEntryUpdateTimestamp(stadbEntry_handle_t entry);

const struct ether_addr *stadbEntry_getAddr(const stadbEntry_handle_t handle) {
    if (handle) {
        return &handle->addr;
    }

    return NULL;
}

LBD_BOOL stadbEntry_isMatchingAddr(const stadbEntry_handle_t entry,
                                   const struct ether_addr *addr) {
    if (!entry || !addr) {
        return LBD_FALSE;
    }

    return lbAreEqualMACAddrs(entry->addr.ether_addr_octet,
                              addr->ether_addr_octet);
}

LBD_BOOL stadbEntry_isBandSupported(const stadbEntry_handle_t entry,
                                    wlanif_band_e band) {
    if (!entry || band >= wlanif_band_invalid) {
        return LBD_FALSE;
    }

    return (entry->operatingBands & 1 << band) != 0;
}

LBD_BOOL stadbEntry_isDualBand(const stadbEntry_handle_t entry) {
    if (!entry) {
        return LBD_FALSE;
    }

    u_int8_t mask = (1 << wlanif_band_24g | 1 << wlanif_band_5g);
    return (entry->operatingBands & mask) == mask;
}

wlanif_band_e stadbEntry_getAssociatedBand(const stadbEntry_handle_t entry,
                                           time_t *deltaSecs) {
    if (!entry) {
        return wlanif_band_invalid;
    }

    if (deltaSecs && stadbEntry_isInNetwork(entry) &&
        entry->assoc.band != wlanif_band_invalid) {
        time_t curTime = stadbEntryGetTimestamp();
        *deltaSecs = curTime - entry->assoc.lastAssoc;
    }

    return entry->assoc.band;
}

LBD_BOOL stadbEntry_isInNetwork(const stadbEntry_handle_t entry) {
    if (!entry) {
        return LBD_FALSE;
    }

    return entry->assoc.hasAssoc;
}

u_int8_t stadbEntry_getRSSI(const stadbEntry_handle_t entry,
                            wlanif_band_e band, time_t *deltaSecs) {
    if (!entry || band >= wlanif_band_invalid) {
        return LBD_INVALID_RSSI;
    }

    if (deltaSecs && entry->latestRSSI[band].rssi != LBD_INVALID_RSSI) {
        *deltaSecs = stadbEntryGetTimestamp() -
                     entry->latestRSSI[band].lastUpdateSecs;
    }

    return entry->latestRSSI[band].rssi;
}

LBD_STATUS stadbEntry_getAge(const stadbEntry_handle_t entry, time_t *ageSecs) {
    if (!entry || !ageSecs) {
        return LBD_NOK;
    }

    *ageSecs = stadbEntryGetTimestamp() - entry->lastUpdateSecs;
    return LBD_OK;
}

void *stadbEntry_getSteeringState(stadbEntry_handle_t entry) {
    if (entry) {
        return entry->steeringState;
    }

    return NULL;
}

LBD_STATUS stadbEntry_setSteeringState(
        stadbEntry_handle_t entry, void *state,
        stadbEntry_steeringStateDestructor_t destructor) {
    if (!entry || (state && !destructor)) {
        return LBD_NOK;
    }

    entry->steeringState = state;
    entry->steeringStateDestructor = destructor;
    return LBD_OK;
}

LBD_STATUS stadbEntry_getActStatus(const stadbEntry_handle_t entry, LBD_BOOL *active, time_t *deltaSecs) {
    if (!entry || !active) {
        return LBD_NOK;
    }

    if (stadbEntry_getAssociatedBand(entry, NULL) == wlanif_band_invalid) {
        // If an entry is not associated, there is no activity status
        return LBD_NOK;
    }

    if (deltaSecs) {
        time_t curTime = stadbEntryGetTimestamp();
        *deltaSecs = curTime - entry->activity.lastActUpdate;
    }

    *active = entry->activity.isAct;

    return LBD_OK;
}

u_int8_t stadbEntry_getProbeRSSI(const stadbEntry_handle_t entry,
                                 wlanif_band_e band, time_t *deltaSecs,
                                 u_int8_t *probeCount) {
    if (!entry || band >= wlanif_band_invalid) {
        return LBD_INVALID_RSSI;
    }

    if (entry->probeRSSI[band].avgRSSI != LBD_INVALID_RSSI) {
        if (deltaSecs) {
            *deltaSecs = stadbEntryGetTimestamp() -
                         entry->probeRSSI[band].lastUpdateSecs;
        }
        if (probeCount) {
            *probeCount = entry->probeRSSI[band].rssiCount;
        }
    }

    return entry->probeRSSI[band].avgRSSI;
}

// ====================================================================
// "Package" and private helper functions
// ====================================================================

stadbEntry_handle_t stadbEntryCreate(const struct ether_addr *addr) {
    if (!addr) {
        return NULL;
    }

    stadbEntry_handle_t entry = calloc(1, sizeof(stadbEntryPriv_t));
    if (entry) {
        lbCopyMACAddr(addr->ether_addr_octet, entry->addr.ether_addr_octet);
        entry->assoc.band = wlanif_band_invalid;
        stadbEntryUpdateTimestamp(entry);
    }

    return entry;
}

LBD_STATUS stadbEntryRecordRSSI(stadbEntry_handle_t entry,
                                wlanif_band_e band, u_int8_t rssi) {
    if (!entry || band >= wlanif_band_invalid) {
        return LBD_NOK;
    }

    time_t curTime = stadbEntryGetTimestamp();
    stadbEntryMarkBandSupported(entry, band);

    entry->latestRSSI[band].rssi = rssi;
    entry->latestRSSI[band].lastUpdateSecs = curTime;
    entry->lastUpdateSecs = curTime;

    // Since we get a lot of RSSI updates, we don't want to clutter the
    // diaglog stream for those nodes we may not even care about.
    if (stadbEntry_isInNetwork(entry) &&
        diaglog_startEntry(mdModuleID_StaDB,
                           stadb_msgId_rssiUpdate,
                           diaglog_level_demo)) {
        diaglog_writeMAC(&entry->addr);
        diaglog_write8(band);
        diaglog_write8(rssi);
        diaglog_finishEntry();
    }

    return LBD_OK;
}

LBD_STATUS stadbEntryRecordProbeRSSI(stadbEntry_handle_t entry,
                                     wlanif_band_e band, u_int8_t rssi,
                                     time_t maxInterval) {
    if (!entry || band >= wlanif_band_invalid ||
        band == entry->assoc.band) {
        // Ignore probes on the associated band since they present an
        // instantaneous measurement and may not be as accurate as our
        // average RSSI report or triggered RSSI measurement which are
        // both taken over a series of measurements.
        return LBD_NOK;
    }

    time_t curTime = stadbEntryGetTimestamp();
    stadbEntryMarkBandSupported(entry, band);

    if ((curTime - entry->probeRSSI[band].lastUpdateSecs) >
        maxInterval) {
        // Reset probe RSSI averaging
        entry->probeRSSI[band].avgRSSI = rssi;
        entry->probeRSSI[band].lastUpdateSecs = curTime;
        entry->probeRSSI[band].rssiCount = 1;
    } else {
        // Average this one with previous measurements
        entry->probeRSSI[band].avgRSSI =
            (entry->probeRSSI[band].avgRSSI * entry->probeRSSI[band].rssiCount + rssi) /
            (entry->probeRSSI[band].rssiCount + 1);
        entry->probeRSSI[band].lastUpdateSecs = curTime;
        ++entry->probeRSSI[band].rssiCount;
    }

    // Since we get a lot of RSSI updates, we don't want to clutter the
    // diaglog stream for those nodes we may not even care about.
    if (stadbEntry_isInNetwork(entry) &&
        diaglog_startEntry(mdModuleID_StaDB,
                           stadb_msgId_rssiUpdate,
                           diaglog_level_demo)) {
        diaglog_writeMAC(&entry->addr);
        diaglog_write8(band);
        // Report averaged probe RSSI
        diaglog_write8(entry->probeRSSI[band].avgRSSI);
        diaglog_finishEntry();
    }

    return LBD_OK;
}

LBD_STATUS stadbEntryMarkAssociated(stadbEntry_handle_t entry,
                                    wlanif_band_e band,
                                    LBD_BOOL isAssociated,
                                    LBD_BOOL updateActive,
                                    LBD_BOOL emitDiaglog) {
    if (!entry || band >= wlanif_band_invalid) {
        return LBD_NOK;
    }

    wlanif_band_e oldAssocBand = entry->assoc.band;
    time_t curTime = stadbEntryGetTimestamp();
    stadbEntryMarkBandSupported(entry, band);

    if (isAssociated) {
        // Only update the last association time if the band on which we
        // previously thought the STA was associated is out of date/wrong.
        if (entry->assoc.band != band) {
            entry->assoc.lastAssoc = curTime;
        }

        entry->assoc.band = band;
        entry->assoc.hasAssoc = LBD_TRUE;
        if (updateActive) {
            // Also mark entry as active
            entry->activity.isAct = LBD_TRUE;
            entry->activity.lastActUpdate = curTime;
        }
    } else if (band == entry->assoc.band ||
               entry->assoc.band == wlanif_band_invalid) {
        entry->assoc.band = wlanif_band_invalid;

        // An entry that disassociates should be considered in network as it
        // was previously associated. This generally should not happen unless
        // we somehow miss an association event.
        entry->assoc.hasAssoc = LBD_TRUE;

        // Also mark entry as inactive
        entry->activity.isAct = LBD_FALSE;
        entry->activity.lastActUpdate = curTime;
    }

    if (emitDiaglog && oldAssocBand != entry->assoc.band &&
        diaglog_startEntry(mdModuleID_StaDB,
                           stadb_msgId_associationUpdate,
                           diaglog_level_demo)) {
        diaglog_writeMAC(&entry->addr);
        diaglog_write8(entry->assoc.band);
        diaglog_write8(entry->activity.isAct);
        diaglog_write8(stadbEntry_isDualBand(entry));
        diaglog_finishEntry();
    }

    return LBD_OK;
}

void stadbEntryDestroy(stadbEntry_handle_t entry) {
    if (entry && entry->steeringState) {
        assert(entry->steeringStateDestructor);
        entry->steeringStateDestructor(entry->steeringState);
    }

    free(entry);
}

LBD_STATUS stadbEntryMarkActive(stadbEntry_handle_t entry,
                                wlanif_band_e band,
                                LBD_BOOL active) {
    if (!entry || band >= wlanif_band_invalid) {
        return LBD_NOK;
    }

    time_t curTime = stadbEntryGetTimestamp();

    // Only mark the entry as associated if it is being reported as active.
    // If we did it always, we might change the associated band due to the
    // STA having moved from one band to another without cleanly
    // disassociating.
    //
    // For example, if the STA is on 5 GHz and then moves to 2.4 GHz without
    // cleanly disassociating, the driver will still have an activity timer
    // running on 5 GHz. When that expires, if we mark it as associated, we
    // will clobber our current association state (of 2.4 GHz) with this 5 GHz
    // one. This will cause RSSI measurements and steering to be done with the
    // wrong band.
    //
    // Note that we do not update activity status, as it will be done
    // immediately below. We cannot let it mark the activity status as it
    // always sets the status to active for an associated device and inactive
    // for an unassociated device.
    if (active) {
        stadbEntryMarkAssociated(entry, band, LBD_TRUE, /* isAssociated */
                                 LBD_FALSE /* updateActive */,
                                 LBD_FALSE /* emitDiaglog */);
    }

    // Only update the activity if the device is associated, as if it is not,
    // we do not know for sure that it is really a legitimate association
    // (see the note above for reasons).
    if (entry->assoc.band == band) {
        entry->activity.isAct = active;
        entry->activity.lastActUpdate = curTime;

        if (diaglog_startEntry(mdModuleID_StaDB, stadb_msgId_activityUpdate,
                               diaglog_level_demo)) {
            diaglog_writeMAC(&entry->addr);
            diaglog_write8(band);
            diaglog_write8(entry->activity.isAct);
            diaglog_finishEntry();
        }
    }

    return LBD_OK;
}

/**
 * @brief Mark the provided band as being supported by this entry.
 *
 * @param [in] handle  the handle to the entry to modify
 * @param [in] band  the band to enable for the entry
 *
 * @return LBD_OK on success; otherwise LBD_NOK
 */
void stadbEntryMarkBandSupported(stadbEntry_handle_t entry,
                                 wlanif_band_e band) {
    assert(entry && band < wlanif_band_invalid);
    LBD_BOOL wasDualBand = stadbEntry_isDualBand(entry);
    entry->operatingBands |= 1 << band;
    stadbEntryUpdateTimestamp(entry);

    if (stadbEntry_isInNetwork(entry) &&
        !wasDualBand && stadbEntry_isDualBand(entry) &&
        diaglog_startEntry(mdModuleID_StaDB,
                           stadb_msgId_dualBandUpdate,
                           diaglog_level_demo)) {
        diaglog_writeMAC(&entry->addr);
        diaglog_write8(LBD_TRUE);
        diaglog_finishEntry();
    }
}

/**
 * @brief Get a timestamp in seconds for use in delta computations.
 *
 * @return the current time in seconds
 */
static time_t stadbEntryGetTimestamp(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec;
}

/**
 * @brief Update the timestamp in the entry that stores the last time it was
 *        updated.
 *
 * @param [in] entry   the handle to the entry to update
 */
static void stadbEntryUpdateTimestamp(stadbEntry_handle_t entry) {
    assert(entry);
    entry->lastUpdateSecs = stadbEntryGetTimestamp();
}

u_int8_t stadbEntryComputeHashCode(const struct ether_addr *addr) {
    return lbMACAddHash(addr->ether_addr_octet);
}

#ifdef LBD_DBG_MENU

void stadbEntryPrintHeader(struct cmdContext *context) {
    cmdf(context, "MAC Address        Age      In Network   Bands   "
                  "%-22s%-22s%-17s%-17s\n",
                  "2.4 GHz RSSI (age)", "5 GHz RSSI (age)", "Assoc? (age)", "Active? (age)");
}

void stadbEntryPrintEntry(const stadbEntry_handle_t entry,
                          struct cmdContext *context) {
    if (!entry) {
        return;
    }

    time_t curTime = stadbEntryGetTimestamp();
    cmdf(context, lbMACAddFmt(":") "  %-8u %-12s %c%c      ",
         lbMACAddData(entry->addr.ether_addr_octet),
         curTime - entry->lastUpdateSecs,
         stadbEntry_isInNetwork(entry) ? "yes" : "no",
         stadbEntry_isBandSupported(entry, wlanif_band_24g) ? '2' : ' ',
         stadbEntry_isBandSupported(entry, wlanif_band_5g) ? '5' : ' ');

    u_int8_t i;
    for (i = 0; i < wlanif_band_invalid; ++i) {
        // For associated band, display normal RSSI; for non-associated band,
        // display probe RSSI
        if (i == entry->assoc.band) {
            if (entry->latestRSSI[i].rssi != LBD_INVALID_RSSI) {
                cmdf(context, "%-3u (%10u)      ", entry->latestRSSI[i].rssi,
                     curTime - entry->latestRSSI[i].lastUpdateSecs);
            } else {
                cmdf(context, "%-22s", " ");
            }
        } else {
            if (entry->probeRSSI[i].avgRSSI != LBD_INVALID_RSSI) {
                cmdf(context, "%-3u (%10u)      ", entry->probeRSSI[i].avgRSSI,
                     curTime - entry->probeRSSI[i].lastUpdateSecs);
            } else {
                cmdf(context, "%-22s", " ");
            }
        }
    }

    char assoc = ' ';
    switch (entry->assoc.band) {
        case wlanif_band_24g:
            assoc = '2';
            break;

        case wlanif_band_5g:
            assoc = '5';
            break;

        default:
            break;
    }

    cmdf(context, "%-3c ", assoc);

#define TIME_STR_MAX_LEN 13 // headerLen (17) - assoc/activity Len (4)
    char timeStr[TIME_STR_MAX_LEN];
    if (entry->assoc.hasAssoc) {
        snprintf(timeStr, TIME_STR_MAX_LEN, "(%u)",
                 (unsigned int)(curTime - entry->assoc.lastAssoc));
        cmdf(context, "%-*s", TIME_STR_MAX_LEN, timeStr);
    }

    if (entry->assoc.band < wlanif_band_invalid) {
        cmdf(context, "%3s ", entry->activity.isAct ? "yes" : "no");
        snprintf(timeStr, TIME_STR_MAX_LEN, "(%u)",
                 (unsigned int)(curTime - entry->activity.lastActUpdate));
        cmdf(context, "%-*s", TIME_STR_MAX_LEN, timeStr);
    }

    cmdf(context, "\n");
}

#endif /* LBD_DEBUG_MENU */
