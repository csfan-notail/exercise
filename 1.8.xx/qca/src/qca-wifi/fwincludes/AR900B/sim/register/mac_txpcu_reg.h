//                                                                             
// File:       ./headers/mac_txpcu_reg.h                                       
// Creator:    afang                                                           
// Time:       Friday Oct 5, 2012 [2:07:37 pm]                                 
//                                                                             
// Path:       /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl
// Arguments:  /cad/denali/blueprint/3.7.4//Linux-64bit/blueprint -odir        
//             ./headers -codegen ath_ch.codegen -ath_ch -Wdesc                
//             mac_txpcu_reg.rdl                                               
//                                                                             
// Sources:    /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl/mac_txpcu_reg.rdl
//             /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl/ath_ch.pm
//                                                                             
// Blueprint:   3.7.4 (Fri Jan 9 05:41:17 PST 2009)                            
// Machine:    crimson                                                         
// OS:         Linux 2.6.9-103.ELsmp                                           
// Description:                                                                
//                                                                             
// No Description Provided                                                     
//                                                                             
//                                                                             


#ifndef _MAC_TXPCU_REG_H_
#define _MAC_TXPCU_REG_H_

// 0x0000 (TXPCU_TIMEOUT)
#define TXPCU_TIMEOUT_DATA_WAIT_TIME_MSB                             31
#define TXPCU_TIMEOUT_DATA_WAIT_TIME_LSB                             24
#define TXPCU_TIMEOUT_DATA_WAIT_TIME_MASK                            0xff000000
#define TXPCU_TIMEOUT_DATA_WAIT_TIME_GET(x)                          (((x) & TXPCU_TIMEOUT_DATA_WAIT_TIME_MASK) >> TXPCU_TIMEOUT_DATA_WAIT_TIME_LSB)
#define TXPCU_TIMEOUT_DATA_WAIT_TIME_SET(x)                          (((0x0 | (x)) << TXPCU_TIMEOUT_DATA_WAIT_TIME_LSB) & TXPCU_TIMEOUT_DATA_WAIT_TIME_MASK)
#define TXPCU_TIMEOUT_DATA_WAIT_TIME_RESET                           0
#define TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_MSB                         23
#define TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_LSB                         16
#define TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_MASK                        0x00ff0000
#define TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_GET(x)                      (((x) & TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_MASK) >> TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_LSB)
#define TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_SET(x)                      (((0x0 | (x)) << TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_LSB) & TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_MASK)
#define TXPCU_TIMEOUT_PREAMBLE_WAIT_TIME_RESET                       0
#define TXPCU_TIMEOUT_CTS_TIMEOUT_MSB                                15
#define TXPCU_TIMEOUT_CTS_TIMEOUT_LSB                                8
#define TXPCU_TIMEOUT_CTS_TIMEOUT_MASK                               0x0000ff00
#define TXPCU_TIMEOUT_CTS_TIMEOUT_GET(x)                             (((x) & TXPCU_TIMEOUT_CTS_TIMEOUT_MASK) >> TXPCU_TIMEOUT_CTS_TIMEOUT_LSB)
#define TXPCU_TIMEOUT_CTS_TIMEOUT_SET(x)                             (((0x0 | (x)) << TXPCU_TIMEOUT_CTS_TIMEOUT_LSB) & TXPCU_TIMEOUT_CTS_TIMEOUT_MASK)
#define TXPCU_TIMEOUT_CTS_TIMEOUT_RESET                              0
#define TXPCU_TIMEOUT_ACK_TIMEOUT_MSB                                7
#define TXPCU_TIMEOUT_ACK_TIMEOUT_LSB                                0
#define TXPCU_TIMEOUT_ACK_TIMEOUT_MASK                               0x000000ff
#define TXPCU_TIMEOUT_ACK_TIMEOUT_GET(x)                             (((x) & TXPCU_TIMEOUT_ACK_TIMEOUT_MASK) >> TXPCU_TIMEOUT_ACK_TIMEOUT_LSB)
#define TXPCU_TIMEOUT_ACK_TIMEOUT_SET(x)                             (((0x0 | (x)) << TXPCU_TIMEOUT_ACK_TIMEOUT_LSB) & TXPCU_TIMEOUT_ACK_TIMEOUT_MASK)
#define TXPCU_TIMEOUT_ACK_TIMEOUT_RESET                              0
#define TXPCU_TIMEOUT_ADDRESS                                        0x000000
#define TXPCU_TIMEOUT_HW_MASK                                        0xffffffff
#define TXPCU_TIMEOUT_SW_MASK                                        0xffffffff
#define TXPCU_TIMEOUT_RSTMASK                                        0x00000000
#define TXPCU_TIMEOUT_RESET                                          0x00000000

