// vim: set et sw=4 sts=4 cindent:
/*
 * @File: steerexec.c
 *
 * @Abstract: Top-level implementation of top-level steering executor.
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
#include <netinet/ether.h>

#include <dbg.h>

#ifdef LBD_DBG_MENU
#include <cmd.h>
#endif

#include "internal.h"
#include "module.h"
#include "profile.h"

#include "steerexec.h"
#include "steerexecLegacy.h"
#include "stadb.h"


static struct {
    /// The steering executor to use for legacy clients.
    steerexecLegacyHandle_t steerexecLegacy;

    struct dbgModule *dbgModule;
} steerexecState;

/**
 * @brief Default configuration values.
 *
 * These are used if the config file does not specify them.
 */
static struct profileElement steerexecElementDefaultTable[] = {
    { STEEREXEC_STEERING_PROHIBIT_TIME_KEY,          "300" },
    { STEEREXEC_T_STEERING_KEY,                      "15" },
    { STEEREXEC_INITIAL_AUTH_REJ_COALESCE_TIME_KEY,  "2" },
    { STEEREXEC_AUTH_REJ_MAX_KEY,                    "3" },
    { STEEREXEC_STEERING_UNFRIENDLY_TIME_KEY,        "600" },
    { STEEREXEC_LOW_RSSI_THRESHOLD_W2_KEY,           "10" },
    { STEEREXEC_LOW_RSSI_THRESHOLD_W5_KEY,           "10" },
    { STEEREXEC_TARGET_LOW_RSSI_THRESHOLD_W2_KEY,    "15" },
    { STEEREXEC_TARGET_LOW_RSSI_THRESHOLD_W5_KEY,    "15" },
    { STEEREXEC_BLACKLIST_TIME_KEY,                  "86400" },
    { NULL, NULL }
};

// Forward decls
static void steerexecReadConfig(steerexecLegacyConfig_t *config);

static void steerexecAssocObserverCB(stadbEntry_handle_t handle,
                                     void *cookie);

static void steerexecMenuInit(void);

// ====================================================================
// Public API
// ====================================================================

LBD_STATUS steerexec_init(void) {
    steerexecState.dbgModule = dbgModuleFind("steerexec");
    steerexecState.dbgModule->Level = DBGINFO;

    steerexecLegacyConfig_t config;
    steerexecReadConfig(&config);

    if (stadb_registerAssocObserver(steerexecAssocObserverCB,
                                    NULL) != LBD_OK) {
        return LBD_NOK;
    }

    steerexecState.steerexecLegacy =
        steerexecLegacyCreate(&config, steerexecState.dbgModule);
    if (!steerexecState.steerexecLegacy) {
        return LBD_NOK;
    }

    steerexecMenuInit();
    return LBD_OK;
}

LBD_STATUS steerexec_steerToBand(stadbEntry_handle_t entry,
                                 wlanif_band_e band,
                                 LBD_BOOL *ignored) {
    // For phase 2, would need to check whether the entry is legacy or not.
    // For now, we assume that it is.

    // Underlying steering orchestrator must check the entry and band itself.
    if (!steerexecState.steerexecLegacy) {
        return LBD_NOK;
    }

    return steerexecLegacySteerToBand(steerexecState.steerexecLegacy, entry,
                                      band, ignored);
}

LBD_STATUS steerexec_abortSteerToBand(stadbEntry_handle_t entry,
                                      wlanif_band_e band,
                                      LBD_BOOL *ignored) {
    // For phase 2, would need to check whether the entry is legacy or not.
    // For now, we assume that it is.

    // Underlying steering orchestrator must check the entry and band itself.
    if (!steerexecState.steerexecLegacy) {
        return LBD_NOK;
    }

    return steerexecLegacyAbortSteerToBand(steerexecState.steerexecLegacy,
                                           entry, band, ignored);
}

LBD_STATUS steerexec_registerSteeringAllowedObserver(
        steerexec_steeringAllowedObserverCB observer,
        void *cookie) {
    if (!steerexecState.steerexecLegacy) {
        return LBD_NOK;
    }

    return steerexecLegacyRegisterSteeringAllowedObserver(
            steerexecState.steerexecLegacy, observer, cookie);
}

LBD_STATUS steerexec_unregisterSteeringAllowedObserver(
        steerexec_steeringAllowedObserverCB observer,
        void *cookie) {
    if (!steerexecState.steerexecLegacy) {
        return LBD_NOK;
    }

    return steerexecLegacyUnregisterSteeringAllowedObserver(
            steerexecState.steerexecLegacy, observer, cookie);
}

