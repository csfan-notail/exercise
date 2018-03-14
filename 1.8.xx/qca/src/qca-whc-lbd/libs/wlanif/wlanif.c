// vim: set et sw=4 sts=4 cindent:
/*
 * @File: wlanif.c
 *
 * @Abstract: Load balancing daemon WLAN interface
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


#include "wlanif.h"
#include "wlanifBSteerEvents.h"
#include "wlanifLinkEvents.h"
#include "wlanifBSteerControl.h"
#include "wlanifPrivate.h"

#include <stdlib.h>
#include <string.h>
#include <netinet/ether.h>

#include <dbg.h>
#include <cmd.h>

#include "module.h"

static struct {
    wlanifBSteerControlHandle_t bsteerControlHandle;
    wlanifBSteerEventsHandle_t bsteerEventsHandle;
    wlanifLinkEventsHandle_t linkEventsHandle;

    struct dbgModule *dbgModule;
} wlanifState;

static void wlanifListenInitCB(void) {
    if (wlanifBSteerEventsEnable(wlanifState.bsteerEventsHandle) != LBD_OK) {
        // An error will already have been printed. Just exist with a
        // failure code.
        exit(1);
    }
}

// Forward decls
static void wlanifMenuInit(void);

// ====================================================================
// Package level API
// ====================================================================
wlanif_band_e wlanifResolveBandFromSystemIndex(int index) {
    wlanif_band_e band = wlanif_band_invalid;
    if (wlanifState.bsteerControlHandle) {
        band = wlanifBSteerControlResolveBandFromSystemIndex(
                   wlanifState.bsteerControlHandle, index);
    }
    return band;
}

wlanif_band_e wlanifMapFreqToBand(int32_t freq) {
    if (freq <= 1000) {
        // It is a channel number. The values here for 2.4 GHz versus
        // 5 GHz are a start. It's not clear if these are right in
        // every regulator domain.
        if (freq < 27) {
            return wlanif_band_24g;
        } else {
            return wlanif_band_5g;
        }
    } else {
        // Value is a raw frequency (seems to be in 10s of Hz).
        if (freq / 100000000 >= 5) {
            return wlanif_band_5g;
        } else {
            return wlanif_band_24g;
        }
   }
}

// ====================================================================
// Public API
// ====================================================================

LBD_STATUS wlanif_init(void) {
    wlanifState.dbgModule = dbgModuleFind("wlanif");
    wlanifState.dbgModule->Level = DBGDEBUG;

    wlanifState.bsteerControlHandle =
        wlanifBSteerControlCreate(wlanifState.dbgModule);

    if (!wlanifState.bsteerControlHandle) {
        return LBD_NOK;
    }

    // bsteerEventsHandle must be created after bsteerControlHandle
    wlanifState.bsteerEventsHandle =
        wlanifBSteerEventsCreate(wlanifState.dbgModule,
                                 wlanifState.bsteerControlHandle);

    if (!wlanifState.bsteerEventsHandle) {
        return LBD_NOK;
    }

    // linkEventsHandle must be created after bsteerControlHandle
    wlanifState.linkEventsHandle =
        wlanifLinkEventsCreate(wlanifState.dbgModule,
                               wlanifState.bsteerControlHandle);

    if (!wlanifState.linkEventsHandle) {
        return LBD_NOK;
    }

    mdEventTableRegister(mdModuleID_WlanIF, wlanif_event_maxnum);
    mdListenInitCBRegister(mdModuleID_WlanIF, wlanifListenInitCB);

    LBD_BOOL enabled = LBD_FALSE;
    if (wlanifBSteerControlEnableWhenReady(wlanifState.bsteerControlHandle,
                                           &enabled) == LBD_NOK) {
        return LBD_NOK;
    }

    wlanifMenuInit();
    return LBD_OK;
}

LBD_STATUS wlanif_setOverload(wlanif_band_e band, LBD_BOOL overload) {
    if (!wlanifState.bsteerControlHandle) {
        return LBD_NOK;
    }

    return wlanifBSteerControlSetOverload(wlanifState.bsteerControlHandle,
                                          band, overload);
}

LBD_STATUS wlanif_dumpAssociatedSTAs(wlanif_associatedSTAsCB callback,
                                     void *cookie) {
    if (!wlanifState.bsteerControlHandle) {
        return LBD_NOK;
    }

    return wlanifBSteerControlDumpAssociatedSTAs(wlanifState.bsteerControlHandle,
                                                 callback, cookie);
}

LBD_STATUS wlanif_requestStaRSSI(wlanif_band_e band,
                                 const struct ether_addr * staAddr,
                                 u_int8_t numSamples) {
    if (!wlanifState.bsteerControlHandle) {
        return LBD_NOK;
    }

    return wlanifBSteerControlRequestStaRSSI(wlanifState.bsteerControlHandle,
                                             band, staAddr, numSamples);
}

LBD_STATUS wlanif_setProbeRespWHForSTA(wlanif_band_e band,
                                       const struct ether_addr *staAddr,
                                       LBD_BOOL withholdProbes) {
    if (!wlanifState.bsteerControlHandle) {
        return LBD_NOK;
    }

    return wlanifBSteerControlSetProbeRespWHForSTA(
            wlanifState.bsteerControlHandle, band, staAddr, withholdProbes);
}

LBD_STATUS wlanif_disableBandForSTA(wlanif_band_e band,
                                    const struct ether_addr *staAddr) {
    if (!wlanifState.bsteerControlHandle) {
        return LBD_NOK;
    }

    return wlanifBSteerControlDisableBandForSTA(
            wlanifState.bsteerControlHandle, band, staAddr);
}

LBD_STATUS wlanif_enableBandForSTA(wlanif_band_e band,
                                   const struct ether_addr *staAddr) {
    if (!wlanifState.bsteerControlHandle) {
        return LBD_NOK;
    }

    return wlanifBSteerControlEnableBandForSTA(
            wlanifState.bsteerControlHandle, band, staAddr);
}

LBD_STATUS wlanif_disassociateStaOnBand(wlanif_band_e band,
                                  const struct ether_addr *staAddr) {
    if (!wlanifState.bsteerControlHandle) {
        return LBD_NOK;
    }

    return wlanifBSteerControlDisassociateStaOnBand(
            wlanifState.bsteerControlHandle, band, staAddr);
}

LBD_STATUS wlanif_fini() {
    LBD_STATUS status = LBD_OK;
    // Disable band steering feature first
    status |=
        wlanifBSteerControlDisable(wlanifState.bsteerControlHandle);

    status |=
        wlanifBSteerEventsDestroy(wlanifState.bsteerEventsHandle);
    wlanifState.bsteerEventsHandle = NULL;

    status |=
        wlanifLinkEventsDestroy(wlanifState.linkEventsHandle);
    wlanifState.linkEventsHandle = NULL;

    status |=
        wlanifBSteerControlDestroy(wlanifState.bsteerControlHandle);
    wlanifState.bsteerControlHandle = NULL;

    return status;
}

// ====================================================================
// Debug CLI
// ====================================================================

#ifdef LBD_DBG_MENU

static const char *wlanifMenuAclHelp[] = {
    "acl -- perform an ACL operation",
    "Usage:",
    "\tacl enable <2|5> <mac address>: enable association on the band",
    "\tacl disable <2|5> <mac address>: disable association on the band",
    "\tacl kick <2|5> <mac address>: force disassociation",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void wlanifMenuAclHandler(struct cmdContext *context,
                          const char *cmd) {
    const char *opStr = cmdWordNth(cmd, 0);
    const char *bandStr = cmdWordNth(cmd, 1);
    const char *addrStr = cmdWordNth(cmd, 2);
    if (!opStr[0] || !bandStr[0] || !addrStr[0]) {
        cmdf(context, "Invalid ACL command: %s\n", cmd);
        return;
    }

    do {
        // Need to dup these so that we only have the word and not the subsequent
        // text.
        opStr = cmdWordDup(opStr);
        bandStr = cmdWordDup(bandStr);
        addrStr = cmdWordDup(addrStr);

        if (!opStr || !bandStr || !addrStr) {
            cmdf(context, "Failed to duplicate params before parsing\n");
            break;
        }

        wlanif_band_e band;
        if (strcmp(bandStr, "2") == 0) {
            band = wlanif_band_24g;
        } else if (strcmp(bandStr, "5") == 0) {
            band = wlanif_band_5g;
        } else {
            cmdf(context, "Invalid band: %s\n", bandStr);
            break;
        }

        const struct ether_addr *staAddr = ether_aton(addrStr);
        if (!staAddr) {
            cmdf(context, "Invalid MAC address: %s\n", addrStr);
            break;
        }

        LBD_STATUS result;
        if (strcmp(opStr, "enable") == 0) {
            result = wlanif_enableBandForSTA(band, staAddr);
        } else if (strcmp(opStr, "disable") == 0) {
            result = wlanif_disableBandForSTA(band, staAddr);
        } else if (strcmp(opStr, "kick") == 0) {
            result = wlanif_disassociateStaOnBand(band, staAddr);
        } else {
            cmdf(context, "Invalid operation: %s\n", opStr);
            break;
        }

        if (result != LBD_OK) {
            cmdf(context, "Opeation failed: %s %s %s\n",
                 opStr, bandStr, addrStr);
            break;
        }
    } while (0);

    free((char *) opStr);
    free((char *) bandStr);
    free((char *) addrStr);
}

static const struct cmdMenuItem wlanifMenu[] = {
    CMD_MENU_STANDARD_STUFF(),
    { "acl", wlanifMenuAclHandler, NULL, wlanifMenuAclHelp },
    CMD_MENU_END()
};

static const char *wlanifMenuHelp[] = {
    "wlanif -- WLAN Interface",
    NULL
};

static const struct cmdMenuItem wlanifMenuItem = {
    "wlanif",
    cmdMenu,
    (struct cmdMenuItem *) wlanifMenu,
    wlanifMenuHelp
};

#endif /* LBD_DBG_MENU */

static void wlanifMenuInit(void) {
#ifdef LBD_DBG_MENU
    cmdMainMenuAdd(&wlanifMenuItem);
#endif /* LBD_DBG_MENU */
}