// 0x0004 (TXPCU_WATCHDOG)
#define TXPCU_WATCHDOG_STATUS_MSB                                    31
#define TXPCU_WATCHDOG_STATUS_LSB                                    16
#define TXPCU_WATCHDOG_STATUS_MASK                                   0xffff0000
#define TXPCU_WATCHDOG_STATUS_GET(x)                                 (((x) & TXPCU_WATCHDOG_STATUS_MASK) >> TXPCU_WATCHDOG_STATUS_LSB)
#define TXPCU_WATCHDOG_STATUS_SET(x)                                 (((0x0 | (x)) << TXPCU_WATCHDOG_STATUS_LSB) & TXPCU_WATCHDOG_STATUS_MASK)
#define TXPCU_WATCHDOG_STATUS_RESET                                  0
#define TXPCU_WATCHDOG_LIMIT_MSB                                     15
#define TXPCU_WATCHDOG_LIMIT_LSB                                     0
#define TXPCU_WATCHDOG_LIMIT_MASK                                    0x0000ffff
#define TXPCU_WATCHDOG_LIMIT_GET(x)                                  (((x) & TXPCU_WATCHDOG_LIMIT_MASK) >> TXPCU_WATCHDOG_LIMIT_LSB)
#define TXPCU_WATCHDOG_LIMIT_SET(x)                                  (((0x0 | (x)) << TXPCU_WATCHDOG_LIMIT_LSB) & TXPCU_WATCHDOG_LIMIT_MASK)
#define TXPCU_WATCHDOG_LIMIT_RESET                                   10000
#define TXPCU_WATCHDOG_ADDRESS                                       0x000004
#define TXPCU_WATCHDOG_HW_MASK                                       0xffffffff
#define TXPCU_WATCHDOG_SW_MASK                                       0xffffffff
#define TXPCU_WATCHDOG_RSTMASK                                       0x0000ffff
#define TXPCU_WATCHDOG_RESET                                         0x00002710

// 0x0008 (TXPCU_INITIAL_CREDIT)
#define TXPCU_INITIAL_CREDIT_VALUE_USER2_MSB                         23
#define TXPCU_INITIAL_CREDIT_VALUE_USER2_LSB                         16
#define TXPCU_INITIAL_CREDIT_VALUE_USER2_MASK                        0x00ff0000
#define TXPCU_INITIAL_CREDIT_VALUE_USER2_GET(x)                      (((x) & TXPCU_INITIAL_CREDIT_VALUE_USER2_MASK) >> TXPCU_INITIAL_CREDIT_VALUE_USER2_LSB)
#define TXPCU_INITIAL_CREDIT_VALUE_USER2_SET(x)                      (((0x0 | (x)) << TXPCU_INITIAL_CREDIT_VALUE_USER2_LSB) & TXPCU_INITIAL_CREDIT_VALUE_USER2_MASK)
#define TXPCU_INITIAL_CREDIT_VALUE_USER2_RESET                       0
#define TXPCU_INITIAL_CREDIT_VALUE_USER1_MSB                         15
#define TXPCU_INITIAL_CREDIT_VALUE_USER1_LSB                         8
#define TXPCU_INITIAL_CREDIT_VALUE_USER1_MASK                        0x0000ff00
#define TXPCU_INITIAL_CREDIT_VALUE_USER1_GET(x)                      (((x) & TXPCU_INITIAL_CREDIT_VALUE_USER1_MASK) >> TXPCU_INITIAL_CREDIT_VALUE_USER1_LSB)
#define TXPCU_INITIAL_CREDIT_VALUE_USER1_SET(x)                      (((0x0 | (x)) << TXPCU_INITIAL_CREDIT_VALUE_USER1_LSB) & TXPCU_INITIAL_CREDIT_VALUE_USER1_MASK)
#define TXPCU_INITIAL_CREDIT_VALUE_USER1_RESET                       0
#define TXPCU_INITIAL_CREDIT_VALUE_USER0_MSB                         7
#define TXPCU_INITIAL_CREDIT_VALUE_USER0_LSB                         0
#define TXPCU_INITIAL_CREDIT_VALUE_USER0_MASK                        0x000000ff
#define TXPCU_INITIAL_CREDIT_VALUE_USER0_GET(x)                      (((x) & TXPCU_INITIAL_CREDIT_VALUE_USER0_MASK) >> TXPCU_INITIAL_CREDIT_VALUE_USER0_LSB)
#define TXPCU_INITIAL_CREDIT_VALUE_USER0_SET(x)                      (((0x0 | (x)) << TXPCU_INITIAL_CREDIT_VALUE_USER0_LSB) & TXPCU_INITIAL_CREDIT_VALUE_USER0_MASK)
#define TXPCU_INITIAL_CREDIT_VALUE_USER0_RESET                       0
#define TXPCU_INITIAL_CREDIT_ADDRESS                                 0x000008
#define TXPCU_INITIAL_CREDIT_HW_MASK                                 0x00ffffff
#define TXPCU_INITIAL_CREDIT_SW_MASK                                 0x00ffffff
#define TXPCU_INITIAL_CREDIT_RSTMASK                                 0x00000000
#define TXPCU_INITIAL_CREDIT_RESET                                   0x00000000

// 0x0010 (TXPCU_BASIC_RATE_SET0)
#define TXPCU_BASIC_RATE_SET0_VALUE_MSB                              29
#define TXPCU_BASIC_RATE_SET0_VALUE_LSB                              0
#define TXPCU_BASIC_RATE_SET0_VALUE_MASK                             0x3fffffff
#define TXPCU_BASIC_RATE_SET0_VALUE_GET(x)                           (((x) & TXPCU_BASIC_RATE_SET0_VALUE_MASK) >> TXPCU_BASIC_RATE_SET0_VALUE_LSB)
#define TXPCU_BASIC_RATE_SET0_VALUE_SET(x)                           (((0x0 | (x)) << TXPCU_BASIC_RATE_SET0_VALUE_LSB) & TXPCU_BASIC_RATE_SET0_VALUE_MASK)
#define TXPCU_BASIC_RATE_SET0_VALUE_RESET                            836565851
#define TXPCU_BASIC_RATE_SET0_ADDRESS                                0x000010
#define TXPCU_BASIC_RATE_SET0_HW_MASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET0_SW_MASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET0_RSTMASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET0_RESET                                  0x31dcfb5b