LBD_STATUS steerexec_fini(void) {
    stadb_unregisterAssocObserver(steerexecAssocObserverCB, NULL);
    steerexecLegacyDestroy(steerexecState.steerexecLegacy);
    steerexecState.steerexecLegacy = NULL;
    return LBD_OK;
}

// ====================================================================
// Private helper functions
// ====================================================================

/**
 * @brief Read all of the configuration data from the file into the internal
 *        state.
 *
 * @param [out] config  the location to store the config data
 */
static void steerexecReadConfig(steerexecLegacyConfig_t *config) {
    config->steeringProhibitTime =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_STEERING_PROHIBIT_TIME_KEY,
                          steerexecElementDefaultTable);

    config->tSteering =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_T_STEERING_KEY,
                          steerexecElementDefaultTable);

    config->initialAuthRejCoalesceTime =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_INITIAL_AUTH_REJ_COALESCE_TIME_KEY,
                          steerexecElementDefaultTable);

    config->authRejMax =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_AUTH_REJ_MAX_KEY,
                          steerexecElementDefaultTable);

    config->steeringUnfriendlyTime =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_STEERING_UNFRIENDLY_TIME_KEY,
                          steerexecElementDefaultTable);

    config->lowRSSIXingThreshold[wlanif_band_24g] =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_LOW_RSSI_THRESHOLD_W2_KEY,
                          steerexecElementDefaultTable);

    config->lowRSSIXingThreshold[wlanif_band_5g] =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_LOW_RSSI_THRESHOLD_W5_KEY,
                          steerexecElementDefaultTable);

    config->targetLowRSSIThreshold[wlanif_band_24g] =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_TARGET_LOW_RSSI_THRESHOLD_W2_KEY,
                          steerexecElementDefaultTable);

    config->targetLowRSSIThreshold[wlanif_band_5g] =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_TARGET_LOW_RSSI_THRESHOLD_W5_KEY,
                          steerexecElementDefaultTable);

    config->blacklistTime =
        profileGetOptsInt(mdModuleID_SteerExec,
                          STEEREXEC_BLACKLIST_TIME_KEY,
                          steerexecElementDefaultTable);
}

/**
 * @brief Observer callback for changes in the association state for STAs in
 *        the database.
 *
 * @param [in] entry  the STA that was updated
 * @param [in] cookie  the parameter provided in the registration (unused)
 */
static void steerexecAssocObserverCB(stadbEntry_handle_t entry,
                                     void *cookie) {
    // For phase 2, would need to check whether the entry is legacy or not.
    // For now, we assume that it is.

    if (steerexecState.steerexecLegacy) {
        steerexecLegacyHandleAssocUpdate(steerexecState.steerexecLegacy,
                                         entry);
    }
}

#ifdef LBD_DBG_MENU

struct steerexecStatusCmdContext {
    struct cmdContext *context;
    LBD_BOOL inProgressOnly;
};

static void steerexecStatusLegacyIterateCB(stadbEntry_handle_t entry,
                                           void *cookie) {
    struct steerexecStatusCmdContext *statusContext =
        (struct steerexecStatusCmdContext *) cookie;

    steerexecLegacyDumpEntryState(statusContext->context,
                                  steerexecState.steerexecLegacy,
                                  entry, statusContext->inProgressOnly);
}

