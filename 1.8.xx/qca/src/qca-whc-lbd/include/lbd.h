/*
 * @File: lbd.h
 *
 * @Abstract: Load balancing daemon main header file.
 *
 * @Notes: Type definitions used by load balancing daemon
 *
 * Copyright (c) 2011 Atheros Communications Inc.
 * All rights reserved.
 *
 * @@-COPYRIGHT-START-@@
 *
 * Copyright (c) 2012,2014 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 * @@-COPYRIGHT-END-@@
 */

#ifndef lbd__d
#define lbd__d

#include <sys/types.h>			/* Primitive types: u_int32_t, u_int8_t... */
#include <net/ethernet.h>		/* Ethernet structures */

/*
 * LBD_STATUS - Hybrid daemon API return values:
 *
 * LBD_OK: Function succeeded
 * LBD_NOK: Function failed
 *
 */
typedef enum
{
	LBD_OK = 0,
	LBD_NOK = -1

} LBD_STATUS;

/*
 * LBD_BOOL - Hybrid daemon boolean return values: FALSE & TRUE
 */
typedef enum
{
    LBD_FALSE = 0,
    LBD_TRUE = !LBD_FALSE

} LBD_BOOL;

/* MAC address utility functions */
#define HD_ETH_ADDR_LEN             ETH_ALEN

#define LBD_INVALID_RSSI 0x0

/*
 * Ethernet address structure (from net/ethernet.h):
 *
 * struct ether_addr
 * {
 *   u_int8_t ether_addr_octet[ETH_ALEN];
 * } __attribute__ ((__packed__));
 *
 *
 * Ethernet header strucutre:
 *
 * struct ether_header
 * {
 *   u_int8_t  ether_dhost[ETH_ALEN];	// destination eth addr
 *   u_int8_t  ether_shost[ETH_ALEN];	// source ether addr
 *   u_int16_t ether_type;		        // packet type ID field
 * }
 */


#endif /* lbd__d */