// 0x0014 (TXPCU_BASIC_RATE_SET1)
#define TXPCU_BASIC_RATE_SET1_VALUE_MSB                              29
#define TXPCU_BASIC_RATE_SET1_VALUE_LSB                              0
#define TXPCU_BASIC_RATE_SET1_VALUE_MASK                             0x3fffffff
#define TXPCU_BASIC_RATE_SET1_VALUE_GET(x)                           (((x) & TXPCU_BASIC_RATE_SET1_VALUE_MASK) >> TXPCU_BASIC_RATE_SET1_VALUE_LSB)
#define TXPCU_BASIC_RATE_SET1_VALUE_SET(x)                           (((0x0 | (x)) << TXPCU_BASIC_RATE_SET1_VALUE_LSB) & TXPCU_BASIC_RATE_SET1_VALUE_MASK)
#define TXPCU_BASIC_RATE_SET1_VALUE_RESET                            312814972
#define TXPCU_BASIC_RATE_SET1_ADDRESS                                0x000014
#define TXPCU_BASIC_RATE_SET1_HW_MASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET1_SW_MASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET1_RSTMASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET1_RESET                                  0x12a52d7c

// 0x0018 (TXPCU_BASIC_RATE_SET2)
#define TXPCU_BASIC_RATE_SET2_VALUE_MSB                              29
#define TXPCU_BASIC_RATE_SET2_VALUE_LSB                              0
#define TXPCU_BASIC_RATE_SET2_VALUE_MASK                             0x3fffffff
#define TXPCU_BASIC_RATE_SET2_VALUE_GET(x)                           (((x) & TXPCU_BASIC_RATE_SET2_VALUE_MASK) >> TXPCU_BASIC_RATE_SET2_VALUE_LSB)
#define TXPCU_BASIC_RATE_SET2_VALUE_SET(x)                           (((0x0 | (x)) << TXPCU_BASIC_RATE_SET2_VALUE_LSB) & TXPCU_BASIC_RATE_SET2_VALUE_MASK)
#define TXPCU_BASIC_RATE_SET2_VALUE_RESET                            346400041
#define TXPCU_BASIC_RATE_SET2_ADDRESS                                0x000018
#define TXPCU_BASIC_RATE_SET2_HW_MASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET2_SW_MASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET2_RSTMASK                                0x3fffffff
#define TXPCU_BASIC_RATE_SET2_RESET                                  0x14a5a529

// 0x001c (TXPCU_BASIC_RATE_SET3)
#define TXPCU_BASIC_RATE_SET3_VALUE_MSB                              24
#define TXPCU_BASIC_RATE_SET3_VALUE_LSB                              0
#define TXPCU_BASIC_RATE_SET3_VALUE_MASK                             0x01ffffff
#define TXPCU_BASIC_RATE_SET3_VALUE_GET(x)                           (((x) & TXPCU_BASIC_RATE_SET3_VALUE_MASK) >> TXPCU_BASIC_RATE_SET3_VALUE_LSB)
#define TXPCU_BASIC_RATE_SET3_VALUE_SET(x)                           (((0x0 | (x)) << TXPCU_BASIC_RATE_SET3_VALUE_LSB) & TXPCU_BASIC_RATE_SET3_VALUE_MASK)
#define TXPCU_BASIC_RATE_SET3_VALUE_RESET                            9741609
#define TXPCU_BASIC_RATE_SET3_ADDRESS                                0x00001c
#define TXPCU_BASIC_RATE_SET3_HW_MASK                                0x01ffffff
#define TXPCU_BASIC_RATE_SET3_SW_MASK                                0x01ffffff
#define TXPCU_BASIC_RATE_SET3_RSTMASK                                0x01ffffff
#define TXPCU_BASIC_RATE_SET3_RESET                                  0x0094a529

// 0x0020 (TXPCU_BASIC_RATE_SET4)
#define TXPCU_BASIC_RATE_SET4_VALUE_MSB                              9
#define TXPCU_BASIC_RATE_SET4_VALUE_LSB                              0
#define TXPCU_BASIC_RATE_SET4_VALUE_MASK                             0x000003ff
#define TXPCU_BASIC_RATE_SET4_VALUE_GET(x)                           (((x) & TXPCU_BASIC_RATE_SET4_VALUE_MASK) >> TXPCU_BASIC_RATE_SET4_VALUE_LSB)
#define TXPCU_BASIC_RATE_SET4_VALUE_SET(x)                           (((0x0 | (x)) << TXPCU_BASIC_RATE_SET4_VALUE_LSB) & TXPCU_BASIC_RATE_SET4_VALUE_MASK)
#define TXPCU_BASIC_RATE_SET4_VALUE_RESET                            297
#define TXPCU_BASIC_RATE_SET4_ADDRESS                                0x000020
#define TXPCU_BASIC_RATE_SET4_HW_MASK                                0x000003ff
#define TXPCU_BASIC_RATE_SET4_SW_MASK                                0x000003ff
#define TXPCU_BASIC_RATE_SET4_RSTMASK                                0x000003ff
#define TXPCU_BASIC_RATE_SET4_RESET                                  0x00000129