static const char *steerexecMenuStatusHelp[] = {
    "s -- print steering executor status",
    "Usage:",
    "\ts: display all nodes",
    "\ts in: display only steering in progress nodes",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void steerexecMenuStatusHandler(struct cmdContext *context,
                                const char *cmd) {
    LBD_BOOL inProgressOnly = LBD_FALSE;
    const char *arg = cmdWordFirst(cmd);

    if (arg && strncmp("in", arg, 2) == 0) {
        inProgressOnly = LBD_TRUE;
    }

    steerexecLegacyDumpHeader(context, steerexecState.steerexecLegacy);

    struct steerexecStatusCmdContext statusContext = {
        context, inProgressOnly
    };
    if (stadb_iterate(steerexecStatusLegacyIterateCB,
                      &statusContext) != LBD_OK) {
        cmdf(context, "Iteration over station database failed\n");
    }
}

static const char *steerexecMenuSteerHelp[] = {
    "steer -- steer a STA to a band",
    "Usage:",
    "\tsteer <mac> <2|5>: steer the MAC address to the given band",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void steerexecMenuSteerHandler(struct cmdContext *context,
                                const char *cmd) {
    const char *arg = cmdWordFirst(cmd);

    const struct ether_addr *staAddr = ether_aton(arg);
    if (!staAddr) {
        cmdf(context, "steerexec 'steer' command invalid MAC address: %s\n",
             arg);
        return;
    }

    stadbEntry_handle_t entry = stadb_find(staAddr);
    if (!entry) {
        cmdf(context, "steerexec 'steer' unknown MAC address: "
                      lbMACAddFmt(":") "\n",
             lbMACAddData(staAddr->ether_addr_octet));
        return;
    }

    arg = cmdWordNext(arg);
    if (cmdWordLen(arg) != 1) {
        cmdf(context, "steerexec 'steer' command invalid band - "
                      "expected '2|5'\n");
        return;
    }

    wlanif_band_e band = wlanif_band_invalid;
    switch (arg[0]) {
        case '2':
            band = wlanif_band_24g;
            break;

        case '5':
            band = wlanif_band_5g;
            break;

        default:
            cmdf(context, "steerexec 'steer' command invalid band '%c'\n",
                 arg[0]);
            return;
    }

    LBD_BOOL ignored;
    if (steerexec_steerToBand(entry, band, &ignored) != LBD_OK) {
        cmdf(context, "steerexec 'steer' " lbMACAddFmt(":")
                      " to band %u failed\n",
             lbMACAddData(staAddr->ether_addr_octet), band);
    } else if (ignored) {
        cmdf(context, "steerexec 'steer' " lbMACAddFmt(":")
                      " to band %u ignored by executor\n",
             lbMACAddData(staAddr->ether_addr_octet), band);
    }
}

static const char *steerexecMenuAbortHelp[] = {
    "abort -- abort steering a STA",
    "Usage:",
    "\tabort <mac> [<2|5>]: steer the MAC address to the (optional) band",
    NULL
};

#ifndef GMOCK_UNIT_TESTS
static
#endif
void steerexecMenuAbortHandler(struct cmdContext *context,
                               const char *cmd) {
    const char *arg = cmdWordFirst(cmd);

    const struct ether_addr *staAddr = ether_aton(arg);
    if (!staAddr) {
        cmdf(context, "steerexec 'abort' command invalid MAC address: %s\n",
             arg);
        return;
    }

    stadbEntry_handle_t entry = stadb_find(staAddr);
    if (!entry) {
        cmdf(context, "steerexec 'abort' unknown MAC address: "
                      lbMACAddFmt(":") "\n",
             lbMACAddData(staAddr->ether_addr_octet));
        return;
    }

    wlanif_band_e band = wlanif_band_invalid;
    arg = cmdWordNext(arg);
    if (cmdWordLen(arg) == 1) {
        switch (arg[0]) {
            case '2':
                band = wlanif_band_24g;
                break;

            case '5':
                band = wlanif_band_5g;
                break;

            default:
                cmdf(context, "steerexec 'steer' command invalid band '%c'\n",
                     arg[0]);
                return;
        }
    } else if (cmdWordLen(arg) != 0) {
        cmdf(context, "stadb 'abort' command invalid band - "
                      "expected '2|5'\n");
        return;
    }

    LBD_BOOL ignored;
    if (steerexec_abortSteerToBand(entry, band, &ignored) != LBD_OK) {
        cmdf(context, "steerexec 'abort' " lbMACAddFmt(":")
                      " to band %u failed\n",
             lbMACAddData(staAddr->ether_addr_octet), band);
    } else if (ignored) {
        cmdf(context, "steerexec 'abort' " lbMACAddFmt(":")
                      " to band %u ignored by executor\n",
             lbMACAddData(staAddr->ether_addr_octet), band);
    }
}

// Sub-menus for the steering executor debug CLI.
static const struct cmdMenuItem steerexecMenu[] = {
    CMD_MENU_STANDARD_STUFF(),
    { "s", steerexecMenuStatusHandler, NULL, steerexecMenuStatusHelp },
    { "steer", steerexecMenuSteerHandler, NULL, steerexecMenuSteerHelp },
    { "abort", steerexecMenuAbortHandler, NULL, steerexecMenuAbortHelp },
    CMD_MENU_END()
};

static const char *steerexecMenuHelp[] = {
    "steerexec -- Steering Executor",
    NULL
};

// Top-level steering executor menu.
static const struct cmdMenuItem steerexecMenuItem = {
    "steerexec",
    cmdMenu,
    (struct cmdMenuItem *) steerexecMenu,
    steerexecMenuHelp
};

#endif /* LBD_DBG_MENU */

/**
 * @brief Initialize the debug CLI hooks for this module (if necesary).
 */
static void steerexecMenuInit(void) {
#ifdef LBD_DBG_MENU
    cmdMainMenuAdd(&steerexecMenuItem);
#endif /* LBD_DBG_MENU */
}
