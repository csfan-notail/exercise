// vim: set et sw=4 sts=4 cindent:
/*
 * @File: steerexecDiaglogDefs.h
 *
 * @Abstract: Diagnostic logging definitions for steering executor
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

#ifndef steerexecDiaglogDefs__h
#define steerexecDiaglogDefs__h

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Diagnostic logging message identifiers for the steerexec module.
 */
typedef enum steerexec_msgId_e {
    /// Steering of a specific STA to a band began.
    steerexec_msgId_steerToBand,

    /// Steering of a specific STA to a band was aborted.
    steerexec_msgId_abortSteerToBand,

    /// The steering unfriendly status for a STA changed.
    steerexec_msgId_steeringUnfriendly,

    /// The steering prohibited flag for a STA changed.
    steerexec_msgId_steeringProhibited
} steerexec_msgId_e;

#if defined(__cplusplus)
}
#endif

#endif