// 0x0024 (TXPCU_BASIC_SET)
#define TXPCU_BASIC_SET_MCS_MSB                                      31
#define TXPCU_BASIC_SET_MCS_LSB                                      0
#define TXPCU_BASIC_SET_MCS_MASK                                     0xffffffff
#define TXPCU_BASIC_SET_MCS_GET(x)                                   (((x) & TXPCU_BASIC_SET_MCS_MASK) >> TXPCU_BASIC_SET_MCS_LSB)
#define TXPCU_BASIC_SET_MCS_SET(x)                                   (((0x0 | (x)) << TXPCU_BASIC_SET_MCS_LSB) & TXPCU_BASIC_SET_MCS_MASK)
#define TXPCU_BASIC_SET_MCS_RESET                                    0
#define TXPCU_BASIC_SET_ADDRESS                                      0x000024
#define TXPCU_BASIC_SET_HW_MASK                                      0xffffffff
#define TXPCU_BASIC_SET_SW_MASK                                      0xffffffff
#define TXPCU_BASIC_SET_RSTMASK                                      0x00000000
#define TXPCU_BASIC_SET_RESET                                        0x00000000

// 0x0030 (TXPCU_RTS_SUCCESS_CNT)
#define TXPCU_RTS_SUCCESS_CNT_VALUE_MSB                              15
#define TXPCU_RTS_SUCCESS_CNT_VALUE_LSB                              0
#define TXPCU_RTS_SUCCESS_CNT_VALUE_MASK                             0x0000ffff
#define TXPCU_RTS_SUCCESS_CNT_VALUE_GET(x)                           (((x) & TXPCU_RTS_SUCCESS_CNT_VALUE_MASK) >> TXPCU_RTS_SUCCESS_CNT_VALUE_LSB)
#define TXPCU_RTS_SUCCESS_CNT_VALUE_SET(x)                           (((0x0 | (x)) << TXPCU_RTS_SUCCESS_CNT_VALUE_LSB) & TXPCU_RTS_SUCCESS_CNT_VALUE_MASK)
#define TXPCU_RTS_SUCCESS_CNT_VALUE_RESET                            0
#define TXPCU_RTS_SUCCESS_CNT_ADDRESS                                0x000030
#define TXPCU_RTS_SUCCESS_CNT_HW_MASK                                0x0000ffff
#define TXPCU_RTS_SUCCESS_CNT_SW_MASK                                0x0000ffff
#define TXPCU_RTS_SUCCESS_CNT_RSTMASK                                0x00000000
#define TXPCU_RTS_SUCCESS_CNT_RESET                                  0x00000000

// 0x0034 (TXPCU_RTS_FAIL_CNT)
#define TXPCU_RTS_FAIL_CNT_VALUE_MSB                                 15
#define TXPCU_RTS_FAIL_CNT_VALUE_LSB                                 0
#define TXPCU_RTS_FAIL_CNT_VALUE_MASK                                0x0000ffff
#define TXPCU_RTS_FAIL_CNT_VALUE_GET(x)                              (((x) & TXPCU_RTS_FAIL_CNT_VALUE_MASK) >> TXPCU_RTS_FAIL_CNT_VALUE_LSB)
#define TXPCU_RTS_FAIL_CNT_VALUE_SET(x)                              (((0x0 | (x)) << TXPCU_RTS_FAIL_CNT_VALUE_LSB) & TXPCU_RTS_FAIL_CNT_VALUE_MASK)
#define TXPCU_RTS_FAIL_CNT_VALUE_RESET                               0
#define TXPCU_RTS_FAIL_CNT_ADDRESS                                   0x000034
#define TXPCU_RTS_FAIL_CNT_HW_MASK                                   0x0000ffff
#define TXPCU_RTS_FAIL_CNT_SW_MASK                                   0x0000ffff
#define TXPCU_RTS_FAIL_CNT_RSTMASK                                   0x00000000
#define TXPCU_RTS_FAIL_CNT_RESET                                     0x00000000

// 0x0038 (TXPCU_ACK_FAIL_CNT)
#define TXPCU_ACK_FAIL_CNT_VALUE_MSB                                 15
#define TXPCU_ACK_FAIL_CNT_VALUE_LSB                                 0
#define TXPCU_ACK_FAIL_CNT_VALUE_MASK                                0x0000ffff
#define TXPCU_ACK_FAIL_CNT_VALUE_GET(x)                              (((x) & TXPCU_ACK_FAIL_CNT_VALUE_MASK) >> TXPCU_ACK_FAIL_CNT_VALUE_LSB)
#define TXPCU_ACK_FAIL_CNT_VALUE_SET(x)                              (((0x0 | (x)) << TXPCU_ACK_FAIL_CNT_VALUE_LSB) & TXPCU_ACK_FAIL_CNT_VALUE_MASK)
#define TXPCU_ACK_FAIL_CNT_VALUE_RESET                               0
#define TXPCU_ACK_FAIL_CNT_ADDRESS                                   0x000038
#define TXPCU_ACK_FAIL_CNT_HW_MASK                                   0x0000ffff
#define TXPCU_ACK_FAIL_CNT_SW_MASK                                   0x0000ffff
#define TXPCU_ACK_FAIL_CNT_RSTMASK                                   0x00000000
#define TXPCU_ACK_FAIL_CNT_RESET                                     0x00000000

// 0x003c (TXPCU_TX_FRAME_CNT)
#define TXPCU_TX_FRAME_CNT_VALUE_MSB                                 31
#define TXPCU_TX_FRAME_CNT_VALUE_LSB                                 0
#define TXPCU_TX_FRAME_CNT_VALUE_MASK                                0xffffffff
#define TXPCU_TX_FRAME_CNT_VALUE_GET(x)                              (((x) & TXPCU_TX_FRAME_CNT_VALUE_MASK) >> TXPCU_TX_FRAME_CNT_VALUE_LSB)
#define TXPCU_TX_FRAME_CNT_VALUE_SET(x)                              (((0x0 | (x)) << TXPCU_TX_FRAME_CNT_VALUE_LSB) & TXPCU_TX_FRAME_CNT_VALUE_MASK)
#define TXPCU_TX_FRAME_CNT_VALUE_RESET                               0
#define TXPCU_TX_FRAME_CNT_ADDRESS                                   0x00003c
#define TXPCU_TX_FRAME_CNT_HW_MASK                                   0xffffffff
#define TXPCU_TX_FRAME_CNT_SW_MASK                                   0xffffffff
#define TXPCU_TX_FRAME_CNT_RSTMASK                                   0x00000000
#define TXPCU_TX_FRAME_CNT_RESET                                     0x00000000

