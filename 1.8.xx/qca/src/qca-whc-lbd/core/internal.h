/*
 * @File: internal.h
 *
 * @Abstract: Load balancing daemon internal header file.
 *
 * @Notes: Macros and functions used by the load balancing daemon
 *
 * @@-COPYRIGHT-START-@@
 *
 * Copyright (c) 2011,2014 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 * @@-COPYRIGHT-END-@@
 *
 */

#ifndef internal__h /*once only*/
#define internal__h

#include "lbd.h"
#include <string.h>
#include <stddef.h>

/*
 * lbCopyMACAddr - Copy MAC address variable
 */
#define lbCopyMACAddr(src, dst) memcpy( dst, src, HD_ETH_ADDR_LEN )

/*
 * lbAreEqualMACAddrs - Compare two MAC addresses (returns 1 if equal)
 */
#define lbAreEqualMACAddrs(arg1, arg2) (!memcmp(arg1, arg2, HD_ETH_ADDR_LEN))

/*
 * lbMACAddHash - Create a Hash out of a MAC address
 */
#define lbMACAddHash(_arg) (__lbMidx(_arg, 0) ^ __lbMidx(_arg, 1) ^ __lbMidx(_arg, 2) \
		^ __lbMidx(_arg, 3) ^ __lbMidx(_arg, 4) ^ __lbMidx(_arg, 5)) /* convert to use the HD_ETH_ADDR_LEN constant */

/*
 * lbMACAddFmt - Format a MAC address (use with (s)printf)
 */
#define lbMACAddFmt(_sep) "%02X" _sep "%02X" _sep "%02X" _sep "%02X" _sep "%02X" _sep "%02X"

/*
 * lbMACAddData - MAC Address data octets
 */
#define lbMACAddData(_arg) __lbMidx(_arg, 0), __lbMidx(_arg, 1), __lbMidx(_arg, 2), __lbMidx(_arg, 3), __lbMidx(_arg, 4), __lbMidx(_arg, 5)

#define __lbMidx(_arg, _i) (((u_int8_t *)_arg)[_i])

#endif