// 0x0040 (TXPCU_TXSIFS)
#define TXPCU_TXSIFS_ACK_SHIFT_MSB                                   14
#define TXPCU_TXSIFS_ACK_SHIFT_LSB                                   12
#define TXPCU_TXSIFS_ACK_SHIFT_MASK                                  0x00007000
#define TXPCU_TXSIFS_ACK_SHIFT_GET(x)                                (((x) & TXPCU_TXSIFS_ACK_SHIFT_MASK) >> TXPCU_TXSIFS_ACK_SHIFT_LSB)
#define TXPCU_TXSIFS_ACK_SHIFT_SET(x)                                (((0x0 | (x)) << TXPCU_TXSIFS_ACK_SHIFT_LSB) & TXPCU_TXSIFS_ACK_SHIFT_MASK)
#define TXPCU_TXSIFS_ACK_SHIFT_RESET                                 3
#define TXPCU_TXSIFS_TX_LATENCY_MSB                                  11
#define TXPCU_TXSIFS_TX_LATENCY_LSB                                  8
#define TXPCU_TXSIFS_TX_LATENCY_MASK                                 0x00000f00
#define TXPCU_TXSIFS_TX_LATENCY_GET(x)                               (((x) & TXPCU_TXSIFS_TX_LATENCY_MASK) >> TXPCU_TXSIFS_TX_LATENCY_LSB)
#define TXPCU_TXSIFS_TX_LATENCY_SET(x)                               (((0x0 | (x)) << TXPCU_TXSIFS_TX_LATENCY_LSB) & TXPCU_TXSIFS_TX_LATENCY_MASK)
#define TXPCU_TXSIFS_TX_LATENCY_RESET                                2
#define TXPCU_TXSIFS_SIFS_TIME_MSB                                   7
#define TXPCU_TXSIFS_SIFS_TIME_LSB                                   0
#define TXPCU_TXSIFS_SIFS_TIME_MASK                                  0x000000ff
#define TXPCU_TXSIFS_SIFS_TIME_GET(x)                                (((x) & TXPCU_TXSIFS_SIFS_TIME_MASK) >> TXPCU_TXSIFS_SIFS_TIME_LSB)
#define TXPCU_TXSIFS_SIFS_TIME_SET(x)                                (((0x0 | (x)) << TXPCU_TXSIFS_SIFS_TIME_LSB) & TXPCU_TXSIFS_SIFS_TIME_MASK)
#define TXPCU_TXSIFS_SIFS_TIME_RESET                                 16
#define TXPCU_TXSIFS_ADDRESS                                         0x000040
#define TXPCU_TXSIFS_HW_MASK                                         0x00007fff
#define TXPCU_TXSIFS_SW_MASK                                         0x00007fff
#define TXPCU_TXSIFS_RSTMASK                                         0x00007fff
#define TXPCU_TXSIFS_RESET                                           0x00003210

// 0x0044 (TXPCU_USEC_LATENCY)
#define TXPCU_USEC_LATENCY_TX_LATENCY_MSB                            8
#define TXPCU_USEC_LATENCY_TX_LATENCY_LSB                            0
#define TXPCU_USEC_LATENCY_TX_LATENCY_MASK                           0x000001ff
#define TXPCU_USEC_LATENCY_TX_LATENCY_GET(x)                         (((x) & TXPCU_USEC_LATENCY_TX_LATENCY_MASK) >> TXPCU_USEC_LATENCY_TX_LATENCY_LSB)
#define TXPCU_USEC_LATENCY_TX_LATENCY_SET(x)                         (((0x0 | (x)) << TXPCU_USEC_LATENCY_TX_LATENCY_LSB) & TXPCU_USEC_LATENCY_TX_LATENCY_MASK)
#define TXPCU_USEC_LATENCY_TX_LATENCY_RESET                          0
#define TXPCU_USEC_LATENCY_ADDRESS                                   0x000044
#define TXPCU_USEC_LATENCY_HW_MASK                                   0x000001ff
#define TXPCU_USEC_LATENCY_SW_MASK                                   0x000001ff
#define TXPCU_USEC_LATENCY_RSTMASK                                   0x00000000
#define TXPCU_USEC_LATENCY_RESET                                     0x00000000

// 0x0048 (TXPCU_DPD_TRAINING_SELF_CTS_BW)
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_MSB                       1
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_LSB                       0
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_MASK                      0x00000003
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_GET(x)                    (((x) & TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_MASK) >> TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_LSB)
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_SET(x)                    (((0x0 | (x)) << TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_LSB) & TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_MASK)
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_REG_RESET                     0
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_ADDRESS                       0x000048
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_HW_MASK                       0x00000003
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_SW_MASK                       0x00000003
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_RSTMASK                       0x00000000
#define TXPCU_DPD_TRAINING_SELF_CTS_BW_RESET                         0x00000000

// 0x004c (TXPCU_DEF_ANTENNA)
#define TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_MSB                          31
#define TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_LSB                          31
#define TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_MASK                         0x80000000
#define TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_GET(x)                       (((x) & TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_MASK) >> TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_LSB)
#define TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_SET(x)                       (((0x0 | (x)) << TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_LSB) & TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_MASK)
#define TXPCU_DEF_ANTENNA_RX_ANT_DIV_ON_RESET                        0
#define TXPCU_DEF_ANTENNA_RX_ANT_EN_MSB                              30
#define TXPCU_DEF_ANTENNA_RX_ANT_EN_LSB                              30
#define TXPCU_DEF_ANTENNA_RX_ANT_EN_MASK                             0x40000000
#define TXPCU_DEF_ANTENNA_RX_ANT_EN_GET(x)                           (((x) & TXPCU_DEF_ANTENNA_RX_ANT_EN_MASK) >> TXPCU_DEF_ANTENNA_RX_ANT_EN_LSB)
#define TXPCU_DEF_ANTENNA_RX_ANT_EN_SET(x)                           (((0x0 | (x)) << TXPCU_DEF_ANTENNA_RX_ANT_EN_LSB) & TXPCU_DEF_ANTENNA_RX_ANT_EN_MASK)
#define TXPCU_DEF_ANTENNA_RX_ANT_EN_RESET                            0
#define TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_MSB                         29
#define TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_LSB                         29
#define TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_MASK                        0x20000000
#define TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_GET(x)                      (((x) & TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_MASK) >> TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_LSB)
#define TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_SET(x)                      (((0x0 | (x)) << TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_LSB) & TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_MASK)
#define TXPCU_DEF_ANTENNA_FAST_TX_ANT_EN_RESET                       0
#define TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_MSB                      28
#define TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_LSB                      28
#define TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_MASK                     0x10000000
#define TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_GET(x)                   (((x) & TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_MASK) >> TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_LSB)
#define TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_SET(x)                   (((0x0 | (x)) << TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_LSB) & TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_MASK)
#define TXPCU_DEF_ANTENNA_RX_LNA_CONFIG_SEL_RESET                    0
#define TXPCU_DEF_ANTENNA_FAST_DEF_ANT_MSB                           27
#define TXPCU_DEF_ANTENNA_FAST_DEF_ANT_LSB                           27
#define TXPCU_DEF_ANTENNA_FAST_DEF_ANT_MASK                          0x08000000
#define TXPCU_DEF_ANTENNA_FAST_DEF_ANT_GET(x)                        (((x) & TXPCU_DEF_ANTENNA_FAST_DEF_ANT_MASK) >> TXPCU_DEF_ANTENNA_FAST_DEF_ANT_LSB)
#define TXPCU_DEF_ANTENNA_FAST_DEF_ANT_SET(x)                        (((0x0 | (x)) << TXPCU_DEF_ANTENNA_FAST_DEF_ANT_LSB) & TXPCU_DEF_ANTENNA_FAST_DEF_ANT_MASK)
#define TXPCU_DEF_ANTENNA_FAST_DEF_ANT_RESET                         0
#define TXPCU_DEF_ANTENNA_TX_CUR_ANT_MSB                             26
#define TXPCU_DEF_ANTENNA_TX_CUR_ANT_LSB                             26
#define TXPCU_DEF_ANTENNA_TX_CUR_ANT_MASK                            0x04000000
#define TXPCU_DEF_ANTENNA_TX_CUR_ANT_GET(x)                          (((x) & TXPCU_DEF_ANTENNA_TX_CUR_ANT_MASK) >> TXPCU_DEF_ANTENNA_TX_CUR_ANT_LSB)
#define TXPCU_DEF_ANTENNA_TX_CUR_ANT_SET(x)                          (((0x0 | (x)) << TXPCU_DEF_ANTENNA_TX_CUR_ANT_LSB) & TXPCU_DEF_ANTENNA_TX_CUR_ANT_MASK)
#define TXPCU_DEF_ANTENNA_TX_CUR_ANT_RESET                           0
#define TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_MSB                         25
#define TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_LSB                         25
#define TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_MASK                        0x02000000
#define TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_GET(x)                      (((x) & TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_MASK) >> TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_LSB)
#define TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_SET(x)                      (((0x0 | (x)) << TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_LSB) & TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_MASK)
#define TXPCU_DEF_ANTENNA_SLOW_TX_ANT_EN_RESET                       0
#define TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_MSB                         24
#define TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_LSB                         24
#define TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_MASK                        0x01000000
#define TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_GET(x)                      (((x) & TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_MASK) >> TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_LSB)
#define TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_SET(x)                      (((0x0 | (x)) << TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_LSB) & TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_MASK)
#define TXPCU_DEF_ANTENNA_TX_DEF_ANT_SEL_RESET                       0
#define TXPCU_DEF_ANTENNA_VALUE_MSB                                  23
#define TXPCU_DEF_ANTENNA_VALUE_LSB                                  0
#define TXPCU_DEF_ANTENNA_VALUE_MASK                                 0x00ffffff
#define TXPCU_DEF_ANTENNA_VALUE_GET(x)                               (((x) & TXPCU_DEF_ANTENNA_VALUE_MASK) >> TXPCU_DEF_ANTENNA_VALUE_LSB)
#define TXPCU_DEF_ANTENNA_VALUE_SET(x)                               (((0x0 | (x)) << TXPCU_DEF_ANTENNA_VALUE_LSB) & TXPCU_DEF_ANTENNA_VALUE_MASK)
#define TXPCU_DEF_ANTENNA_VALUE_RESET                                0
#define TXPCU_DEF_ANTENNA_ADDRESS                                    0x00004c
#define TXPCU_DEF_ANTENNA_HW_MASK                                    0xffffffff
#define TXPCU_DEF_ANTENNA_SW_MASK                                    0xffffffff
#define TXPCU_DEF_ANTENNA_RSTMASK                                    0x00000000
#define TXPCU_DEF_ANTENNA_RESET                                      0x00000000

// 0x0050 (TXPCU_SELF_GEN_ANTENNA_MASK)
#define TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_MSB                  7
#define TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_LSB                  5
#define TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_MASK                 0x000000e0
#define TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_GET(x)               (((x) & TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_MASK) >> TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_LSB)
#define TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_SET(x)               (((0x0 | (x)) << TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_LSB) & TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_MASK)
#define TXPCU_SELF_GEN_ANTENNA_MASK_M_ACK_VALUE_RESET                7
#define TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_MSB                4
#define TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_LSB                4
#define TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_MASK               0x00000010
#define TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_GET(x)             (((x) & TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_MASK) >> TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_LSB)
#define TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_SET(x)             (((0x0 | (x)) << TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_LSB) & TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_MASK)
#define TXPCU_SELF_GEN_ANTENNA_MASK_FORCE_CHAIN_0_RESET              0
#define TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_MSB                  3
#define TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_LSB                  3
#define TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_MASK                 0x00000008
#define TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_GET(x)               (((x) & TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_MASK) >> TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_LSB)
#define TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_SET(x)               (((0x0 | (x)) << TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_LSB) & TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_MASK)
#define TXPCU_SELF_GEN_ANTENNA_MASK_ONE_RESP_EN_RESET                1
#define TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_MSB                        2
#define TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_LSB                        0
#define TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_MASK                       0x00000007
#define TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_GET(x)                     (((x) & TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_MASK) >> TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_LSB)
#define TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_SET(x)                     (((0x0 | (x)) << TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_LSB) & TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_MASK)
#define TXPCU_SELF_GEN_ANTENNA_MASK_VALUE_RESET                      7
#define TXPCU_SELF_GEN_ANTENNA_MASK_ADDRESS                          0x000050
#define TXPCU_SELF_GEN_ANTENNA_MASK_HW_MASK                          0x000000ff
#define TXPCU_SELF_GEN_ANTENNA_MASK_SW_MASK                          0x000000ff
#define TXPCU_SELF_GEN_ANTENNA_MASK_RSTMASK                          0x000000ef
#define TXPCU_SELF_GEN_ANTENNA_MASK_RESET                            0x000000ef

// 0x0054 (TXPCU_MGMT_SEQ)
#define TXPCU_MGMT_SEQ_MAX_MSB                                       27
#define TXPCU_MGMT_SEQ_MAX_LSB                                       16
#define TXPCU_MGMT_SEQ_MAX_MASK                                      0x0fff0000
#define TXPCU_MGMT_SEQ_MAX_GET(x)                                    (((x) & TXPCU_MGMT_SEQ_MAX_MASK) >> TXPCU_MGMT_SEQ_MAX_LSB)
#define TXPCU_MGMT_SEQ_MAX_SET(x)                                    (((0x0 | (x)) << TXPCU_MGMT_SEQ_MAX_LSB) & TXPCU_MGMT_SEQ_MAX_MASK)
#define TXPCU_MGMT_SEQ_MAX_RESET                                     767
#define TXPCU_MGMT_SEQ_MIN_MSB                                       11
#define TXPCU_MGMT_SEQ_MIN_LSB                                       0
#define TXPCU_MGMT_SEQ_MIN_MASK                                      0x00000fff
#define TXPCU_MGMT_SEQ_MIN_GET(x)                                    (((x) & TXPCU_MGMT_SEQ_MIN_MASK) >> TXPCU_MGMT_SEQ_MIN_LSB)
#define TXPCU_MGMT_SEQ_MIN_SET(x)                                    (((0x0 | (x)) << TXPCU_MGMT_SEQ_MIN_LSB) & TXPCU_MGMT_SEQ_MIN_MASK)
#define TXPCU_MGMT_SEQ_MIN_RESET                                     0
#define TXPCU_MGMT_SEQ_ADDRESS                                       0x000054
#define TXPCU_MGMT_SEQ_HW_MASK                                       0x0fff0fff
#define TXPCU_MGMT_SEQ_SW_MASK                                       0x0fff0fff
#define TXPCU_MGMT_SEQ_RSTMASK                                       0x0fff0000
#define TXPCU_MGMT_SEQ_RESET                                         0x02ff0000

// 0x0058 (TXPCU_MISC_MODE)
#define TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_MSB                 6
#define TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_LSB                 6
#define TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_MASK                0x00000040
#define TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_GET(x)              (((x) & TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_MASK) >> TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_LSB)
#define TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_SET(x)              (((0x0 | (x)) << TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_LSB) & TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_MASK)
#define TXPCU_MISC_MODE_TX_CONFIG_PHY_ERR_ENABLE_RESET               1
#define TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_MSB                  5
#define TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_LSB                  5
#define TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_MASK                 0x00000020
#define TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_GET(x)               (((x) & TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_MASK) >> TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_LSB)
#define TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_SET(x)               (((0x0 | (x)) << TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_LSB) & TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_MASK)
#define TXPCU_MISC_MODE_CTS2SELF_AD1_FIX_ENABLE_RESET                1
#define TXPCU_MISC_MODE_HT_DYNAMIC_CBW_MSB                           4
#define TXPCU_MISC_MODE_HT_DYNAMIC_CBW_LSB                           4
#define TXPCU_MISC_MODE_HT_DYNAMIC_CBW_MASK                          0x00000010
#define TXPCU_MISC_MODE_HT_DYNAMIC_CBW_GET(x)                        (((x) & TXPCU_MISC_MODE_HT_DYNAMIC_CBW_MASK) >> TXPCU_MISC_MODE_HT_DYNAMIC_CBW_LSB)
#define TXPCU_MISC_MODE_HT_DYNAMIC_CBW_SET(x)                        (((0x0 | (x)) << TXPCU_MISC_MODE_HT_DYNAMIC_CBW_LSB) & TXPCU_MISC_MODE_HT_DYNAMIC_CBW_MASK)
#define TXPCU_MISC_MODE_HT_DYNAMIC_CBW_RESET                         0
#define TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_MSB                      3
#define TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_LSB                      3
#define TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_MASK                     0x00000008
#define TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_GET(x)                   (((x) & TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_MASK) >> TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_LSB)
#define TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_SET(x)                   (((0x0 | (x)) << TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_LSB) & TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_MASK)
#define TXPCU_MISC_MODE_VHT_SERVICE_DYNAMIC_RESET                    1
#define TXPCU_MISC_MODE_ALLOW_RAC_MSB                                2
#define TXPCU_MISC_MODE_ALLOW_RAC_LSB                                2
#define TXPCU_MISC_MODE_ALLOW_RAC_MASK                               0x00000004
#define TXPCU_MISC_MODE_ALLOW_RAC_GET(x)                             (((x) & TXPCU_MISC_MODE_ALLOW_RAC_MASK) >> TXPCU_MISC_MODE_ALLOW_RAC_LSB)
#define TXPCU_MISC_MODE_ALLOW_RAC_SET(x)                             (((0x0 | (x)) << TXPCU_MISC_MODE_ALLOW_RAC_LSB) & TXPCU_MISC_MODE_ALLOW_RAC_MASK)
#define TXPCU_MISC_MODE_ALLOW_RAC_RESET                              0
#define TXPCU_MISC_MODE_SMOOTHING_FORCE_MSB                          1
#define TXPCU_MISC_MODE_SMOOTHING_FORCE_LSB                          1
#define TXPCU_MISC_MODE_SMOOTHING_FORCE_MASK                         0x00000002
#define TXPCU_MISC_MODE_SMOOTHING_FORCE_GET(x)                       (((x) & TXPCU_MISC_MODE_SMOOTHING_FORCE_MASK) >> TXPCU_MISC_MODE_SMOOTHING_FORCE_LSB)
#define TXPCU_MISC_MODE_SMOOTHING_FORCE_SET(x)                       (((0x0 | (x)) << TXPCU_MISC_MODE_SMOOTHING_FORCE_LSB) & TXPCU_MISC_MODE_SMOOTHING_FORCE_MASK)
#define TXPCU_MISC_MODE_SMOOTHING_FORCE_RESET                        1
#define TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_MSB                       0
#define TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_LSB                       0
#define TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_MASK                      0x00000001
#define TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_GET(x)                    (((x) & TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_MASK) >> TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_LSB)
#define TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_SET(x)                    (((0x0 | (x)) << TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_LSB) & TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_MASK)
#define TXPCU_MISC_MODE_SELF_GEN_SMOOTHING_RESET                     1
#define TXPCU_MISC_MODE_ADDRESS                                      0x000058
#define TXPCU_MISC_MODE_HW_MASK                                      0x0000007f
#define TXPCU_MISC_MODE_SW_MASK                                      0x0000007f
#define TXPCU_MISC_MODE_RSTMASK                                      0x0000006b
#define TXPCU_MISC_MODE_RESET                                        0x0000006b

// 0x005c (TXPCU_RTT_CTRL)
#define TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_MSB                       0
#define TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_LSB                       0
#define TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_MASK                      0x00000001
#define TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_GET(x)                    (((x) & TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_MASK) >> TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_LSB)
#define TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_SET(x)                    (((0x0 | (x)) << TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_LSB) & TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_MASK)
#define TXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_RESET                     0
#define TXPCU_RTT_CTRL_ADDRESS                                       0x00005c
#define TXPCU_RTT_CTRL_HW_MASK                                       0x00000001
#define TXPCU_RTT_CTRL_SW_MASK                                       0x00000001
#define TXPCU_RTT_CTRL_RSTMASK                                       0x00000000
#define TXPCU_RTT_CTRL_RESET                                         0x00000000

// 0x0060 (TXPCU_TRACEBUS_CTRL)
#define TXPCU_TRACEBUS_CTRL_SELECT_MSB                               3
#define TXPCU_TRACEBUS_CTRL_SELECT_LSB                               0
#define TXPCU_TRACEBUS_CTRL_SELECT_MASK                              0x0000000f
#define TXPCU_TRACEBUS_CTRL_SELECT_GET(x)                            (((x) & TXPCU_TRACEBUS_CTRL_SELECT_MASK) >> TXPCU_TRACEBUS_CTRL_SELECT_LSB)
#define TXPCU_TRACEBUS_CTRL_SELECT_SET(x)                            (((0x0 | (x)) << TXPCU_TRACEBUS_CTRL_SELECT_LSB) & TXPCU_TRACEBUS_CTRL_SELECT_MASK)
#define TXPCU_TRACEBUS_CTRL_SELECT_RESET                             0
#define TXPCU_TRACEBUS_CTRL_ADDRESS                                  0x000060
#define TXPCU_TRACEBUS_CTRL_HW_MASK                                  0x0000000f
#define TXPCU_TRACEBUS_CTRL_SW_MASK                                  0x0000000f
#define TXPCU_TRACEBUS_CTRL_RSTMASK                                  0x00000000
#define TXPCU_TRACEBUS_CTRL_RESET                                    0x00000000


#endif /* _MAC_TXPCU_REG_H_ */
