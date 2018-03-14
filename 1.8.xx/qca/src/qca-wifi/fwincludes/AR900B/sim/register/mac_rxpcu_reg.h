//                                                                             
// File:       ./headers/mac_rxpcu_reg.h                                       
// Creator:    afang                                                           
// Time:       Friday Oct 5, 2012 [2:07:17 pm]                                 
//                                                                             
// Path:       /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl
// Arguments:  /cad/denali/blueprint/3.7.4//Linux-64bit/blueprint -odir        
//             ./headers -codegen ath_ch.codegen -ath_ch -Wdesc                
//             mac_rxpcu_reg.rdl                                               
//                                                                             
// Sources:    /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl/mac_rxpcu_reg.rdl
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


#ifndef _MAC_RXPCU_REG_H_
#define _MAC_RXPCU_REG_H_

// 0x0000 (RXPCU_BSSID2_L32)
#define RXPCU_BSSID2_L32_ADDR_MSB                                    31
#define RXPCU_BSSID2_L32_ADDR_LSB                                    0
#define RXPCU_BSSID2_L32_ADDR_MASK                                   0xffffffff
#define RXPCU_BSSID2_L32_ADDR_GET(x)                                 (((x) & RXPCU_BSSID2_L32_ADDR_MASK) >> RXPCU_BSSID2_L32_ADDR_LSB)
#define RXPCU_BSSID2_L32_ADDR_SET(x)                                 (((0x0 | (x)) << RXPCU_BSSID2_L32_ADDR_LSB) & RXPCU_BSSID2_L32_ADDR_MASK)
#define RXPCU_BSSID2_L32_ADDR_RESET                                  0
#define RXPCU_BSSID2_L32_ADDRESS                                     0x000000
#define RXPCU_BSSID2_L32_HW_MASK                                     0xffffffff
#define RXPCU_BSSID2_L32_SW_MASK                                     0xffffffff
#define RXPCU_BSSID2_L32_RSTMASK                                     0x00000000
#define RXPCU_BSSID2_L32_RESET                                       0x00000000

// 0x0004 (RXPCU_BSSID2_U16)
#define RXPCU_BSSID2_U16_AID_MSB                                     30
#define RXPCU_BSSID2_U16_AID_LSB                                     20
#define RXPCU_BSSID2_U16_AID_MASK                                    0x7ff00000
#define RXPCU_BSSID2_U16_AID_GET(x)                                  (((x) & RXPCU_BSSID2_U16_AID_MASK) >> RXPCU_BSSID2_U16_AID_LSB)
#define RXPCU_BSSID2_U16_AID_SET(x)                                  (((0x0 | (x)) << RXPCU_BSSID2_U16_AID_LSB) & RXPCU_BSSID2_U16_AID_MASK)
#define RXPCU_BSSID2_U16_AID_RESET                                   0
#define RXPCU_BSSID2_U16_ENABLE_MSB                                  16
#define RXPCU_BSSID2_U16_ENABLE_LSB                                  16
#define RXPCU_BSSID2_U16_ENABLE_MASK                                 0x00010000
#define RXPCU_BSSID2_U16_ENABLE_GET(x)                               (((x) & RXPCU_BSSID2_U16_ENABLE_MASK) >> RXPCU_BSSID2_U16_ENABLE_LSB)
#define RXPCU_BSSID2_U16_ENABLE_SET(x)                               (((0x0 | (x)) << RXPCU_BSSID2_U16_ENABLE_LSB) & RXPCU_BSSID2_U16_ENABLE_MASK)
#define RXPCU_BSSID2_U16_ENABLE_RESET                                0
#define RXPCU_BSSID2_U16_ADDR_MSB                                    15
#define RXPCU_BSSID2_U16_ADDR_LSB                                    0
#define RXPCU_BSSID2_U16_ADDR_MASK                                   0x0000ffff
#define RXPCU_BSSID2_U16_ADDR_GET(x)                                 (((x) & RXPCU_BSSID2_U16_ADDR_MASK) >> RXPCU_BSSID2_U16_ADDR_LSB)
#define RXPCU_BSSID2_U16_ADDR_SET(x)                                 (((0x0 | (x)) << RXPCU_BSSID2_U16_ADDR_LSB) & RXPCU_BSSID2_U16_ADDR_MASK)
#define RXPCU_BSSID2_U16_ADDR_RESET                                  0
#define RXPCU_BSSID2_U16_ADDRESS                                     0x000004
#define RXPCU_BSSID2_U16_HW_MASK                                     0x7ff1ffff
#define RXPCU_BSSID2_U16_SW_MASK                                     0x7ff1ffff
#define RXPCU_BSSID2_U16_RSTMASK                                     0x00000000
#define RXPCU_BSSID2_U16_RESET                                       0x00000000

// 0x0008 (RXPCU_BSSID3_L32)
#define RXPCU_BSSID3_L32_ADDR_MSB                                    31
#define RXPCU_BSSID3_L32_ADDR_LSB                                    0
#define RXPCU_BSSID3_L32_ADDR_MASK                                   0xffffffff
#define RXPCU_BSSID3_L32_ADDR_GET(x)                                 (((x) & RXPCU_BSSID3_L32_ADDR_MASK) >> RXPCU_BSSID3_L32_ADDR_LSB)
#define RXPCU_BSSID3_L32_ADDR_SET(x)                                 (((0x0 | (x)) << RXPCU_BSSID3_L32_ADDR_LSB) & RXPCU_BSSID3_L32_ADDR_MASK)
#define RXPCU_BSSID3_L32_ADDR_RESET                                  0
#define RXPCU_BSSID3_L32_ADDRESS                                     0x000008
#define RXPCU_BSSID3_L32_HW_MASK                                     0xffffffff
#define RXPCU_BSSID3_L32_SW_MASK                                     0xffffffff
#define RXPCU_BSSID3_L32_RSTMASK                                     0x00000000
#define RXPCU_BSSID3_L32_RESET                                       0x00000000

// 0x000c (RXPCU_BSSID3_U16)
#define RXPCU_BSSID3_U16_ENABLE_MSB                                  16
#define RXPCU_BSSID3_U16_ENABLE_LSB                                  16
#define RXPCU_BSSID3_U16_ENABLE_MASK                                 0x00010000
#define RXPCU_BSSID3_U16_ENABLE_GET(x)                               (((x) & RXPCU_BSSID3_U16_ENABLE_MASK) >> RXPCU_BSSID3_U16_ENABLE_LSB)
#define RXPCU_BSSID3_U16_ENABLE_SET(x)                               (((0x0 | (x)) << RXPCU_BSSID3_U16_ENABLE_LSB) & RXPCU_BSSID3_U16_ENABLE_MASK)
#define RXPCU_BSSID3_U16_ENABLE_RESET                                0
#define RXPCU_BSSID3_U16_ADDR_MSB                                    15
#define RXPCU_BSSID3_U16_ADDR_LSB                                    0
#define RXPCU_BSSID3_U16_ADDR_MASK                                   0x0000ffff
#define RXPCU_BSSID3_U16_ADDR_GET(x)                                 (((x) & RXPCU_BSSID3_U16_ADDR_MASK) >> RXPCU_BSSID3_U16_ADDR_LSB)
#define RXPCU_BSSID3_U16_ADDR_SET(x)                                 (((0x0 | (x)) << RXPCU_BSSID3_U16_ADDR_LSB) & RXPCU_BSSID3_U16_ADDR_MASK)
#define RXPCU_BSSID3_U16_ADDR_RESET                                  0
#define RXPCU_BSSID3_U16_ADDRESS                                     0x00000c
#define RXPCU_BSSID3_U16_HW_MASK                                     0x0001ffff
#define RXPCU_BSSID3_U16_SW_MASK                                     0x0001ffff
#define RXPCU_BSSID3_U16_RSTMASK                                     0x00000000
#define RXPCU_BSSID3_U16_RESET                                       0x00000000

// 0x0010 (RXPCU_BSSID4_L32)
#define RXPCU_BSSID4_L32_ADDR_MSB                                    31
#define RXPCU_BSSID4_L32_ADDR_LSB                                    0
#define RXPCU_BSSID4_L32_ADDR_MASK                                   0xffffffff
#define RXPCU_BSSID4_L32_ADDR_GET(x)                                 (((x) & RXPCU_BSSID4_L32_ADDR_MASK) >> RXPCU_BSSID4_L32_ADDR_LSB)
#define RXPCU_BSSID4_L32_ADDR_SET(x)                                 (((0x0 | (x)) << RXPCU_BSSID4_L32_ADDR_LSB) & RXPCU_BSSID4_L32_ADDR_MASK)
#define RXPCU_BSSID4_L32_ADDR_RESET                                  0
#define RXPCU_BSSID4_L32_ADDRESS                                     0x000010
#define RXPCU_BSSID4_L32_HW_MASK                                     0xffffffff
#define RXPCU_BSSID4_L32_SW_MASK                                     0xffffffff
#define RXPCU_BSSID4_L32_RSTMASK                                     0x00000000
#define RXPCU_BSSID4_L32_RESET                                       0x00000000

// 0x0014 (RXPCU_BSSID4_U16)
#define RXPCU_BSSID4_U16_ENABLE_MSB                                  16
#define RXPCU_BSSID4_U16_ENABLE_LSB                                  16
#define RXPCU_BSSID4_U16_ENABLE_MASK                                 0x00010000
#define RXPCU_BSSID4_U16_ENABLE_GET(x)                               (((x) & RXPCU_BSSID4_U16_ENABLE_MASK) >> RXPCU_BSSID4_U16_ENABLE_LSB)
#define RXPCU_BSSID4_U16_ENABLE_SET(x)                               (((0x0 | (x)) << RXPCU_BSSID4_U16_ENABLE_LSB) & RXPCU_BSSID4_U16_ENABLE_MASK)
#define RXPCU_BSSID4_U16_ENABLE_RESET                                0
#define RXPCU_BSSID4_U16_ADDR_MSB                                    15
#define RXPCU_BSSID4_U16_ADDR_LSB                                    0
#define RXPCU_BSSID4_U16_ADDR_MASK                                   0x0000ffff
#define RXPCU_BSSID4_U16_ADDR_GET(x)                                 (((x) & RXPCU_BSSID4_U16_ADDR_MASK) >> RXPCU_BSSID4_U16_ADDR_LSB)
#define RXPCU_BSSID4_U16_ADDR_SET(x)                                 (((0x0 | (x)) << RXPCU_BSSID4_U16_ADDR_LSB) & RXPCU_BSSID4_U16_ADDR_MASK)
#define RXPCU_BSSID4_U16_ADDR_RESET                                  0
#define RXPCU_BSSID4_U16_ADDRESS                                     0x000014
#define RXPCU_BSSID4_U16_HW_MASK                                     0x0001ffff
#define RXPCU_BSSID4_U16_SW_MASK                                     0x0001ffff
#define RXPCU_BSSID4_U16_RSTMASK                                     0x00000000
#define RXPCU_BSSID4_U16_RESET                                       0x00000000

// 0x0018 (RXPCU_ADDR1_MASK_L32)
#define RXPCU_ADDR1_MASK_L32_VALUE_MSB                               31
#define RXPCU_ADDR1_MASK_L32_VALUE_LSB                               0
#define RXPCU_ADDR1_MASK_L32_VALUE_MASK                              0xffffffff
#define RXPCU_ADDR1_MASK_L32_VALUE_GET(x)                            (((x) & RXPCU_ADDR1_MASK_L32_VALUE_MASK) >> RXPCU_ADDR1_MASK_L32_VALUE_LSB)
#define RXPCU_ADDR1_MASK_L32_VALUE_SET(x)                            (((0x0 | (x)) << RXPCU_ADDR1_MASK_L32_VALUE_LSB) & RXPCU_ADDR1_MASK_L32_VALUE_MASK)
#define RXPCU_ADDR1_MASK_L32_VALUE_RESET                             4294967295
#define RXPCU_ADDR1_MASK_L32_ADDRESS                                 0x000018
#define RXPCU_ADDR1_MASK_L32_HW_MASK                                 0xffffffff
#define RXPCU_ADDR1_MASK_L32_SW_MASK                                 0xffffffff
#define RXPCU_ADDR1_MASK_L32_RSTMASK                                 0xffffffff
#define RXPCU_ADDR1_MASK_L32_RESET                                   0xffffffff

// 0x001c (RXPCU_ADDR1_MASK_U16)
#define RXPCU_ADDR1_MASK_U16_VALUE_MSB                               15
#define RXPCU_ADDR1_MASK_U16_VALUE_LSB                               0
#define RXPCU_ADDR1_MASK_U16_VALUE_MASK                              0x0000ffff
#define RXPCU_ADDR1_MASK_U16_VALUE_GET(x)                            (((x) & RXPCU_ADDR1_MASK_U16_VALUE_MASK) >> RXPCU_ADDR1_MASK_U16_VALUE_LSB)
#define RXPCU_ADDR1_MASK_U16_VALUE_SET(x)                            (((0x0 | (x)) << RXPCU_ADDR1_MASK_U16_VALUE_LSB) & RXPCU_ADDR1_MASK_U16_VALUE_MASK)
#define RXPCU_ADDR1_MASK_U16_VALUE_RESET                             65535
#define RXPCU_ADDR1_MASK_U16_ADDRESS                                 0x00001c
#define RXPCU_ADDR1_MASK_U16_HW_MASK                                 0x0000ffff
#define RXPCU_ADDR1_MASK_U16_SW_MASK                                 0x0000ffff
#define RXPCU_ADDR1_MASK_U16_RSTMASK                                 0x0000ffff
#define RXPCU_ADDR1_MASK_U16_RESET                                   0x0000ffff

// 0x0020 (RXPCU_MCAST_FILTER_L32)
#define RXPCU_MCAST_FILTER_L32_VALUE_MSB                             31
#define RXPCU_MCAST_FILTER_L32_VALUE_LSB                             0
#define RXPCU_MCAST_FILTER_L32_VALUE_MASK                            0xffffffff
#define RXPCU_MCAST_FILTER_L32_VALUE_GET(x)                          (((x) & RXPCU_MCAST_FILTER_L32_VALUE_MASK) >> RXPCU_MCAST_FILTER_L32_VALUE_LSB)
#define RXPCU_MCAST_FILTER_L32_VALUE_SET(x)                          (((0x0 | (x)) << RXPCU_MCAST_FILTER_L32_VALUE_LSB) & RXPCU_MCAST_FILTER_L32_VALUE_MASK)
#define RXPCU_MCAST_FILTER_L32_VALUE_RESET                           0
#define RXPCU_MCAST_FILTER_L32_ADDRESS                               0x000020
#define RXPCU_MCAST_FILTER_L32_HW_MASK                               0xffffffff
#define RXPCU_MCAST_FILTER_L32_SW_MASK                               0xffffffff
#define RXPCU_MCAST_FILTER_L32_RSTMASK                               0x00000000
#define RXPCU_MCAST_FILTER_L32_RESET                                 0x00000000

// 0x0024 (RXPCU_MCAST_FILTER_U32)
#define RXPCU_MCAST_FILTER_U32_VALUE_MSB                             31
#define RXPCU_MCAST_FILTER_U32_VALUE_LSB                             0
#define RXPCU_MCAST_FILTER_U32_VALUE_MASK                            0xffffffff
#define RXPCU_MCAST_FILTER_U32_VALUE_GET(x)                          (((x) & RXPCU_MCAST_FILTER_U32_VALUE_MASK) >> RXPCU_MCAST_FILTER_U32_VALUE_LSB)
#define RXPCU_MCAST_FILTER_U32_VALUE_SET(x)                          (((0x0 | (x)) << RXPCU_MCAST_FILTER_U32_VALUE_LSB) & RXPCU_MCAST_FILTER_U32_VALUE_MASK)
#define RXPCU_MCAST_FILTER_U32_VALUE_RESET                           0
#define RXPCU_MCAST_FILTER_U32_ADDRESS                               0x000024
#define RXPCU_MCAST_FILTER_U32_HW_MASK                               0xffffffff
#define RXPCU_MCAST_FILTER_U32_SW_MASK                               0xffffffff
#define RXPCU_MCAST_FILTER_U32_RSTMASK                               0x00000000
#define RXPCU_MCAST_FILTER_U32_RESET                                 0x00000000

// 0x0028 (RXPCU_RX_FILTER)
#define RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_MSB                    31
#define RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_LSB                    31
#define RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_MASK                   0x80000000
#define RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_GET(x)                 (((x) & RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_MASK) >> RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_LSB)
#define RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_SET(x)                 (((0x0 | (x)) << RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_LSB) & RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_MASK)
#define RXPCU_RX_FILTER_BSSID_BASED_MULTICAST_RESET                  0
#define RXPCU_RX_FILTER_PHY_DATA_MSB                                 30
#define RXPCU_RX_FILTER_PHY_DATA_LSB                                 30
#define RXPCU_RX_FILTER_PHY_DATA_MASK                                0x40000000
#define RXPCU_RX_FILTER_PHY_DATA_GET(x)                              (((x) & RXPCU_RX_FILTER_PHY_DATA_MASK) >> RXPCU_RX_FILTER_PHY_DATA_LSB)
#define RXPCU_RX_FILTER_PHY_DATA_SET(x)                              (((0x0 | (x)) << RXPCU_RX_FILTER_PHY_DATA_LSB) & RXPCU_RX_FILTER_PHY_DATA_MASK)
#define RXPCU_RX_FILTER_PHY_DATA_RESET                               0
#define RXPCU_RX_FILTER_MY_BEACON2_MSB                               29
#define RXPCU_RX_FILTER_MY_BEACON2_LSB                               29
#define RXPCU_RX_FILTER_MY_BEACON2_MASK                              0x20000000
#define RXPCU_RX_FILTER_MY_BEACON2_GET(x)                            (((x) & RXPCU_RX_FILTER_MY_BEACON2_MASK) >> RXPCU_RX_FILTER_MY_BEACON2_LSB)
#define RXPCU_RX_FILTER_MY_BEACON2_SET(x)                            (((0x0 | (x)) << RXPCU_RX_FILTER_MY_BEACON2_LSB) & RXPCU_RX_FILTER_MY_BEACON2_MASK)
#define RXPCU_RX_FILTER_MY_BEACON2_RESET                             0
#define RXPCU_RX_FILTER_GENERIC_FILTER_MSB                           28
#define RXPCU_RX_FILTER_GENERIC_FILTER_LSB                           27
#define RXPCU_RX_FILTER_GENERIC_FILTER_MASK                          0x18000000
#define RXPCU_RX_FILTER_GENERIC_FILTER_GET(x)                        (((x) & RXPCU_RX_FILTER_GENERIC_FILTER_MASK) >> RXPCU_RX_FILTER_GENERIC_FILTER_LSB)
#define RXPCU_RX_FILTER_GENERIC_FILTER_SET(x)                        (((0x0 | (x)) << RXPCU_RX_FILTER_GENERIC_FILTER_LSB) & RXPCU_RX_FILTER_GENERIC_FILTER_MASK)
#define RXPCU_RX_FILTER_GENERIC_FILTER_RESET                         0
#define RXPCU_RX_FILTER_GENERIC_FTYPE_MSB                            26
#define RXPCU_RX_FILTER_GENERIC_FTYPE_LSB                            21
#define RXPCU_RX_FILTER_GENERIC_FTYPE_MASK                           0x07e00000
#define RXPCU_RX_FILTER_GENERIC_FTYPE_GET(x)                         (((x) & RXPCU_RX_FILTER_GENERIC_FTYPE_MASK) >> RXPCU_RX_FILTER_GENERIC_FTYPE_LSB)
#define RXPCU_RX_FILTER_GENERIC_FTYPE_SET(x)                         (((0x0 | (x)) << RXPCU_RX_FILTER_GENERIC_FTYPE_LSB) & RXPCU_RX_FILTER_GENERIC_FTYPE_MASK)
#define RXPCU_RX_FILTER_GENERIC_FTYPE_RESET                          0
#define RXPCU_RX_FILTER_FROM_TO_DS_MSB                               20
#define RXPCU_RX_FILTER_FROM_TO_DS_LSB                               20
#define RXPCU_RX_FILTER_FROM_TO_DS_MASK                              0x00100000
#define RXPCU_RX_FILTER_FROM_TO_DS_GET(x)                            (((x) & RXPCU_RX_FILTER_FROM_TO_DS_MASK) >> RXPCU_RX_FILTER_FROM_TO_DS_LSB)
#define RXPCU_RX_FILTER_FROM_TO_DS_SET(x)                            (((0x0 | (x)) << RXPCU_RX_FILTER_FROM_TO_DS_LSB) & RXPCU_RX_FILTER_FROM_TO_DS_MASK)
#define RXPCU_RX_FILTER_FROM_TO_DS_RESET                             0
#define RXPCU_RX_FILTER_CONTROL_WRAPPER_MSB                          19
#define RXPCU_RX_FILTER_CONTROL_WRAPPER_LSB                          19
#define RXPCU_RX_FILTER_CONTROL_WRAPPER_MASK                         0x00080000
#define RXPCU_RX_FILTER_CONTROL_WRAPPER_GET(x)                       (((x) & RXPCU_RX_FILTER_CONTROL_WRAPPER_MASK) >> RXPCU_RX_FILTER_CONTROL_WRAPPER_LSB)
#define RXPCU_RX_FILTER_CONTROL_WRAPPER_SET(x)                       (((0x0 | (x)) << RXPCU_RX_FILTER_CONTROL_WRAPPER_LSB) & RXPCU_RX_FILTER_CONTROL_WRAPPER_MASK)
#define RXPCU_RX_FILTER_CONTROL_WRAPPER_RESET                        0
#define RXPCU_RX_FILTER_MGMT_ACTION_MCAST_MSB                        18
#define RXPCU_RX_FILTER_MGMT_ACTION_MCAST_LSB                        18
#define RXPCU_RX_FILTER_MGMT_ACTION_MCAST_MASK                       0x00040000
#define RXPCU_RX_FILTER_MGMT_ACTION_MCAST_GET(x)                     (((x) & RXPCU_RX_FILTER_MGMT_ACTION_MCAST_MASK) >> RXPCU_RX_FILTER_MGMT_ACTION_MCAST_LSB)
#define RXPCU_RX_FILTER_MGMT_ACTION_MCAST_SET(x)                     (((0x0 | (x)) << RXPCU_RX_FILTER_MGMT_ACTION_MCAST_LSB) & RXPCU_RX_FILTER_MGMT_ACTION_MCAST_MASK)
#define RXPCU_RX_FILTER_MGMT_ACTION_MCAST_RESET                      0
#define RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_MSB                       17
#define RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_LSB                       17
#define RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_MASK                      0x00020000
#define RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_GET(x)                    (((x) & RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_MASK) >> RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_LSB)
#define RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_SET(x)                    (((0x0 | (x)) << RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_LSB) & RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_MASK)
#define RXPCU_RX_FILTER_HW_BCN_PROC_ENABLE_RESET                     0
#define RXPCU_RX_FILTER_MCAST_BCAST_ALL_MSB                          15
#define RXPCU_RX_FILTER_MCAST_BCAST_ALL_LSB                          15
#define RXPCU_RX_FILTER_MCAST_BCAST_ALL_MASK                         0x00008000
#define RXPCU_RX_FILTER_MCAST_BCAST_ALL_GET(x)                       (((x) & RXPCU_RX_FILTER_MCAST_BCAST_ALL_MASK) >> RXPCU_RX_FILTER_MCAST_BCAST_ALL_LSB)
#define RXPCU_RX_FILTER_MCAST_BCAST_ALL_SET(x)                       (((0x0 | (x)) << RXPCU_RX_FILTER_MCAST_BCAST_ALL_LSB) & RXPCU_RX_FILTER_MCAST_BCAST_ALL_MASK)
#define RXPCU_RX_FILTER_MCAST_BCAST_ALL_RESET                        0
#define RXPCU_RX_FILTER_PS_POLL_MSB                                  14
#define RXPCU_RX_FILTER_PS_POLL_LSB                                  14
#define RXPCU_RX_FILTER_PS_POLL_MASK                                 0x00004000
#define RXPCU_RX_FILTER_PS_POLL_GET(x)                               (((x) & RXPCU_RX_FILTER_PS_POLL_MASK) >> RXPCU_RX_FILTER_PS_POLL_LSB)
#define RXPCU_RX_FILTER_PS_POLL_SET(x)                               (((0x0 | (x)) << RXPCU_RX_FILTER_PS_POLL_LSB) & RXPCU_RX_FILTER_PS_POLL_MASK)
#define RXPCU_RX_FILTER_PS_POLL_RESET                                0
#define RXPCU_RX_FILTER_RESERVED_0_MSB                               13
#define RXPCU_RX_FILTER_RESERVED_0_LSB                               13
#define RXPCU_RX_FILTER_RESERVED_0_MASK                              0x00002000
#define RXPCU_RX_FILTER_RESERVED_0_GET(x)                            (((x) & RXPCU_RX_FILTER_RESERVED_0_MASK) >> RXPCU_RX_FILTER_RESERVED_0_LSB)
#define RXPCU_RX_FILTER_RESERVED_0_SET(x)                            (((0x0 | (x)) << RXPCU_RX_FILTER_RESERVED_0_LSB) & RXPCU_RX_FILTER_RESERVED_0_MASK)
#define RXPCU_RX_FILTER_RESERVED_0_RESET                             0
#define RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_MSB                      12
#define RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_LSB                      12
#define RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_MASK                     0x00001000
#define RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_GET(x)                   (((x) & RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_MASK) >> RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_LSB)
#define RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_SET(x)                   (((0x0 | (x)) << RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_LSB) & RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_MASK)
#define RXPCU_RX_FILTER_UNCOMPRESSED_BA_BAR_RESET                    0
#define RXPCU_RX_FILTER_COMPRESSED_BA_MSB                            11
#define RXPCU_RX_FILTER_COMPRESSED_BA_LSB                            11
#define RXPCU_RX_FILTER_COMPRESSED_BA_MASK                           0x00000800
#define RXPCU_RX_FILTER_COMPRESSED_BA_GET(x)                         (((x) & RXPCU_RX_FILTER_COMPRESSED_BA_MASK) >> RXPCU_RX_FILTER_COMPRESSED_BA_LSB)
#define RXPCU_RX_FILTER_COMPRESSED_BA_SET(x)                         (((0x0 | (x)) << RXPCU_RX_FILTER_COMPRESSED_BA_LSB) & RXPCU_RX_FILTER_COMPRESSED_BA_MASK)
#define RXPCU_RX_FILTER_COMPRESSED_BA_RESET                          0
#define RXPCU_RX_FILTER_COMPRESSED_BAR_MSB                           10
#define RXPCU_RX_FILTER_COMPRESSED_BAR_LSB                           10
#define RXPCU_RX_FILTER_COMPRESSED_BAR_MASK                          0x00000400
#define RXPCU_RX_FILTER_COMPRESSED_BAR_GET(x)                        (((x) & RXPCU_RX_FILTER_COMPRESSED_BAR_MASK) >> RXPCU_RX_FILTER_COMPRESSED_BAR_LSB)
#define RXPCU_RX_FILTER_COMPRESSED_BAR_SET(x)                        (((0x0 | (x)) << RXPCU_RX_FILTER_COMPRESSED_BAR_LSB) & RXPCU_RX_FILTER_COMPRESSED_BAR_MASK)
#define RXPCU_RX_FILTER_COMPRESSED_BAR_RESET                         0
#define RXPCU_RX_FILTER_MY_BEACON_MSB                                9
#define RXPCU_RX_FILTER_MY_BEACON_LSB                                9
#define RXPCU_RX_FILTER_MY_BEACON_MASK                               0x00000200
#define RXPCU_RX_FILTER_MY_BEACON_GET(x)                             (((x) & RXPCU_RX_FILTER_MY_BEACON_MASK) >> RXPCU_RX_FILTER_MY_BEACON_LSB)
#define RXPCU_RX_FILTER_MY_BEACON_SET(x)                             (((0x0 | (x)) << RXPCU_RX_FILTER_MY_BEACON_LSB) & RXPCU_RX_FILTER_MY_BEACON_MASK)
#define RXPCU_RX_FILTER_MY_BEACON_RESET                              0
#define RXPCU_RX_FILTER_PROBE_REQ_MSB                                7
#define RXPCU_RX_FILTER_PROBE_REQ_LSB                                7
#define RXPCU_RX_FILTER_PROBE_REQ_MASK                               0x00000080
#define RXPCU_RX_FILTER_PROBE_REQ_GET(x)                             (((x) & RXPCU_RX_FILTER_PROBE_REQ_MASK) >> RXPCU_RX_FILTER_PROBE_REQ_LSB)
#define RXPCU_RX_FILTER_PROBE_REQ_SET(x)                             (((0x0 | (x)) << RXPCU_RX_FILTER_PROBE_REQ_LSB) & RXPCU_RX_FILTER_PROBE_REQ_MASK)
#define RXPCU_RX_FILTER_PROBE_REQ_RESET                              0
#define RXPCU_RX_FILTER_PROMISCUOUS_MSB                              5
#define RXPCU_RX_FILTER_PROMISCUOUS_LSB                              5
#define RXPCU_RX_FILTER_PROMISCUOUS_MASK                             0x00000020
#define RXPCU_RX_FILTER_PROMISCUOUS_GET(x)                           (((x) & RXPCU_RX_FILTER_PROMISCUOUS_MASK) >> RXPCU_RX_FILTER_PROMISCUOUS_LSB)
#define RXPCU_RX_FILTER_PROMISCUOUS_SET(x)                           (((0x0 | (x)) << RXPCU_RX_FILTER_PROMISCUOUS_LSB) & RXPCU_RX_FILTER_PROMISCUOUS_MASK)
#define RXPCU_RX_FILTER_PROMISCUOUS_RESET                            0
#define RXPCU_RX_FILTER_BEACON_MSB                                   4
#define RXPCU_RX_FILTER_BEACON_LSB                                   4
#define RXPCU_RX_FILTER_BEACON_MASK                                  0x00000010
#define RXPCU_RX_FILTER_BEACON_GET(x)                                (((x) & RXPCU_RX_FILTER_BEACON_MASK) >> RXPCU_RX_FILTER_BEACON_LSB)
#define RXPCU_RX_FILTER_BEACON_SET(x)                                (((0x0 | (x)) << RXPCU_RX_FILTER_BEACON_LSB) & RXPCU_RX_FILTER_BEACON_MASK)
#define RXPCU_RX_FILTER_BEACON_RESET                                 0
#define RXPCU_RX_FILTER_CONTROL_MSB                                  3
#define RXPCU_RX_FILTER_CONTROL_LSB                                  3
#define RXPCU_RX_FILTER_CONTROL_MASK                                 0x00000008
#define RXPCU_RX_FILTER_CONTROL_GET(x)                               (((x) & RXPCU_RX_FILTER_CONTROL_MASK) >> RXPCU_RX_FILTER_CONTROL_LSB)
#define RXPCU_RX_FILTER_CONTROL_SET(x)                               (((0x0 | (x)) << RXPCU_RX_FILTER_CONTROL_LSB) & RXPCU_RX_FILTER_CONTROL_MASK)
#define RXPCU_RX_FILTER_CONTROL_RESET                                0
#define RXPCU_RX_FILTER_BROADCAST_MSB                                2
#define RXPCU_RX_FILTER_BROADCAST_LSB                                2
#define RXPCU_RX_FILTER_BROADCAST_MASK                               0x00000004
#define RXPCU_RX_FILTER_BROADCAST_GET(x)                             (((x) & RXPCU_RX_FILTER_BROADCAST_MASK) >> RXPCU_RX_FILTER_BROADCAST_LSB)
#define RXPCU_RX_FILTER_BROADCAST_SET(x)                             (((0x0 | (x)) << RXPCU_RX_FILTER_BROADCAST_LSB) & RXPCU_RX_FILTER_BROADCAST_MASK)
#define RXPCU_RX_FILTER_BROADCAST_RESET                              0
#define RXPCU_RX_FILTER_MULTICAST_MSB                                1
#define RXPCU_RX_FILTER_MULTICAST_LSB                                1
#define RXPCU_RX_FILTER_MULTICAST_MASK                               0x00000002
#define RXPCU_RX_FILTER_MULTICAST_GET(x)                             (((x) & RXPCU_RX_FILTER_MULTICAST_MASK) >> RXPCU_RX_FILTER_MULTICAST_LSB)
#define RXPCU_RX_FILTER_MULTICAST_SET(x)                             (((0x0 | (x)) << RXPCU_RX_FILTER_MULTICAST_LSB) & RXPCU_RX_FILTER_MULTICAST_MASK)
#define RXPCU_RX_FILTER_MULTICAST_RESET                              0
#define RXPCU_RX_FILTER_UNICAST_MSB                                  0
#define RXPCU_RX_FILTER_UNICAST_LSB                                  0
#define RXPCU_RX_FILTER_UNICAST_MASK                                 0x00000001
#define RXPCU_RX_FILTER_UNICAST_GET(x)                               (((x) & RXPCU_RX_FILTER_UNICAST_MASK) >> RXPCU_RX_FILTER_UNICAST_LSB)
#define RXPCU_RX_FILTER_UNICAST_SET(x)                               (((0x0 | (x)) << RXPCU_RX_FILTER_UNICAST_LSB) & RXPCU_RX_FILTER_UNICAST_MASK)
#define RXPCU_RX_FILTER_UNICAST_RESET                                0
#define RXPCU_RX_FILTER_ADDRESS                                      0x000028
#define RXPCU_RX_FILTER_HW_MASK                                      0xfffefebf
#define RXPCU_RX_FILTER_SW_MASK                                      0xfffefebf
#define RXPCU_RX_FILTER_RSTMASK                                      0x00000000
#define RXPCU_RX_FILTER_RESET                                        0x00000000

// 0x002c (RXPCU_RX_FILTER2)
#define RXPCU_RX_FILTER2_BRPOLL_MSB                                  5
#define RXPCU_RX_FILTER2_BRPOLL_LSB                                  5
#define RXPCU_RX_FILTER2_BRPOLL_MASK                                 0x00000020
#define RXPCU_RX_FILTER2_BRPOLL_GET(x)                               (((x) & RXPCU_RX_FILTER2_BRPOLL_MASK) >> RXPCU_RX_FILTER2_BRPOLL_LSB)
#define RXPCU_RX_FILTER2_BRPOLL_SET(x)                               (((0x0 | (x)) << RXPCU_RX_FILTER2_BRPOLL_LSB) & RXPCU_RX_FILTER2_BRPOLL_MASK)
#define RXPCU_RX_FILTER2_BRPOLL_RESET                                0
#define RXPCU_RX_FILTER2_NDPA_MSB                                    4
#define RXPCU_RX_FILTER2_NDPA_LSB                                    4
#define RXPCU_RX_FILTER2_NDPA_MASK                                   0x00000010
#define RXPCU_RX_FILTER2_NDPA_GET(x)                                 (((x) & RXPCU_RX_FILTER2_NDPA_MASK) >> RXPCU_RX_FILTER2_NDPA_LSB)
#define RXPCU_RX_FILTER2_NDPA_SET(x)                                 (((0x0 | (x)) << RXPCU_RX_FILTER2_NDPA_LSB) & RXPCU_RX_FILTER2_NDPA_MASK)
#define RXPCU_RX_FILTER2_NDPA_RESET                                  0
#define RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_MSB                     3
#define RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_LSB                     0
#define RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_MASK                    0x0000000f
#define RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_GET(x)                  (((x) & RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_MASK) >> RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_LSB)
#define RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_SET(x)                  (((0x0 | (x)) << RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_LSB) & RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_MASK)
#define RXPCU_RX_FILTER2_BSSID_BASED_UNICAST_RESET                   0
#define RXPCU_RX_FILTER2_ADDRESS                                     0x00002c
#define RXPCU_RX_FILTER2_HW_MASK                                     0x0000003f
#define RXPCU_RX_FILTER2_SW_MASK                                     0x0000003f
#define RXPCU_RX_FILTER2_RSTMASK                                     0x00000000
#define RXPCU_RX_FILTER2_RESET                                       0x00000000

// 0x0030 (RXPCU_PHY_DATA_LENGTH_THRESH)
#define RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_MSB                      16
#define RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_LSB                      16
#define RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_MASK                     0x00010000
#define RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_GET(x)                   (((x) & RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_MASK) >> RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_LSB)
#define RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_SET(x)                   (((0x0 | (x)) << RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_LSB) & RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_MASK)
#define RXPCU_PHY_DATA_LENGTH_THRESH_ENABLE_RESET                    0
#define RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_MSB                       13
#define RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_LSB                       0
#define RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_MASK                      0x00003fff
#define RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_GET(x)                    (((x) & RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_MASK) >> RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_LSB)
#define RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_SET(x)                    (((0x0 | (x)) << RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_LSB) & RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_MASK)
#define RXPCU_PHY_DATA_LENGTH_THRESH_VALUE_RESET                     100
#define RXPCU_PHY_DATA_LENGTH_THRESH_ADDRESS                         0x000030
#define RXPCU_PHY_DATA_LENGTH_THRESH_HW_MASK                         0x00013fff
#define RXPCU_PHY_DATA_LENGTH_THRESH_SW_MASK                         0x00013fff
#define RXPCU_PHY_DATA_LENGTH_THRESH_RSTMASK                         0x00003fff
#define RXPCU_PHY_DATA_LENGTH_THRESH_RESET                           0x00000064

// 0x0034 (RXPCU_BCN_RSSI_CTL)
#define RXPCU_BCN_RSSI_CTL_RESET_MSB                                 29
#define RXPCU_BCN_RSSI_CTL_RESET_LSB                                 29
#define RXPCU_BCN_RSSI_CTL_RESET_MASK                                0x20000000
#define RXPCU_BCN_RSSI_CTL_RESET_GET(x)                              (((x) & RXPCU_BCN_RSSI_CTL_RESET_MASK) >> RXPCU_BCN_RSSI_CTL_RESET_LSB)
#define RXPCU_BCN_RSSI_CTL_RESET_SET(x)                              (((0x0 | (x)) << RXPCU_BCN_RSSI_CTL_RESET_LSB) & RXPCU_BCN_RSSI_CTL_RESET_MASK)
#define RXPCU_BCN_RSSI_CTL_RESET_RESET                               0
#define RXPCU_BCN_RSSI_CTL_WEIGHT_MSB                                28
#define RXPCU_BCN_RSSI_CTL_WEIGHT_LSB                                24
#define RXPCU_BCN_RSSI_CTL_WEIGHT_MASK                               0x1f000000
#define RXPCU_BCN_RSSI_CTL_WEIGHT_GET(x)                             (((x) & RXPCU_BCN_RSSI_CTL_WEIGHT_MASK) >> RXPCU_BCN_RSSI_CTL_WEIGHT_LSB)
#define RXPCU_BCN_RSSI_CTL_WEIGHT_SET(x)                             (((0x0 | (x)) << RXPCU_BCN_RSSI_CTL_WEIGHT_LSB) & RXPCU_BCN_RSSI_CTL_WEIGHT_MASK)
#define RXPCU_BCN_RSSI_CTL_WEIGHT_RESET                              0
#define RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_MSB                      23
#define RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_LSB                      16
#define RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_MASK                     0x00ff0000
#define RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_GET(x)                   (((x) & RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_MASK) >> RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_LSB)
#define RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_SET(x)                   (((0x0 | (x)) << RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_LSB) & RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_MASK)
#define RXPCU_BCN_RSSI_CTL_RSSI_HIGH_THRESH_RESET                    0
#define RXPCU_BCN_RSSI_CTL_MISS_THRESH_MSB                           15
#define RXPCU_BCN_RSSI_CTL_MISS_THRESH_LSB                           8
#define RXPCU_BCN_RSSI_CTL_MISS_THRESH_MASK                          0x0000ff00
#define RXPCU_BCN_RSSI_CTL_MISS_THRESH_GET(x)                        (((x) & RXPCU_BCN_RSSI_CTL_MISS_THRESH_MASK) >> RXPCU_BCN_RSSI_CTL_MISS_THRESH_LSB)
#define RXPCU_BCN_RSSI_CTL_MISS_THRESH_SET(x)                        (((0x0 | (x)) << RXPCU_BCN_RSSI_CTL_MISS_THRESH_LSB) & RXPCU_BCN_RSSI_CTL_MISS_THRESH_MASK)
#define RXPCU_BCN_RSSI_CTL_MISS_THRESH_RESET                         0
#define RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_MSB                       7
#define RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_LSB                       0
#define RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_MASK                      0x000000ff
#define RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_GET(x)                    (((x) & RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_MASK) >> RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_LSB)
#define RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_SET(x)                    (((0x0 | (x)) << RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_LSB) & RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_MASK)
#define RXPCU_BCN_RSSI_CTL_RSSI_LOW_THRESH_RESET                     0
#define RXPCU_BCN_RSSI_CTL_ADDRESS                                   0x000034
#define RXPCU_BCN_RSSI_CTL_HW_MASK                                   0x3fffffff
#define RXPCU_BCN_RSSI_CTL_SW_MASK                                   0x3fffffff
#define RXPCU_BCN_RSSI_CTL_RSTMASK                                   0x00000000
#define RXPCU_BCN_RSSI_CTL_RESET                                     0x00000000

// 0x0038 (RXPCU_BCN_RSSI_CTL2)
#define RXPCU_BCN_RSSI_CTL2_RESET2_MSB                               29
#define RXPCU_BCN_RSSI_CTL2_RESET2_LSB                               29
#define RXPCU_BCN_RSSI_CTL2_RESET2_MASK                              0x20000000
#define RXPCU_BCN_RSSI_CTL2_RESET2_GET(x)                            (((x) & RXPCU_BCN_RSSI_CTL2_RESET2_MASK) >> RXPCU_BCN_RSSI_CTL2_RESET2_LSB)
#define RXPCU_BCN_RSSI_CTL2_RESET2_SET(x)                            (((0x0 | (x)) << RXPCU_BCN_RSSI_CTL2_RESET2_LSB) & RXPCU_BCN_RSSI_CTL2_RESET2_MASK)
#define RXPCU_BCN_RSSI_CTL2_RESET2_RESET                             0
#define RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_MSB                    23
#define RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_LSB                    16
#define RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_MASK                   0x00ff0000
#define RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_GET(x)                 (((x) & RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_MASK) >> RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_LSB)
#define RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_SET(x)                 (((0x0 | (x)) << RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_LSB) & RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_MASK)
#define RXPCU_BCN_RSSI_CTL2_RSSI2_HIGH_THRESH_RESET                  0
#define RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_MSB                     7
#define RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_LSB                     0
#define RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_MASK                    0x000000ff
#define RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_GET(x)                  (((x) & RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_MASK) >> RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_LSB)
#define RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_SET(x)                  (((0x0 | (x)) << RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_LSB) & RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_MASK)
#define RXPCU_BCN_RSSI_CTL2_RSSI2_LOW_THRESH_RESET                   0
#define RXPCU_BCN_RSSI_CTL2_ADDRESS                                  0x000038
#define RXPCU_BCN_RSSI_CTL2_HW_MASK                                  0x20ff00ff
#define RXPCU_BCN_RSSI_CTL2_SW_MASK                                  0x20ff00ff
#define RXPCU_BCN_RSSI_CTL2_RSTMASK                                  0x00000000
#define RXPCU_BCN_RSSI_CTL2_RESET                                    0x00000000

// 0x003c (RXPCU_BCN_RSSI_AVE)
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE2_MSB                            27
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE2_LSB                            16
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE2_MASK                           0x0fff0000
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE2_GET(x)                         (((x) & RXPCU_BCN_RSSI_AVE_AVE_VALUE2_MASK) >> RXPCU_BCN_RSSI_AVE_AVE_VALUE2_LSB)
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE2_SET(x)                         (((0x0 | (x)) << RXPCU_BCN_RSSI_AVE_AVE_VALUE2_LSB) & RXPCU_BCN_RSSI_AVE_AVE_VALUE2_MASK)
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE2_RESET                          2048
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE_MSB                             11
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE_LSB                             0
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE_MASK                            0x00000fff
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE_GET(x)                          (((x) & RXPCU_BCN_RSSI_AVE_AVE_VALUE_MASK) >> RXPCU_BCN_RSSI_AVE_AVE_VALUE_LSB)
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE_SET(x)                          (((0x0 | (x)) << RXPCU_BCN_RSSI_AVE_AVE_VALUE_LSB) & RXPCU_BCN_RSSI_AVE_AVE_VALUE_MASK)
#define RXPCU_BCN_RSSI_AVE_AVE_VALUE_RESET                           2048
#define RXPCU_BCN_RSSI_AVE_ADDRESS                                   0x00003c
#define RXPCU_BCN_RSSI_AVE_HW_MASK                                   0x0fff0fff
#define RXPCU_BCN_RSSI_AVE_SW_MASK                                   0x0fff0fff
#define RXPCU_BCN_RSSI_AVE_RSTMASK                                   0x0fff0fff
#define RXPCU_BCN_RSSI_AVE_RESET                                     0x08000800

// 0x0040 (RXPCU_FILTER_RSSI_AVE)
#define RXPCU_FILTER_RSSI_AVE_RESET_MSB                              12
#define RXPCU_FILTER_RSSI_AVE_RESET_LSB                              12
#define RXPCU_FILTER_RSSI_AVE_RESET_MASK                             0x00001000
#define RXPCU_FILTER_RSSI_AVE_RESET_GET(x)                           (((x) & RXPCU_FILTER_RSSI_AVE_RESET_MASK) >> RXPCU_FILTER_RSSI_AVE_RESET_LSB)
#define RXPCU_FILTER_RSSI_AVE_RESET_SET(x)                           (((0x0 | (x)) << RXPCU_FILTER_RSSI_AVE_RESET_LSB) & RXPCU_FILTER_RSSI_AVE_RESET_MASK)
#define RXPCU_FILTER_RSSI_AVE_RESET_RESET                            0
#define RXPCU_FILTER_RSSI_AVE_ENABLE_MSB                             11
#define RXPCU_FILTER_RSSI_AVE_ENABLE_LSB                             11
#define RXPCU_FILTER_RSSI_AVE_ENABLE_MASK                            0x00000800
#define RXPCU_FILTER_RSSI_AVE_ENABLE_GET(x)                          (((x) & RXPCU_FILTER_RSSI_AVE_ENABLE_MASK) >> RXPCU_FILTER_RSSI_AVE_ENABLE_LSB)
#define RXPCU_FILTER_RSSI_AVE_ENABLE_SET(x)                          (((0x0 | (x)) << RXPCU_FILTER_RSSI_AVE_ENABLE_LSB) & RXPCU_FILTER_RSSI_AVE_ENABLE_MASK)
#define RXPCU_FILTER_RSSI_AVE_ENABLE_RESET                           0
#define RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_MSB                10
#define RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_LSB                8
#define RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_MASK               0x00000700
#define RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_GET(x)             (((x) & RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_MASK) >> RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_LSB)
#define RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_SET(x)             (((0x0 | (x)) << RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_LSB) & RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_MASK)
#define RXPCU_FILTER_RSSI_AVE_NUM_FRAMES_EXPONENT_RESET              5
#define RXPCU_FILTER_RSSI_AVE_AVE_VALUE_MSB                          7
#define RXPCU_FILTER_RSSI_AVE_AVE_VALUE_LSB                          0
#define RXPCU_FILTER_RSSI_AVE_AVE_VALUE_MASK                         0x000000ff
#define RXPCU_FILTER_RSSI_AVE_AVE_VALUE_GET(x)                       (((x) & RXPCU_FILTER_RSSI_AVE_AVE_VALUE_MASK) >> RXPCU_FILTER_RSSI_AVE_AVE_VALUE_LSB)
#define RXPCU_FILTER_RSSI_AVE_AVE_VALUE_SET(x)                       (((0x0 | (x)) << RXPCU_FILTER_RSSI_AVE_AVE_VALUE_LSB) & RXPCU_FILTER_RSSI_AVE_AVE_VALUE_MASK)
#define RXPCU_FILTER_RSSI_AVE_AVE_VALUE_RESET                        128
#define RXPCU_FILTER_RSSI_AVE_ADDRESS                                0x000040
#define RXPCU_FILTER_RSSI_AVE_HW_MASK                                0x00001fff
#define RXPCU_FILTER_RSSI_AVE_SW_MASK                                0x00001fff
#define RXPCU_FILTER_RSSI_AVE_RSTMASK                                0x000007ff
#define RXPCU_FILTER_RSSI_AVE_RESET                                  0x00000580

// 0x0044 (RXPCU_BA_BAR_CONTROL)
#define RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_MSB                        19
#define RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_LSB                        16
#define RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_MASK                       0x000f0000
#define RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_GET(x)                     (((x) & RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_MASK) >> RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_LSB)
#define RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_SET(x)                     (((0x0 | (x)) << RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_LSB) & RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_MASK)
#define RXPCU_BA_BAR_CONTROL_SEARCH_DEPTH_RESET                      3
#define RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_MSB           12
#define RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_LSB           12
#define RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_MASK          0x00001000
#define RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_GET(x)        (((x) & RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_MASK) >> RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_LSB)
#define RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_SET(x)        (((0x0 | (x)) << RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_LSB) & RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_MASK)
#define RXPCU_BA_BAR_CONTROL_UPDATE_BA_BITMAP_QOS_NULL_RESET         0
#define RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_MSB                      10
#define RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_LSB                      10
#define RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_MASK                     0x00000400
#define RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_GET(x)                   (((x) & RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_MASK) >> RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_LSB)
#define RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_SET(x)                   (((0x0 | (x)) << RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_LSB) & RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_MASK)
#define RXPCU_BA_BAR_CONTROL_FORCE_NO_MATCH_RESET                    0
#define RXPCU_BA_BAR_CONTROL_ADDRESS                                 0x000044
#define RXPCU_BA_BAR_CONTROL_HW_MASK                                 0x000f1400
#define RXPCU_BA_BAR_CONTROL_SW_MASK                                 0x000f1400
#define RXPCU_BA_BAR_CONTROL_RSTMASK                                 0x000f0000
#define RXPCU_BA_BAR_CONTROL_RESET                                   0x00030000

// 0x0048 (RXPCU_RTT_CTRL)
#define RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_MSB                       16
#define RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_LSB                       16
#define RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_MASK                      0x00010000
#define RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_GET(x)                    (((x) & RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_MASK) >> RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_LSB)
#define RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_SET(x)                    (((0x0 | (x)) << RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_LSB) & RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_MASK)
#define RXPCU_RTT_CTRL_ALLOW_HT_VHT_ACK_BA_RESET                     0
#define RXPCU_RTT_CTRL_ACTION_MSB                                    15
#define RXPCU_RTT_CTRL_ACTION_LSB                                    8
#define RXPCU_RTT_CTRL_ACTION_MASK                                   0x0000ff00
#define RXPCU_RTT_CTRL_ACTION_GET(x)                                 (((x) & RXPCU_RTT_CTRL_ACTION_MASK) >> RXPCU_RTT_CTRL_ACTION_LSB)
#define RXPCU_RTT_CTRL_ACTION_SET(x)                                 (((0x0 | (x)) << RXPCU_RTT_CTRL_ACTION_LSB) & RXPCU_RTT_CTRL_ACTION_MASK)
#define RXPCU_RTT_CTRL_ACTION_RESET                                  1
#define RXPCU_RTT_CTRL_CATEGORY_MSB                                  7
#define RXPCU_RTT_CTRL_CATEGORY_LSB                                  0
#define RXPCU_RTT_CTRL_CATEGORY_MASK                                 0x000000ff
#define RXPCU_RTT_CTRL_CATEGORY_GET(x)                               (((x) & RXPCU_RTT_CTRL_CATEGORY_MASK) >> RXPCU_RTT_CTRL_CATEGORY_LSB)
#define RXPCU_RTT_CTRL_CATEGORY_SET(x)                               (((0x0 | (x)) << RXPCU_RTT_CTRL_CATEGORY_LSB) & RXPCU_RTT_CTRL_CATEGORY_MASK)
#define RXPCU_RTT_CTRL_CATEGORY_RESET                                11
#define RXPCU_RTT_CTRL_ADDRESS                                       0x000048
#define RXPCU_RTT_CTRL_HW_MASK                                       0x0001ffff
#define RXPCU_RTT_CTRL_SW_MASK                                       0x0001ffff
#define RXPCU_RTT_CTRL_RSTMASK                                       0x0000ffff
#define RXPCU_RTT_CTRL_RESET                                         0x0000010b

// 0x004c (RXPCU_XRSTMP)
#define RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_MSB                        23
#define RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_LSB                        16
#define RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_MASK                       0x00ff0000
#define RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_GET(x)                     (((x) & RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_MASK) >> RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_LSB)
#define RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_SET(x)                     (((0x0 | (x)) << RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_LSB) & RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_MASK)
#define RXPCU_XRSTMP_RX_ABORT_RSSI_THRESH_RESET                      37
#define RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_MSB                        15
#define RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_LSB                        8
#define RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_MASK                       0x0000ff00
#define RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_GET(x)                     (((x) & RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_MASK) >> RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_LSB)
#define RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_SET(x)                     (((0x0 | (x)) << RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_LSB) & RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_MASK)
#define RXPCU_XRSTMP_TX_STOMP_RSSI_THRESH_RESET                      37
#define RXPCU_XRSTMP_RX_ABORT_DATA_MSB                               5
#define RXPCU_XRSTMP_RX_ABORT_DATA_LSB                               5
#define RXPCU_XRSTMP_RX_ABORT_DATA_MASK                              0x00000020
#define RXPCU_XRSTMP_RX_ABORT_DATA_GET(x)                            (((x) & RXPCU_XRSTMP_RX_ABORT_DATA_MASK) >> RXPCU_XRSTMP_RX_ABORT_DATA_LSB)
#define RXPCU_XRSTMP_RX_ABORT_DATA_SET(x)                            (((0x0 | (x)) << RXPCU_XRSTMP_RX_ABORT_DATA_LSB) & RXPCU_XRSTMP_RX_ABORT_DATA_MASK)
#define RXPCU_XRSTMP_RX_ABORT_DATA_RESET                             0
#define RXPCU_XRSTMP_TX_STOMP_DATA_MSB                               4
#define RXPCU_XRSTMP_TX_STOMP_DATA_LSB                               4
#define RXPCU_XRSTMP_TX_STOMP_DATA_MASK                              0x00000010
#define RXPCU_XRSTMP_TX_STOMP_DATA_GET(x)                            (((x) & RXPCU_XRSTMP_TX_STOMP_DATA_MASK) >> RXPCU_XRSTMP_TX_STOMP_DATA_LSB)
#define RXPCU_XRSTMP_TX_STOMP_DATA_SET(x)                            (((0x0 | (x)) << RXPCU_XRSTMP_TX_STOMP_DATA_LSB) & RXPCU_XRSTMP_TX_STOMP_DATA_MASK)
#define RXPCU_XRSTMP_TX_STOMP_DATA_RESET                             0
#define RXPCU_XRSTMP_TX_STOMP_BSSID_MSB                              3
#define RXPCU_XRSTMP_TX_STOMP_BSSID_LSB                              3
#define RXPCU_XRSTMP_TX_STOMP_BSSID_MASK                             0x00000008
#define RXPCU_XRSTMP_TX_STOMP_BSSID_GET(x)                           (((x) & RXPCU_XRSTMP_TX_STOMP_BSSID_MASK) >> RXPCU_XRSTMP_TX_STOMP_BSSID_LSB)
#define RXPCU_XRSTMP_TX_STOMP_BSSID_SET(x)                           (((0x0 | (x)) << RXPCU_XRSTMP_TX_STOMP_BSSID_LSB) & RXPCU_XRSTMP_TX_STOMP_BSSID_MASK)
#define RXPCU_XRSTMP_TX_STOMP_BSSID_RESET                            0
#define RXPCU_XRSTMP_TX_STOMP_RSSI_MSB                               2
#define RXPCU_XRSTMP_TX_STOMP_RSSI_LSB                               2
#define RXPCU_XRSTMP_TX_STOMP_RSSI_MASK                              0x00000004
#define RXPCU_XRSTMP_TX_STOMP_RSSI_GET(x)                            (((x) & RXPCU_XRSTMP_TX_STOMP_RSSI_MASK) >> RXPCU_XRSTMP_TX_STOMP_RSSI_LSB)
#define RXPCU_XRSTMP_TX_STOMP_RSSI_SET(x)                            (((0x0 | (x)) << RXPCU_XRSTMP_TX_STOMP_RSSI_LSB) & RXPCU_XRSTMP_TX_STOMP_RSSI_MASK)
#define RXPCU_XRSTMP_TX_STOMP_RSSI_RESET                             0
#define RXPCU_XRSTMP_RX_ABORT_BSSID_MSB                              1
#define RXPCU_XRSTMP_RX_ABORT_BSSID_LSB                              1
#define RXPCU_XRSTMP_RX_ABORT_BSSID_MASK                             0x00000002
#define RXPCU_XRSTMP_RX_ABORT_BSSID_GET(x)                           (((x) & RXPCU_XRSTMP_RX_ABORT_BSSID_MASK) >> RXPCU_XRSTMP_RX_ABORT_BSSID_LSB)
#define RXPCU_XRSTMP_RX_ABORT_BSSID_SET(x)                           (((0x0 | (x)) << RXPCU_XRSTMP_RX_ABORT_BSSID_LSB) & RXPCU_XRSTMP_RX_ABORT_BSSID_MASK)
#define RXPCU_XRSTMP_RX_ABORT_BSSID_RESET                            0
#define RXPCU_XRSTMP_RX_ABORT_RSSI_MSB                               0
#define RXPCU_XRSTMP_RX_ABORT_RSSI_LSB                               0
#define RXPCU_XRSTMP_RX_ABORT_RSSI_MASK                              0x00000001
#define RXPCU_XRSTMP_RX_ABORT_RSSI_GET(x)                            (((x) & RXPCU_XRSTMP_RX_ABORT_RSSI_MASK) >> RXPCU_XRSTMP_RX_ABORT_RSSI_LSB)
#define RXPCU_XRSTMP_RX_ABORT_RSSI_SET(x)                            (((0x0 | (x)) << RXPCU_XRSTMP_RX_ABORT_RSSI_LSB) & RXPCU_XRSTMP_RX_ABORT_RSSI_MASK)
#define RXPCU_XRSTMP_RX_ABORT_RSSI_RESET                             0
#define RXPCU_XRSTMP_ADDRESS                                         0x00004c
#define RXPCU_XRSTMP_HW_MASK                                         0x00ffff3f
#define RXPCU_XRSTMP_SW_MASK                                         0x00ffff3f
#define RXPCU_XRSTMP_RSTMASK                                         0x00ffff00
#define RXPCU_XRSTMP_RESET                                           0x00252500

// 0x0050 (RXPCU_TXBF_CNTL)
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_MSB                      4
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_LSB                      4
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_MASK                     0x00000010
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_GET(x)                   (((x) & RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_MASK) >> RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_LSB)
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_SET(x)                   (((0x0 | (x)) << RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_LSB) & RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_MASK)
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_VALID_RESET                    0
#define RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_MSB                     3
#define RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_LSB                     3
#define RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_MASK                    0x00000008
#define RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_GET(x)                  (((x) & RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_MASK) >> RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_LSB)
#define RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_SET(x)                  (((0x0 | (x)) << RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_LSB) & RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_MASK)
#define RXPCU_TXBF_CNTL_IGNORE_CV_DATA_VALID_RESET                   0
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_MSB                        2
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_LSB                        2
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_MASK                       0x00000004
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_GET(x)                     (((x) & RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_MASK) >> RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_LSB)
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_SET(x)                     (((0x0 | (x)) << RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_LSB) & RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_MASK)
#define RXPCU_TXBF_CNTL_CLEAR_CV_DATA_RDY_RESET                      0
#define RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_MSB                       1
#define RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_LSB                       1
#define RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_MASK                      0x00000002
#define RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_GET(x)                    (((x) & RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_MASK) >> RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_LSB)
#define RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_SET(x)                    (((0x0 | (x)) << RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_LSB) & RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_MASK)
#define RXPCU_TXBF_CNTL_CLEAR_NDP_TIMER_EN_RESET                     0
#define RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_MSB                           0
#define RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_LSB                           0
#define RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_MASK                          0x00000001
#define RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_GET(x)                        (((x) & RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_MASK) >> RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_LSB)
#define RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_SET(x)                        (((0x0 | (x)) << RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_LSB) & RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_MASK)
#define RXPCU_TXBF_CNTL_CLEAR_CV_FRAME_RESET                         0
#define RXPCU_TXBF_CNTL_ADDRESS                                      0x000050
#define RXPCU_TXBF_CNTL_HW_MASK                                      0x0000001f
#define RXPCU_TXBF_CNTL_SW_MASK                                      0x0000001f
#define RXPCU_TXBF_CNTL_RSTMASK                                      0x00000000
#define RXPCU_TXBF_CNTL_RESET                                        0x00000000

// 0x0054 (RXPCU_LOCATION_MODE_TIMER)
#define RXPCU_LOCATION_MODE_TIMER_VALUE_MSB                          31
#define RXPCU_LOCATION_MODE_TIMER_VALUE_LSB                          0
#define RXPCU_LOCATION_MODE_TIMER_VALUE_MASK                         0xffffffff
#define RXPCU_LOCATION_MODE_TIMER_VALUE_GET(x)                       (((x) & RXPCU_LOCATION_MODE_TIMER_VALUE_MASK) >> RXPCU_LOCATION_MODE_TIMER_VALUE_LSB)
#define RXPCU_LOCATION_MODE_TIMER_VALUE_SET(x)                       (((0x0 | (x)) << RXPCU_LOCATION_MODE_TIMER_VALUE_LSB) & RXPCU_LOCATION_MODE_TIMER_VALUE_MASK)
#define RXPCU_LOCATION_MODE_TIMER_VALUE_RESET                        0
#define RXPCU_LOCATION_MODE_TIMER_ADDRESS                            0x000054
#define RXPCU_LOCATION_MODE_TIMER_HW_MASK                            0xffffffff
#define RXPCU_LOCATION_MODE_TIMER_SW_MASK                            0xffffffff
#define RXPCU_LOCATION_MODE_TIMER_RSTMASK                            0x00000000
#define RXPCU_LOCATION_MODE_TIMER_RESET                              0x00000000

// 0x0058 (RXPCU_SEC_CHANNEL_RX_PIFS_CNT)
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_MSB                      31
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_LSB                      0
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_MASK                     0xffffffff
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_GET(x)                   (((x) & RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_MASK) >> RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_LSB)
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_SET(x)                   (((0x0 | (x)) << RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_LSB) & RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_MASK)
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_VALUE_RESET                    2640
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_ADDRESS                        0x000058
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_HW_MASK                        0xffffffff
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_SW_MASK                        0xffffffff
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_RSTMASK                        0xffffffff
#define RXPCU_SEC_CHANNEL_RX_PIFS_CNT_RESET                          0x00000a50

// 0x005c (RXPCU_SEC_CHANNEL_TX_PIFS_CNT)
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_MSB                      31
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_LSB                      0
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_MASK                     0xffffffff
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_GET(x)                   (((x) & RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_MASK) >> RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_LSB)
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_SET(x)                   (((0x0 | (x)) << RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_LSB) & RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_MASK)
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_VALUE_RESET                    2024
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_ADDRESS                        0x00005c
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_HW_MASK                        0xffffffff
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_SW_MASK                        0xffffffff
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_RSTMASK                        0xffffffff
#define RXPCU_SEC_CHANNEL_TX_PIFS_CNT_RESET                          0x000007e8

// 0x0060 (RXPCU_MAX_CFP_DUR)
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_MSB                  27
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_LSB                  24
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_MASK                 0x0f000000
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_GET(x)               (((x) & RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_MASK) >> RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_LSB)
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_SET(x)               (((0x0 | (x)) << RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_LSB) & RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_MASK)
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_DENOMINATOR_RESET                0
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_MSB                    19
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_LSB                    16
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_MASK                   0x000f0000
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_GET(x)                 (((x) & RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_MASK) >> RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_LSB)
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_SET(x)                 (((0x0 | (x)) << RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_LSB) & RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_MASK)
#define RXPCU_MAX_CFP_DUR_USEC_FRAC_NUMERATOR_RESET                  0
#define RXPCU_MAX_CFP_DUR_VALUE_MSB                                  15
#define RXPCU_MAX_CFP_DUR_VALUE_LSB                                  0
#define RXPCU_MAX_CFP_DUR_VALUE_MASK                                 0x0000ffff
#define RXPCU_MAX_CFP_DUR_VALUE_GET(x)                               (((x) & RXPCU_MAX_CFP_DUR_VALUE_MASK) >> RXPCU_MAX_CFP_DUR_VALUE_LSB)
#define RXPCU_MAX_CFP_DUR_VALUE_SET(x)                               (((0x0 | (x)) << RXPCU_MAX_CFP_DUR_VALUE_LSB) & RXPCU_MAX_CFP_DUR_VALUE_MASK)
#define RXPCU_MAX_CFP_DUR_VALUE_RESET                                0
#define RXPCU_MAX_CFP_DUR_ADDRESS                                    0x000060
#define RXPCU_MAX_CFP_DUR_HW_MASK                                    0x0f0fffff
#define RXPCU_MAX_CFP_DUR_SW_MASK                                    0x0f0fffff
#define RXPCU_MAX_CFP_DUR_RSTMASK                                    0x00000000
#define RXPCU_MAX_CFP_DUR_RESET                                      0x00000000

// 0x0064 (RXPCU_NAV)
#define RXPCU_NAV_VALUE_MSB                                          25
#define RXPCU_NAV_VALUE_LSB                                          0
#define RXPCU_NAV_VALUE_MASK                                         0x03ffffff
#define RXPCU_NAV_VALUE_GET(x)                                       (((x) & RXPCU_NAV_VALUE_MASK) >> RXPCU_NAV_VALUE_LSB)
#define RXPCU_NAV_VALUE_SET(x)                                       (((0x0 | (x)) << RXPCU_NAV_VALUE_LSB) & RXPCU_NAV_VALUE_MASK)
#define RXPCU_NAV_VALUE_RESET                                        0
#define RXPCU_NAV_ADDRESS                                            0x000064
#define RXPCU_NAV_HW_MASK                                            0x03ffffff
#define RXPCU_NAV_SW_MASK                                            0x03ffffff
#define RXPCU_NAV_RSTMASK                                            0x00000000
#define RXPCU_NAV_RESET                                              0x00000000

// 0x0070 (RXPCU_DIRECT_CONNECT)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_MSB        19
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_LSB        19
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_MASK       0x00080000
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_GET(x)     (((x) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_MASK) >> RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_LSB)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_SET(x)     (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_LSB) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_MASK)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_PROBE_RESP_RESET      1
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_MSB   18
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_LSB   18
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_MASK  0x00040000
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_GET(x) (((x) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_MASK) >> RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_LSB)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_SET(x) (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_LSB) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_MASK)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_UCAST_MOON_PROBE_RESP_RESET 1
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_MSB        17
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_LSB        17
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_MASK       0x00020000
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_GET(x)     (((x) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_MASK) >> RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_LSB)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_SET(x)     (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_LSB) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_MASK)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_MCAST_PROBE_RESP_RESET      1
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_MSB        16
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_LSB        16
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_MASK       0x00010000
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_GET(x)     (((x) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_MASK) >> RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_LSB)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_SET(x)     (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_LSB) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_MASK)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BCAST_PROBE_RESP_RESET      1
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_MSB                  15
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_LSB                  15
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_MASK                 0x00008000
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_GET(x)               (((x) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_MASK) >> RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_LSB)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_SET(x)               (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_LSB) & RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_MASK)
#define RXPCU_DIRECT_CONNECT_SYNC_TSF_ON_BEACON_RESET                1
#define RXPCU_DIRECT_CONNECT_RX_LATENCY_MSB                          14
#define RXPCU_DIRECT_CONNECT_RX_LATENCY_LSB                          9
#define RXPCU_DIRECT_CONNECT_RX_LATENCY_MASK                         0x00007e00
#define RXPCU_DIRECT_CONNECT_RX_LATENCY_GET(x)                       (((x) & RXPCU_DIRECT_CONNECT_RX_LATENCY_MASK) >> RXPCU_DIRECT_CONNECT_RX_LATENCY_LSB)
#define RXPCU_DIRECT_CONNECT_RX_LATENCY_SET(x)                       (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_RX_LATENCY_LSB) & RXPCU_DIRECT_CONNECT_RX_LATENCY_MASK)
#define RXPCU_DIRECT_CONNECT_RX_LATENCY_RESET                        0
#define RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_MSB                      8
#define RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_LSB                      8
#define RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_MASK                     0x00000100
#define RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_GET(x)                   (((x) & RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_MASK) >> RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_LSB)
#define RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_SET(x)                   (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_LSB) & RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_MASK)
#define RXPCU_DIRECT_CONNECT_SVD_TSF_SEL_EN_RESET                    0
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_MSB                  7
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_LSB                  7
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_MASK                 0x00000080
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_GET(x)               (((x) & RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_MASK) >> RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_LSB)
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_SET(x)               (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_LSB) & RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_MASK)
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_OVERRIDE_RESET                0
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_MSB                   6
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_LSB                   6
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_MASK                  0x00000040
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_GET(x)                (((x) & RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_MASK) >> RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_LSB)
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_SET(x)                (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_LSB) & RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_MASK)
#define RXPCU_DIRECT_CONNECT_BMISS_CNT_TSF_SEL_RESET                 0
#define RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_MSB                 5
#define RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_LSB                 5
#define RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_MASK                0x00000020
#define RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_GET(x)              (((x) & RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_MASK) >> RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_LSB)
#define RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_SET(x)              (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_LSB) & RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_MASK)
#define RXPCU_DIRECT_CONNECT_MY_BEACON2_OVERRIDE_RESET               0
#define RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_MSB                  4
#define RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_LSB                  4
#define RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_MASK                 0x00000010
#define RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_GET(x)               (((x) & RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_MASK) >> RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_LSB)
#define RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_SET(x)               (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_LSB) & RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_MASK)
#define RXPCU_DIRECT_CONNECT_MY_BEACON_OVERRIDE_RESET                0
#define RXPCU_DIRECT_CONNECT_TSF2_UPDATE_MSB                         3
#define RXPCU_DIRECT_CONNECT_TSF2_UPDATE_LSB                         3
#define RXPCU_DIRECT_CONNECT_TSF2_UPDATE_MASK                        0x00000008
#define RXPCU_DIRECT_CONNECT_TSF2_UPDATE_GET(x)                      (((x) & RXPCU_DIRECT_CONNECT_TSF2_UPDATE_MASK) >> RXPCU_DIRECT_CONNECT_TSF2_UPDATE_LSB)
#define RXPCU_DIRECT_CONNECT_TSF2_UPDATE_SET(x)                      (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_TSF2_UPDATE_LSB) & RXPCU_DIRECT_CONNECT_TSF2_UPDATE_MASK)
#define RXPCU_DIRECT_CONNECT_TSF2_UPDATE_RESET                       0
#define RXPCU_DIRECT_CONNECT_TSF1_UPDATE_MSB                         2
#define RXPCU_DIRECT_CONNECT_TSF1_UPDATE_LSB                         2
#define RXPCU_DIRECT_CONNECT_TSF1_UPDATE_MASK                        0x00000004
#define RXPCU_DIRECT_CONNECT_TSF1_UPDATE_GET(x)                      (((x) & RXPCU_DIRECT_CONNECT_TSF1_UPDATE_MASK) >> RXPCU_DIRECT_CONNECT_TSF1_UPDATE_LSB)
#define RXPCU_DIRECT_CONNECT_TSF1_UPDATE_SET(x)                      (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_TSF1_UPDATE_LSB) & RXPCU_DIRECT_CONNECT_TSF1_UPDATE_MASK)
#define RXPCU_DIRECT_CONNECT_TSF1_UPDATE_RESET                       1
#define RXPCU_DIRECT_CONNECT_TS_TSF_SEL_MSB                          1
#define RXPCU_DIRECT_CONNECT_TS_TSF_SEL_LSB                          1
#define RXPCU_DIRECT_CONNECT_TS_TSF_SEL_MASK                         0x00000002
#define RXPCU_DIRECT_CONNECT_TS_TSF_SEL_GET(x)                       (((x) & RXPCU_DIRECT_CONNECT_TS_TSF_SEL_MASK) >> RXPCU_DIRECT_CONNECT_TS_TSF_SEL_LSB)
#define RXPCU_DIRECT_CONNECT_TS_TSF_SEL_SET(x)                       (((0x0 | (x)) << RXPCU_DIRECT_CONNECT_TS_TSF_SEL_LSB) & RXPCU_DIRECT_CONNECT_TS_TSF_SEL_MASK)
#define RXPCU_DIRECT_CONNECT_TS_TSF_SEL_RESET                        0
#define RXPCU_DIRECT_CONNECT_ADDRESS                                 0x000070
#define RXPCU_DIRECT_CONNECT_HW_MASK                                 0x000ffffe
#define RXPCU_DIRECT_CONNECT_SW_MASK                                 0x000ffffe
#define RXPCU_DIRECT_CONNECT_RSTMASK                                 0x000f8004
#define RXPCU_DIRECT_CONNECT_RESET                                   0x000f8004

// 0x0074 (RXPCU_LAST_BEACON_TSF)
#define RXPCU_LAST_BEACON_TSF_VALUE_MSB                              31
#define RXPCU_LAST_BEACON_TSF_VALUE_LSB                              0
#define RXPCU_LAST_BEACON_TSF_VALUE_MASK                             0xffffffff
#define RXPCU_LAST_BEACON_TSF_VALUE_GET(x)                           (((x) & RXPCU_LAST_BEACON_TSF_VALUE_MASK) >> RXPCU_LAST_BEACON_TSF_VALUE_LSB)
#define RXPCU_LAST_BEACON_TSF_VALUE_SET(x)                           (((0x0 | (x)) << RXPCU_LAST_BEACON_TSF_VALUE_LSB) & RXPCU_LAST_BEACON_TSF_VALUE_MASK)
#define RXPCU_LAST_BEACON_TSF_VALUE_RESET                            0
#define RXPCU_LAST_BEACON_TSF_ADDRESS                                0x000074
#define RXPCU_LAST_BEACON_TSF_HW_MASK                                0xffffffff
#define RXPCU_LAST_BEACON_TSF_SW_MASK                                0xffffffff
#define RXPCU_LAST_BEACON_TSF_RSTMASK                                0x00000000
#define RXPCU_LAST_BEACON_TSF_RESET                                  0x00000000

// 0x0078 (RXPCU_LAST_BEACON2_TSF)
#define RXPCU_LAST_BEACON2_TSF_VALUE_MSB                             31
#define RXPCU_LAST_BEACON2_TSF_VALUE_LSB                             0
#define RXPCU_LAST_BEACON2_TSF_VALUE_MASK                            0xffffffff
#define RXPCU_LAST_BEACON2_TSF_VALUE_GET(x)                          (((x) & RXPCU_LAST_BEACON2_TSF_VALUE_MASK) >> RXPCU_LAST_BEACON2_TSF_VALUE_LSB)
#define RXPCU_LAST_BEACON2_TSF_VALUE_SET(x)                          (((0x0 | (x)) << RXPCU_LAST_BEACON2_TSF_VALUE_LSB) & RXPCU_LAST_BEACON2_TSF_VALUE_MASK)
#define RXPCU_LAST_BEACON2_TSF_VALUE_RESET                           0
#define RXPCU_LAST_BEACON2_TSF_ADDRESS                               0x000078
#define RXPCU_LAST_BEACON2_TSF_HW_MASK                               0xffffffff
#define RXPCU_LAST_BEACON2_TSF_SW_MASK                               0xffffffff
#define RXPCU_LAST_BEACON2_TSF_RSTMASK                               0x00000000
#define RXPCU_LAST_BEACON2_TSF_RESET                                 0x00000000

// 0x007c (RXPCU_HW_BCN_PROC1)
#define RXPCU_HW_BCN_PROC1_ELM2_ID_MSB                               31
#define RXPCU_HW_BCN_PROC1_ELM2_ID_LSB                               24
#define RXPCU_HW_BCN_PROC1_ELM2_ID_MASK                              0xff000000
#define RXPCU_HW_BCN_PROC1_ELM2_ID_GET(x)                            (((x) & RXPCU_HW_BCN_PROC1_ELM2_ID_MASK) >> RXPCU_HW_BCN_PROC1_ELM2_ID_LSB)
#define RXPCU_HW_BCN_PROC1_ELM2_ID_SET(x)                            (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_ELM2_ID_LSB) & RXPCU_HW_BCN_PROC1_ELM2_ID_MASK)
#define RXPCU_HW_BCN_PROC1_ELM2_ID_RESET                             0
#define RXPCU_HW_BCN_PROC1_ELM1_ID_MSB                               23
#define RXPCU_HW_BCN_PROC1_ELM1_ID_LSB                               16
#define RXPCU_HW_BCN_PROC1_ELM1_ID_MASK                              0x00ff0000
#define RXPCU_HW_BCN_PROC1_ELM1_ID_GET(x)                            (((x) & RXPCU_HW_BCN_PROC1_ELM1_ID_MASK) >> RXPCU_HW_BCN_PROC1_ELM1_ID_LSB)
#define RXPCU_HW_BCN_PROC1_ELM1_ID_SET(x)                            (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_ELM1_ID_LSB) & RXPCU_HW_BCN_PROC1_ELM1_ID_MASK)
#define RXPCU_HW_BCN_PROC1_ELM1_ID_RESET                             0
#define RXPCU_HW_BCN_PROC1_ELM0_ID_MSB                               15
#define RXPCU_HW_BCN_PROC1_ELM0_ID_LSB                               8
#define RXPCU_HW_BCN_PROC1_ELM0_ID_MASK                              0x0000ff00
#define RXPCU_HW_BCN_PROC1_ELM0_ID_GET(x)                            (((x) & RXPCU_HW_BCN_PROC1_ELM0_ID_MASK) >> RXPCU_HW_BCN_PROC1_ELM0_ID_LSB)
#define RXPCU_HW_BCN_PROC1_ELM0_ID_SET(x)                            (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_ELM0_ID_LSB) & RXPCU_HW_BCN_PROC1_ELM0_ID_MASK)
#define RXPCU_HW_BCN_PROC1_ELM0_ID_RESET                             0
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_MSB                          7
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_LSB                          7
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_MASK                         0x00000080
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_GET(x)                       (((x) & RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_MASK) >> RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_LSB)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_SET(x)                       (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_LSB) & RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_MASK)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM2_RESET                        0
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_MSB                          6
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_LSB                          6
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_MASK                         0x00000040
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_GET(x)                       (((x) & RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_MASK) >> RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_LSB)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_SET(x)                       (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_LSB) & RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_MASK)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM1_RESET                        0
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_MSB                          5
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_LSB                          5
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_MASK                         0x00000020
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_GET(x)                       (((x) & RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_MASK) >> RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_LSB)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_SET(x)                       (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_LSB) & RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_MASK)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_ELM0_RESET                        0
#define RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_MSB                       4
#define RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_LSB                       4
#define RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_MASK                      0x00000010
#define RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_GET(x)                    (((x) & RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_MASK) >> RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_LSB)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_SET(x)                    (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_LSB) & RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_MASK)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_TIM_ELM_RESET                     0
#define RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_MSB                      3
#define RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_LSB                      3
#define RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_MASK                     0x00000008
#define RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_GET(x)                   (((x) & RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_MASK) >> RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_LSB)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_SET(x)                   (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_LSB) & RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_MASK)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_CAP_INFO_RESET                    0
#define RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_MSB                     2
#define RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_LSB                     2
#define RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_MASK                    0x00000004
#define RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_GET(x)                  (((x) & RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_MASK) >> RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_LSB)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_SET(x)                  (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_LSB) & RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_MASK)
#define RXPCU_HW_BCN_PROC1_EXCLUDE_BCN_INTVL_RESET                   0
#define RXPCU_HW_BCN_PROC1_RESET_CRC_MSB                             1
#define RXPCU_HW_BCN_PROC1_RESET_CRC_LSB                             1
#define RXPCU_HW_BCN_PROC1_RESET_CRC_MASK                            0x00000002
#define RXPCU_HW_BCN_PROC1_RESET_CRC_GET(x)                          (((x) & RXPCU_HW_BCN_PROC1_RESET_CRC_MASK) >> RXPCU_HW_BCN_PROC1_RESET_CRC_LSB)
#define RXPCU_HW_BCN_PROC1_RESET_CRC_SET(x)                          (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_RESET_CRC_LSB) & RXPCU_HW_BCN_PROC1_RESET_CRC_MASK)
#define RXPCU_HW_BCN_PROC1_RESET_CRC_RESET                           0
#define RXPCU_HW_BCN_PROC1_CRC_ENABLE_MSB                            0
#define RXPCU_HW_BCN_PROC1_CRC_ENABLE_LSB                            0
#define RXPCU_HW_BCN_PROC1_CRC_ENABLE_MASK                           0x00000001
#define RXPCU_HW_BCN_PROC1_CRC_ENABLE_GET(x)                         (((x) & RXPCU_HW_BCN_PROC1_CRC_ENABLE_MASK) >> RXPCU_HW_BCN_PROC1_CRC_ENABLE_LSB)
#define RXPCU_HW_BCN_PROC1_CRC_ENABLE_SET(x)                         (((0x0 | (x)) << RXPCU_HW_BCN_PROC1_CRC_ENABLE_LSB) & RXPCU_HW_BCN_PROC1_CRC_ENABLE_MASK)
#define RXPCU_HW_BCN_PROC1_CRC_ENABLE_RESET                          0
#define RXPCU_HW_BCN_PROC1_ADDRESS                                   0x00007c
#define RXPCU_HW_BCN_PROC1_HW_MASK                                   0xffffffff
#define RXPCU_HW_BCN_PROC1_SW_MASK                                   0xffffffff
#define RXPCU_HW_BCN_PROC1_RSTMASK                                   0x00000000
#define RXPCU_HW_BCN_PROC1_RESET                                     0x00000000

// 0x0080 (RXPCU_HW_BCN_PROC2)
#define RXPCU_HW_BCN_PROC2_ELM3_ID_MSB                               23
#define RXPCU_HW_BCN_PROC2_ELM3_ID_LSB                               16
#define RXPCU_HW_BCN_PROC2_ELM3_ID_MASK                              0x00ff0000
#define RXPCU_HW_BCN_PROC2_ELM3_ID_GET(x)                            (((x) & RXPCU_HW_BCN_PROC2_ELM3_ID_MASK) >> RXPCU_HW_BCN_PROC2_ELM3_ID_LSB)
#define RXPCU_HW_BCN_PROC2_ELM3_ID_SET(x)                            (((0x0 | (x)) << RXPCU_HW_BCN_PROC2_ELM3_ID_LSB) & RXPCU_HW_BCN_PROC2_ELM3_ID_MASK)
#define RXPCU_HW_BCN_PROC2_ELM3_ID_RESET                             0
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_MSB                       15
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_LSB                       8
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_MASK                      0x0000ff00
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_GET(x)                    (((x) & RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_MASK) >> RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_LSB)
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_SET(x)                    (((0x0 | (x)) << RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_LSB) & RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_MASK)
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_RESET                     2
#define RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_MSB                          2
#define RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_LSB                          2
#define RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_MASK                         0x00000004
#define RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_GET(x)                       (((x) & RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_MASK) >> RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_LSB)
#define RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_SET(x)                       (((0x0 | (x)) << RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_LSB) & RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_MASK)
#define RXPCU_HW_BCN_PROC2_EXCLUDE_ELM3_RESET                        0
#define RXPCU_HW_BCN_PROC2_RESET_INTERVAL_MSB                        1
#define RXPCU_HW_BCN_PROC2_RESET_INTERVAL_LSB                        1
#define RXPCU_HW_BCN_PROC2_RESET_INTERVAL_MASK                       0x00000002
#define RXPCU_HW_BCN_PROC2_RESET_INTERVAL_GET(x)                     (((x) & RXPCU_HW_BCN_PROC2_RESET_INTERVAL_MASK) >> RXPCU_HW_BCN_PROC2_RESET_INTERVAL_LSB)
#define RXPCU_HW_BCN_PROC2_RESET_INTERVAL_SET(x)                     (((0x0 | (x)) << RXPCU_HW_BCN_PROC2_RESET_INTERVAL_LSB) & RXPCU_HW_BCN_PROC2_RESET_INTERVAL_MASK)
#define RXPCU_HW_BCN_PROC2_RESET_INTERVAL_RESET                      0
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_MSB                0
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_LSB                0
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_MASK               0x00000001
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_GET(x)             (((x) & RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_MASK) >> RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_LSB)
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_SET(x)             (((0x0 | (x)) << RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_LSB) & RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_MASK)
#define RXPCU_HW_BCN_PROC2_FILTER_INTERVAL_ENABLE_RESET              0
#define RXPCU_HW_BCN_PROC2_ADDRESS                                   0x000080
#define RXPCU_HW_BCN_PROC2_HW_MASK                                   0x00ffff07
#define RXPCU_HW_BCN_PROC2_SW_MASK                                   0x00ffff07
#define RXPCU_HW_BCN_PROC2_RSTMASK                                   0x0000ff00
#define RXPCU_HW_BCN_PROC2_RESET                                     0x00000200

// 0x0084 (RXPCU_DYM_MIMO_PWR_SAVE)
#define RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_MSB                10
#define RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_LSB                8
#define RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_MASK               0x00000700
#define RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_GET(x)             (((x) & RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_MASK) >> RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_LSB)
#define RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_SET(x)             (((0x0 | (x)) << RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_LSB) & RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_MASK)
#define RXPCU_DYM_MIMO_PWR_SAVE_HI_PWR_CHAIN_MASK_RESET              3
#define RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_MSB               6
#define RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_LSB               4
#define RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_MASK              0x00000070
#define RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_GET(x)            (((x) & RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_MASK) >> RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_LSB)
#define RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_SET(x)            (((0x0 | (x)) << RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_LSB) & RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_MASK)
#define RXPCU_DYM_MIMO_PWR_SAVE_LOW_PWR_CHAIN_MASK_RESET             1
#define RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_MSB                2
#define RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_LSB                2
#define RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_MASK               0x00000004
#define RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_GET(x)             (((x) & RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_MASK) >> RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_LSB)
#define RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_SET(x)             (((0x0 | (x)) << RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_LSB) & RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_MASK)
#define RXPCU_DYM_MIMO_PWR_SAVE_SW_CHAIN_MASK_SEL_RESET              0
#define RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_MSB                       1
#define RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_LSB                       1
#define RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_MASK                      0x00000002
#define RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_GET(x)                    (((x) & RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_MASK) >> RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_LSB)
#define RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_SET(x)                    (((0x0 | (x)) << RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_LSB) & RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_MASK)
#define RXPCU_DYM_MIMO_PWR_SAVE_HW_CTRL_EN_RESET                     0
#define RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_MSB                     0
#define RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_LSB                     0
#define RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_MASK                    0x00000001
#define RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_GET(x)                  (((x) & RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_MASK) >> RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_LSB)
#define RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_SET(x)                  (((0x0 | (x)) << RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_LSB) & RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_MASK)
#define RXPCU_DYM_MIMO_PWR_SAVE_USE_MAC_CTRL_RESET                   0
#define RXPCU_DYM_MIMO_PWR_SAVE_ADDRESS                              0x000084
#define RXPCU_DYM_MIMO_PWR_SAVE_HW_MASK                              0x00000777
#define RXPCU_DYM_MIMO_PWR_SAVE_SW_MASK                              0x00000777
#define RXPCU_DYM_MIMO_PWR_SAVE_RSTMASK                              0x00000770
#define RXPCU_DYM_MIMO_PWR_SAVE_RESET                                0x00000310

// 0x0088 (RXPCU_POWER_SAVE_0)
#define RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_MSB                     9
#define RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_LSB                     9
#define RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_MASK                    0x00000200
#define RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_GET(x)                  (((x) & RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_SET(x)                  (((0x0 | (x)) << RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_LSB) & RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_RSSI_CHECK_ENABLE_RESET                   0
#define RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_MSB                  8
#define RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_LSB                  8
#define RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_MASK                 0x00000100
#define RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_GET(x)               (((x) & RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_SET(x)               (((0x0 | (x)) << RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_LSB) & RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_GID_NSTS_ZERO_ENABLE_RESET                0
#define RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_MSB             7
#define RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_LSB             7
#define RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_MASK            0x00000080
#define RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_GET(x)          (((x) & RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_SET(x)          (((0x0 | (x)) << RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_LSB) & RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_DELIMITER_CRC_FAIL_ENABLE_RESET           0
#define RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_MSB           6
#define RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_LSB           6
#define RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_MASK          0x00000040
#define RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_GET(x)        (((x) & RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_SET(x)        (((0x0 | (x)) << RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_LSB) & RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_MAC_ADDRESS_MISMATCH_ENABLE_RESET         0
#define RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_MSB               4
#define RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_LSB               4
#define RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_MASK              0x00000010
#define RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_GET(x)            (((x) & RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_SET(x)            (((0x0 | (x)) << RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_LSB) & RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_EARLY_DTIM_CLEAR_ENABLE_RESET             0
#define RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_MSB                3
#define RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_LSB                3
#define RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_MASK               0x00000008
#define RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_GET(x)             (((x) & RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_SET(x)             (((0x0 | (x)) << RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_LSB) & RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_EARLY_TIM_CLEAR_ENABLE_RESET              0
#define RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_MSB              2
#define RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_LSB              2
#define RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_MASK             0x00000004
#define RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_GET(x)           (((x) & RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_SET(x)           (((0x0 | (x)) << RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_LSB) & RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_EOF_PAD_DELIMITER_ENABLE_RESET            0
#define RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_MSB                      1
#define RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_LSB                      1
#define RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_MASK                     0x00000002
#define RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_GET(x)                   (((x) & RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_SET(x)                   (((0x0 | (x)) << RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_LSB) & RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_GID_MATCH_ENABLE_RESET                    0
#define RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_MSB              0
#define RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_LSB              0
#define RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_MASK             0x00000001
#define RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_GET(x)           (((x) & RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_MASK) >> RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_LSB)
#define RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_SET(x)           (((0x0 | (x)) << RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_LSB) & RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_MASK)
#define RXPCU_POWER_SAVE_0_PARTIAL_AID_MATCH_ENABLE_RESET            0
#define RXPCU_POWER_SAVE_0_ADDRESS                                   0x000088
#define RXPCU_POWER_SAVE_0_HW_MASK                                   0x000003df
#define RXPCU_POWER_SAVE_0_SW_MASK                                   0x000003df
#define RXPCU_POWER_SAVE_0_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_0_RESET                                     0x00000000

// 0x008c (RXPCU_POWER_SAVE_1)
#define RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_MSB                   28
#define RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_LSB                   20
#define RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_MASK                  0x1ff00000
#define RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_GET(x)                (((x) & RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_MASK) >> RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_LSB)
#define RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_SET(x)                (((0x0 | (x)) << RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_LSB) & RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_MASK)
#define RXPCU_POWER_SAVE_1_P_AID_2_MATCH_VALUE_RESET                 0
#define RXPCU_POWER_SAVE_1_RESERVED_1_MSB                            19
#define RXPCU_POWER_SAVE_1_RESERVED_1_LSB                            19
#define RXPCU_POWER_SAVE_1_RESERVED_1_MASK                           0x00080000
#define RXPCU_POWER_SAVE_1_RESERVED_1_GET(x)                         (((x) & RXPCU_POWER_SAVE_1_RESERVED_1_MASK) >> RXPCU_POWER_SAVE_1_RESERVED_1_LSB)
#define RXPCU_POWER_SAVE_1_RESERVED_1_SET(x)                         (((0x0 | (x)) << RXPCU_POWER_SAVE_1_RESERVED_1_LSB) & RXPCU_POWER_SAVE_1_RESERVED_1_MASK)
#define RXPCU_POWER_SAVE_1_RESERVED_1_RESET                          0
#define RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_MSB                   18
#define RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_LSB                   10
#define RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_MASK                  0x0007fc00
#define RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_GET(x)                (((x) & RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_MASK) >> RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_LSB)
#define RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_SET(x)                (((0x0 | (x)) << RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_LSB) & RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_MASK)
#define RXPCU_POWER_SAVE_1_P_AID_1_MATCH_VALUE_RESET                 0
#define RXPCU_POWER_SAVE_1_RESERVED_0_MSB                            9
#define RXPCU_POWER_SAVE_1_RESERVED_0_LSB                            9
#define RXPCU_POWER_SAVE_1_RESERVED_0_MASK                           0x00000200
#define RXPCU_POWER_SAVE_1_RESERVED_0_GET(x)                         (((x) & RXPCU_POWER_SAVE_1_RESERVED_0_MASK) >> RXPCU_POWER_SAVE_1_RESERVED_0_LSB)
#define RXPCU_POWER_SAVE_1_RESERVED_0_SET(x)                         (((0x0 | (x)) << RXPCU_POWER_SAVE_1_RESERVED_0_LSB) & RXPCU_POWER_SAVE_1_RESERVED_0_MASK)
#define RXPCU_POWER_SAVE_1_RESERVED_0_RESET                          0
#define RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_MSB                   8
#define RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_LSB                   0
#define RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_MASK                  0x000001ff
#define RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_GET(x)                (((x) & RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_MASK) >> RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_LSB)
#define RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_SET(x)                (((0x0 | (x)) << RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_LSB) & RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_MASK)
#define RXPCU_POWER_SAVE_1_P_AID_0_MATCH_VALUE_RESET                 0
#define RXPCU_POWER_SAVE_1_ADDRESS                                   0x00008c
#define RXPCU_POWER_SAVE_1_HW_MASK                                   0x1fffffff
#define RXPCU_POWER_SAVE_1_SW_MASK                                   0x1fffffff
#define RXPCU_POWER_SAVE_1_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_1_RESET                                     0x00000000

// 0x0090 (RXPCU_POWER_SAVE_2)
#define RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_MSB                   28
#define RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_LSB                   20
#define RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_MASK                  0x1ff00000
#define RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_GET(x)                (((x) & RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_MASK) >> RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_LSB)
#define RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_SET(x)                (((0x0 | (x)) << RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_LSB) & RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_MASK)
#define RXPCU_POWER_SAVE_2_P_AID_5_MATCH_VALUE_RESET                 0
#define RXPCU_POWER_SAVE_2_RESERVED_1_MSB                            19
#define RXPCU_POWER_SAVE_2_RESERVED_1_LSB                            19
#define RXPCU_POWER_SAVE_2_RESERVED_1_MASK                           0x00080000
#define RXPCU_POWER_SAVE_2_RESERVED_1_GET(x)                         (((x) & RXPCU_POWER_SAVE_2_RESERVED_1_MASK) >> RXPCU_POWER_SAVE_2_RESERVED_1_LSB)
#define RXPCU_POWER_SAVE_2_RESERVED_1_SET(x)                         (((0x0 | (x)) << RXPCU_POWER_SAVE_2_RESERVED_1_LSB) & RXPCU_POWER_SAVE_2_RESERVED_1_MASK)
#define RXPCU_POWER_SAVE_2_RESERVED_1_RESET                          0
#define RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_MSB                   18
#define RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_LSB                   10
#define RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_MASK                  0x0007fc00
#define RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_GET(x)                (((x) & RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_MASK) >> RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_LSB)
#define RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_SET(x)                (((0x0 | (x)) << RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_LSB) & RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_MASK)
#define RXPCU_POWER_SAVE_2_P_AID_4_MATCH_VALUE_RESET                 0
#define RXPCU_POWER_SAVE_2_RESERVED_0_MSB                            9
#define RXPCU_POWER_SAVE_2_RESERVED_0_LSB                            9
#define RXPCU_POWER_SAVE_2_RESERVED_0_MASK                           0x00000200
#define RXPCU_POWER_SAVE_2_RESERVED_0_GET(x)                         (((x) & RXPCU_POWER_SAVE_2_RESERVED_0_MASK) >> RXPCU_POWER_SAVE_2_RESERVED_0_LSB)
#define RXPCU_POWER_SAVE_2_RESERVED_0_SET(x)                         (((0x0 | (x)) << RXPCU_POWER_SAVE_2_RESERVED_0_LSB) & RXPCU_POWER_SAVE_2_RESERVED_0_MASK)
#define RXPCU_POWER_SAVE_2_RESERVED_0_RESET                          0
#define RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_MSB                   8
#define RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_LSB                   0
#define RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_MASK                  0x000001ff
#define RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_GET(x)                (((x) & RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_MASK) >> RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_LSB)
#define RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_SET(x)                (((0x0 | (x)) << RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_LSB) & RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_MASK)
#define RXPCU_POWER_SAVE_2_P_AID_3_MATCH_VALUE_RESET                 0
#define RXPCU_POWER_SAVE_2_ADDRESS                                   0x000090
#define RXPCU_POWER_SAVE_2_HW_MASK                                   0x1fffffff
#define RXPCU_POWER_SAVE_2_SW_MASK                                   0x1fffffff
#define RXPCU_POWER_SAVE_2_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_2_RESET                                     0x00000000

// 0x0094 (RXPCU_POWER_SAVE_3)
#define RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_MSB                  31
#define RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_LSB                  0
#define RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_MASK                 0xffffffff
#define RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_GET(x)               (((x) & RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_MASK) >> RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_LSB)
#define RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_SET(x)               (((0x0 | (x)) << RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_LSB) & RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_MASK)
#define RXPCU_POWER_SAVE_3_GID_LSB_BITMAP_VALUE_RESET                0
#define RXPCU_POWER_SAVE_3_ADDRESS                                   0x000094
#define RXPCU_POWER_SAVE_3_HW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_3_SW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_3_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_3_RESET                                     0x00000000

// 0x0098 (RXPCU_POWER_SAVE_4)
#define RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_MSB                  31
#define RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_LSB                  0
#define RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_MASK                 0xffffffff
#define RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_GET(x)               (((x) & RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_MASK) >> RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_LSB)
#define RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_SET(x)               (((0x0 | (x)) << RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_LSB) & RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_MASK)
#define RXPCU_POWER_SAVE_4_GID_MSB_BITMAP_VALUE_RESET                0
#define RXPCU_POWER_SAVE_4_ADDRESS                                   0x000098
#define RXPCU_POWER_SAVE_4_HW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_4_SW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_4_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_4_RESET                                     0x00000000

// 0x009c (RXPCU_POWER_SAVE_5)
#define RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_MSB            31
#define RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_LSB            0
#define RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_MASK           0xffffffff
#define RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_GET(x)         (((x) & RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_MASK) >> RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_LSB)
#define RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_SET(x)         (((0x0 | (x)) << RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_LSB) & RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_MASK)
#define RXPCU_POWER_SAVE_5_GID_USER_NUMBER_REGISTER_0_RESET          0
#define RXPCU_POWER_SAVE_5_ADDRESS                                   0x00009c
#define RXPCU_POWER_SAVE_5_HW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_5_SW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_5_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_5_RESET                                     0x00000000

// 0x00a0 (RXPCU_POWER_SAVE_6)
#define RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_MSB            31
#define RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_LSB            0
#define RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_MASK           0xffffffff
#define RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_GET(x)         (((x) & RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_MASK) >> RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_LSB)
#define RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_SET(x)         (((0x0 | (x)) << RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_LSB) & RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_MASK)
#define RXPCU_POWER_SAVE_6_GID_USER_NUMBER_REGISTER_1_RESET          0
#define RXPCU_POWER_SAVE_6_ADDRESS                                   0x0000a0
#define RXPCU_POWER_SAVE_6_HW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_6_SW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_6_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_6_RESET                                     0x00000000

// 0x00a4 (RXPCU_POWER_SAVE_7)
#define RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_MSB            31
#define RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_LSB            0
#define RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_MASK           0xffffffff
#define RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_GET(x)         (((x) & RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_MASK) >> RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_LSB)
#define RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_SET(x)         (((0x0 | (x)) << RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_LSB) & RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_MASK)
#define RXPCU_POWER_SAVE_7_GID_USER_NUMBER_REGISTER_2_RESET          0
#define RXPCU_POWER_SAVE_7_ADDRESS                                   0x0000a4
#define RXPCU_POWER_SAVE_7_HW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_7_SW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_7_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_7_RESET                                     0x00000000

// 0x00a8 (RXPCU_POWER_SAVE_8)
#define RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_MSB            31
#define RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_LSB            0
#define RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_MASK           0xffffffff
#define RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_GET(x)         (((x) & RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_MASK) >> RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_LSB)
#define RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_SET(x)         (((0x0 | (x)) << RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_LSB) & RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_MASK)
#define RXPCU_POWER_SAVE_8_GID_USER_NUMBER_REGISTER_3_RESET          0
#define RXPCU_POWER_SAVE_8_ADDRESS                                   0x0000a8
#define RXPCU_POWER_SAVE_8_HW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_8_SW_MASK                                   0xffffffff
#define RXPCU_POWER_SAVE_8_RSTMASK                                   0x00000000
#define RXPCU_POWER_SAVE_8_RESET                                     0x00000000

// 0x00ac (RXPCU_PAID_GID_MISMATCH)
#define RXPCU_PAID_GID_MISMATCH_EIFS_EN_MSB                          0
#define RXPCU_PAID_GID_MISMATCH_EIFS_EN_LSB                          0
#define RXPCU_PAID_GID_MISMATCH_EIFS_EN_MASK                         0x00000001
#define RXPCU_PAID_GID_MISMATCH_EIFS_EN_GET(x)                       (((x) & RXPCU_PAID_GID_MISMATCH_EIFS_EN_MASK) >> RXPCU_PAID_GID_MISMATCH_EIFS_EN_LSB)
#define RXPCU_PAID_GID_MISMATCH_EIFS_EN_SET(x)                       (((0x0 | (x)) << RXPCU_PAID_GID_MISMATCH_EIFS_EN_LSB) & RXPCU_PAID_GID_MISMATCH_EIFS_EN_MASK)
#define RXPCU_PAID_GID_MISMATCH_EIFS_EN_RESET                        0
#define RXPCU_PAID_GID_MISMATCH_ADDRESS                              0x0000ac
#define RXPCU_PAID_GID_MISMATCH_HW_MASK                              0x00000001
#define RXPCU_PAID_GID_MISMATCH_SW_MASK                              0x00000001
#define RXPCU_PAID_GID_MISMATCH_RSTMASK                              0x00000000
#define RXPCU_PAID_GID_MISMATCH_RESET                                0x00000000

// 0x00c0 (RXPCU_ASE_AST_BASE_ADDR)
#define RXPCU_ASE_AST_BASE_ADDR_VALUE_MSB                            31
#define RXPCU_ASE_AST_BASE_ADDR_VALUE_LSB                            0
#define RXPCU_ASE_AST_BASE_ADDR_VALUE_MASK                           0xffffffff
#define RXPCU_ASE_AST_BASE_ADDR_VALUE_GET(x)                         (((x) & RXPCU_ASE_AST_BASE_ADDR_VALUE_MASK) >> RXPCU_ASE_AST_BASE_ADDR_VALUE_LSB)
#define RXPCU_ASE_AST_BASE_ADDR_VALUE_SET(x)                         (((0x0 | (x)) << RXPCU_ASE_AST_BASE_ADDR_VALUE_LSB) & RXPCU_ASE_AST_BASE_ADDR_VALUE_MASK)
#define RXPCU_ASE_AST_BASE_ADDR_VALUE_RESET                          0
#define RXPCU_ASE_AST_BASE_ADDR_ADDRESS                              0x0000c0
#define RXPCU_ASE_AST_BASE_ADDR_HW_MASK                              0xffffffff
#define RXPCU_ASE_AST_BASE_ADDR_SW_MASK                              0xffffffff
#define RXPCU_ASE_AST_BASE_ADDR_RSTMASK                              0x00000000
#define RXPCU_ASE_AST_BASE_ADDR_RESET                                0x00000000

// 0x00c4 (RXPCU_ASE_AST_SIZE)
#define RXPCU_ASE_AST_SIZE_VALUE_MSB                                 12
#define RXPCU_ASE_AST_SIZE_VALUE_LSB                                 0
#define RXPCU_ASE_AST_SIZE_VALUE_MASK                                0x00001fff
#define RXPCU_ASE_AST_SIZE_VALUE_GET(x)                              (((x) & RXPCU_ASE_AST_SIZE_VALUE_MASK) >> RXPCU_ASE_AST_SIZE_VALUE_LSB)
#define RXPCU_ASE_AST_SIZE_VALUE_SET(x)                              (((0x0 | (x)) << RXPCU_ASE_AST_SIZE_VALUE_LSB) & RXPCU_ASE_AST_SIZE_VALUE_MASK)
#define RXPCU_ASE_AST_SIZE_VALUE_RESET                               0
#define RXPCU_ASE_AST_SIZE_ADDRESS                                   0x0000c4
#define RXPCU_ASE_AST_SIZE_HW_MASK                                   0x00001fff
#define RXPCU_ASE_AST_SIZE_SW_MASK                                   0x00001fff
#define RXPCU_ASE_AST_SIZE_RSTMASK                                   0x00000000
#define RXPCU_ASE_AST_SIZE_RESET                                     0x00000000

// 0x00c8 (RXPCU_ASE_SEARCH_CTRL)
#define RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_MSB                     31
#define RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_LSB                     16
#define RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_MASK                    0xffff0000
#define RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_GET(x)                  (((x) & RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_MASK) >> RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_LSB)
#define RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_SET(x)                  (((0x0 | (x)) << RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_LSB) & RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_MASK)
#define RXPCU_ASE_SEARCH_CTRL_TIMEOUT_THRESH_RESET                   0
#define RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_MSB                   8
#define RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_LSB                   8
#define RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_MASK                  0x00000100
#define RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_GET(x)                (((x) & RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_MASK) >> RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_LSB)
#define RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_SET(x)                (((0x0 | (x)) << RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_LSB) & RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_MASK)
#define RXPCU_ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_RESET                 0
#define RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_MSB                         7
#define RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_LSB                         0
#define RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_MASK                        0x000000ff
#define RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_GET(x)                      (((x) & RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_MASK) >> RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_LSB)
#define RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_SET(x)                      (((0x0 | (x)) << RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_LSB) & RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_MASK)
#define RXPCU_ASE_SEARCH_CTRL_MAX_SEARCH_RESET                       0
#define RXPCU_ASE_SEARCH_CTRL_ADDRESS                                0x0000c8
#define RXPCU_ASE_SEARCH_CTRL_HW_MASK                                0xffff01ff
#define RXPCU_ASE_SEARCH_CTRL_SW_MASK                                0xffff01ff
#define RXPCU_ASE_SEARCH_CTRL_RSTMASK                                0x00000000
#define RXPCU_ASE_SEARCH_CTRL_RESET                                  0x00000000

// 0x00cc (RXPCU_ASE_HASH_CTRL)
#define RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_MSB                     23
#define RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_LSB                     23
#define RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_MASK                    0x00800000
#define RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_GET(x)                  (((x) & RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_MASK) >> RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_LSB)
#define RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_SET(x)                  (((0x0 | (x)) << RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_LSB) & RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_MASK)
#define RXPCU_ASE_HASH_CTRL_CACHE_INVALIDATE_RESET                   0
#define RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_MSB                        22
#define RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_LSB                        22
#define RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_MASK                       0x00400000
#define RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_GET(x)                     (((x) & RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_MASK) >> RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_LSB)
#define RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_SET(x)                     (((0x0 | (x)) << RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_LSB) & RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_MASK)
#define RXPCU_ASE_HASH_CTRL_CACHE_DISABLE_RESET                      0
#define RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_MSB                 21
#define RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_LSB                 21
#define RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_MASK                0x00200000
#define RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_GET(x)              (((x) & RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_MASK) >> RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_LSB)
#define RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_SET(x)              (((0x0 | (x)) << RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_LSB) & RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_MASK)
#define RXPCU_ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_RESET               0
#define RXPCU_ASE_HASH_CTRL_HASH_SEL_MSB                             20
#define RXPCU_ASE_HASH_CTRL_HASH_SEL_LSB                             20
#define RXPCU_ASE_HASH_CTRL_HASH_SEL_MASK                            0x00100000
#define RXPCU_ASE_HASH_CTRL_HASH_SEL_GET(x)                          (((x) & RXPCU_ASE_HASH_CTRL_HASH_SEL_MASK) >> RXPCU_ASE_HASH_CTRL_HASH_SEL_LSB)
#define RXPCU_ASE_HASH_CTRL_HASH_SEL_SET(x)                          (((0x0 | (x)) << RXPCU_ASE_HASH_CTRL_HASH_SEL_LSB) & RXPCU_ASE_HASH_CTRL_HASH_SEL_MASK)
#define RXPCU_ASE_HASH_CTRL_HASH_SEL_RESET                           1
#define RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_MSB                     19
#define RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_LSB                     16
#define RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_MASK                    0x000f0000
#define RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_GET(x)                  (((x) & RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_MASK) >> RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_LSB)
#define RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_SET(x)                  (((0x0 | (x)) << RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_LSB) & RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_MASK)
#define RXPCU_ASE_HASH_CTRL_HASH_MCAST_SHIFT_RESET                   2
#define RXPCU_ASE_HASH_CTRL_HASH_XOR_MSB                             15
#define RXPCU_ASE_HASH_CTRL_HASH_XOR_LSB                             0
#define RXPCU_ASE_HASH_CTRL_HASH_XOR_MASK                            0x0000ffff
#define RXPCU_ASE_HASH_CTRL_HASH_XOR_GET(x)                          (((x) & RXPCU_ASE_HASH_CTRL_HASH_XOR_MASK) >> RXPCU_ASE_HASH_CTRL_HASH_XOR_LSB)
#define RXPCU_ASE_HASH_CTRL_HASH_XOR_SET(x)                          (((0x0 | (x)) << RXPCU_ASE_HASH_CTRL_HASH_XOR_LSB) & RXPCU_ASE_HASH_CTRL_HASH_XOR_MASK)
#define RXPCU_ASE_HASH_CTRL_HASH_XOR_RESET                           43981
#define RXPCU_ASE_HASH_CTRL_ADDRESS                                  0x0000cc
#define RXPCU_ASE_HASH_CTRL_HW_MASK                                  0x00ffffff
#define RXPCU_ASE_HASH_CTRL_SW_MASK                                  0x00ffffff
#define RXPCU_ASE_HASH_CTRL_RSTMASK                                  0x001fffff
#define RXPCU_ASE_HASH_CTRL_RESET                                    0x0012abcd

// 0x00d0 (RXPCU_MISC_MODE)
#define RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_MSB             31
#define RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_LSB             31
#define RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_MASK            0x80000000
#define RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_GET(x)          (((x) & RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_MASK) >> RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_LSB)
#define RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_SET(x)          (((0x0 | (x)) << RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_LSB) & RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_MASK)
#define RXPCU_MISC_MODE_LOCATION_MODE_CONTROL_ENABLE_RESET           0
#define RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_MSB                28
#define RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_LSB                28
#define RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_MASK               0x10000000
#define RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_GET(x)             (((x) & RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_MASK) >> RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_LSB)
#define RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_SET(x)             (((0x0 | (x)) << RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_LSB) & RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_MASK)
#define RXPCU_MISC_MODE_ALWAYS_PERFORM_KEY_SEARCH_RESET              0
#define RXPCU_MISC_MODE_SEL_EVM_MSB                                  27
#define RXPCU_MISC_MODE_SEL_EVM_LSB                                  27
#define RXPCU_MISC_MODE_SEL_EVM_MASK                                 0x08000000
#define RXPCU_MISC_MODE_SEL_EVM_GET(x)                               (((x) & RXPCU_MISC_MODE_SEL_EVM_MASK) >> RXPCU_MISC_MODE_SEL_EVM_LSB)
#define RXPCU_MISC_MODE_SEL_EVM_SET(x)                               (((0x0 | (x)) << RXPCU_MISC_MODE_SEL_EVM_LSB) & RXPCU_MISC_MODE_SEL_EVM_MASK)
#define RXPCU_MISC_MODE_SEL_EVM_RESET                                1
#define RXPCU_MISC_MODE_CLEAR_BA_VALID_MSB                           26
#define RXPCU_MISC_MODE_CLEAR_BA_VALID_LSB                           26
#define RXPCU_MISC_MODE_CLEAR_BA_VALID_MASK                          0x04000000
#define RXPCU_MISC_MODE_CLEAR_BA_VALID_GET(x)                        (((x) & RXPCU_MISC_MODE_CLEAR_BA_VALID_MASK) >> RXPCU_MISC_MODE_CLEAR_BA_VALID_LSB)
#define RXPCU_MISC_MODE_CLEAR_BA_VALID_SET(x)                        (((0x0 | (x)) << RXPCU_MISC_MODE_CLEAR_BA_VALID_LSB) & RXPCU_MISC_MODE_CLEAR_BA_VALID_MASK)
#define RXPCU_MISC_MODE_CLEAR_BA_VALID_RESET                         0
#define RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_MSB                       23
#define RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_LSB                       23
#define RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_MASK                      0x00800000
#define RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_GET(x)                    (((x) & RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_MASK) >> RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_LSB)
#define RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_SET(x)                    (((0x0 | (x)) << RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_LSB) & RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_MASK)
#define RXPCU_MISC_MODE_RX_HCF_POLL_ENABLE_RESET                     1
#define RXPCU_MISC_MODE_TBTT_PROTECT_MSB                             21
#define RXPCU_MISC_MODE_TBTT_PROTECT_LSB                             21
#define RXPCU_MISC_MODE_TBTT_PROTECT_MASK                            0x00200000
#define RXPCU_MISC_MODE_TBTT_PROTECT_GET(x)                          (((x) & RXPCU_MISC_MODE_TBTT_PROTECT_MASK) >> RXPCU_MISC_MODE_TBTT_PROTECT_LSB)
#define RXPCU_MISC_MODE_TBTT_PROTECT_SET(x)                          (((0x0 | (x)) << RXPCU_MISC_MODE_TBTT_PROTECT_LSB) & RXPCU_MISC_MODE_TBTT_PROTECT_MASK)
#define RXPCU_MISC_MODE_TBTT_PROTECT_RESET                           1
#define RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_MSB                       20
#define RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_LSB                       20
#define RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_MASK                      0x00100000
#define RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_GET(x)                    (((x) & RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_MASK) >> RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_LSB)
#define RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_SET(x)                    (((0x0 | (x)) << RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_LSB) & RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_MASK)
#define RXPCU_MISC_MODE_BT_ANT_PREVENTS_RX_RESET                     1
#define RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_MSB                    18
#define RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_LSB                    18
#define RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_MASK                   0x00040000
#define RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_GET(x)                 (((x) & RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_MASK) >> RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_LSB)
#define RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_SET(x)                 (((0x0 | (x)) << RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_LSB) & RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_MASK)
#define RXPCU_MISC_MODE_FORCE_QUIET_COLLISION_RESET                  0
#define RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_MSB                     14
#define RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_LSB                     14
#define RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_MASK                    0x00004000
#define RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_GET(x)                  (((x) & RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_MASK) >> RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_LSB)
#define RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_SET(x)                  (((0x0 | (x)) << RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_LSB) & RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_MASK)
#define RXPCU_MISC_MODE_MISS_BEACON_IN_SLEEP_RESET                   1
#define RXPCU_MISC_MODE_DEBUG_MODE_SIFS_MSB                          10
#define RXPCU_MISC_MODE_DEBUG_MODE_SIFS_LSB                          10
#define RXPCU_MISC_MODE_DEBUG_MODE_SIFS_MASK                         0x00000400
#define RXPCU_MISC_MODE_DEBUG_MODE_SIFS_GET(x)                       (((x) & RXPCU_MISC_MODE_DEBUG_MODE_SIFS_MASK) >> RXPCU_MISC_MODE_DEBUG_MODE_SIFS_LSB)
#define RXPCU_MISC_MODE_DEBUG_MODE_SIFS_SET(x)                       (((0x0 | (x)) << RXPCU_MISC_MODE_DEBUG_MODE_SIFS_LSB) & RXPCU_MISC_MODE_DEBUG_MODE_SIFS_MASK)
#define RXPCU_MISC_MODE_DEBUG_MODE_SIFS_RESET                        0
#define RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_MSB                     9
#define RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_LSB                     9
#define RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_MASK                    0x00000200
#define RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_GET(x)                  (((x) & RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_MASK) >> RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_LSB)
#define RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_SET(x)                  (((0x0 | (x)) << RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_LSB) & RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_MASK)
#define RXPCU_MISC_MODE_DEBUG_MODE_BA_BITMAP_RESET                   0
#define RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_MSB               6
#define RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_LSB               6
#define RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_MASK              0x00000040
#define RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_GET(x)            (((x) & RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_MASK) >> RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_LSB)
#define RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_SET(x)            (((0x0 | (x)) << RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_LSB) & RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_MASK)
#define RXPCU_MISC_MODE_RCV_DELAY_SOUNDING_IM_TXBF_RESET             0
#define RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_MSB                         5
#define RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_LSB                         5
#define RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_MASK                        0x00000020
#define RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_GET(x)                      (((x) & RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_MASK) >> RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_LSB)
#define RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_SET(x)                      (((0x0 | (x)) << RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_LSB) & RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_MASK)
#define RXPCU_MISC_MODE_RXSM2SVD_PRE_RST_RESET                       0
#define RXPCU_MISC_MODE_DEBUG_MODE_AD_MSB                            1
#define RXPCU_MISC_MODE_DEBUG_MODE_AD_LSB                            1
#define RXPCU_MISC_MODE_DEBUG_MODE_AD_MASK                           0x00000002
#define RXPCU_MISC_MODE_DEBUG_MODE_AD_GET(x)                         (((x) & RXPCU_MISC_MODE_DEBUG_MODE_AD_MASK) >> RXPCU_MISC_MODE_DEBUG_MODE_AD_LSB)
#define RXPCU_MISC_MODE_DEBUG_MODE_AD_SET(x)                         (((0x0 | (x)) << RXPCU_MISC_MODE_DEBUG_MODE_AD_LSB) & RXPCU_MISC_MODE_DEBUG_MODE_AD_MASK)
#define RXPCU_MISC_MODE_DEBUG_MODE_AD_RESET                          0
#define RXPCU_MISC_MODE_BSSID_MATCH_FORCE_MSB                        0
#define RXPCU_MISC_MODE_BSSID_MATCH_FORCE_LSB                        0
#define RXPCU_MISC_MODE_BSSID_MATCH_FORCE_MASK                       0x00000001
#define RXPCU_MISC_MODE_BSSID_MATCH_FORCE_GET(x)                     (((x) & RXPCU_MISC_MODE_BSSID_MATCH_FORCE_MASK) >> RXPCU_MISC_MODE_BSSID_MATCH_FORCE_LSB)
#define RXPCU_MISC_MODE_BSSID_MATCH_FORCE_SET(x)                     (((0x0 | (x)) << RXPCU_MISC_MODE_BSSID_MATCH_FORCE_LSB) & RXPCU_MISC_MODE_BSSID_MATCH_FORCE_MASK)
#define RXPCU_MISC_MODE_BSSID_MATCH_FORCE_RESET                      0
#define RXPCU_MISC_MODE_ADDRESS                                      0x0000d0
#define RXPCU_MISC_MODE_HW_MASK                                      0x9cb44663
#define RXPCU_MISC_MODE_SW_MASK                                      0x9cb44663
#define RXPCU_MISC_MODE_RSTMASK                                      0x08b04000
#define RXPCU_MISC_MODE_RESET                                        0x08b04000

// 0x00d4 (RXPCU_MISC_MODE2)
#define RXPCU_MISC_MODE2_PCU_LOOP_TXBF_MSB                           30
#define RXPCU_MISC_MODE2_PCU_LOOP_TXBF_LSB                           30
#define RXPCU_MISC_MODE2_PCU_LOOP_TXBF_MASK                          0x40000000
#define RXPCU_MISC_MODE2_PCU_LOOP_TXBF_GET(x)                        (((x) & RXPCU_MISC_MODE2_PCU_LOOP_TXBF_MASK) >> RXPCU_MISC_MODE2_PCU_LOOP_TXBF_LSB)
#define RXPCU_MISC_MODE2_PCU_LOOP_TXBF_SET(x)                        (((0x0 | (x)) << RXPCU_MISC_MODE2_PCU_LOOP_TXBF_LSB) & RXPCU_MISC_MODE2_PCU_LOOP_TXBF_MASK)
#define RXPCU_MISC_MODE2_PCU_LOOP_TXBF_RESET                         0
#define RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_MSB                      28
#define RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_LSB                      28
#define RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_MASK                     0x10000000
#define RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_GET(x)                   (((x) & RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_MASK) >> RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_LSB)
#define RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_SET(x)                   (((0x0 | (x)) << RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_LSB) & RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_MASK)
#define RXPCU_MISC_MODE2_H_TO_SW_DEBUG_MODE_RESET                    0
#define RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_MSB                     27
#define RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_LSB                     27
#define RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_MASK                    0x08000000
#define RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_GET(x)                  (((x) & RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_MASK) >> RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_LSB)
#define RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_SET(x)                  (((0x0 | (x)) << RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_LSB) & RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_MASK)
#define RXPCU_MISC_MODE2_DECOUPLE_DECRYPTION_RESET                   1
#define RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_MSB                       25
#define RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_LSB                       25
#define RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_MASK                      0x02000000
#define RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_GET(x)                    (((x) & RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_MASK) >> RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_LSB)
#define RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_SET(x)                    (((0x0 | (x)) << RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_LSB) & RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_MASK)
#define RXPCU_MISC_MODE2_RCV_TIMESTAMP_FIX_RESET                     1
#define RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_MSB                    23
#define RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_LSB                    23
#define RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_MASK                   0x00800000
#define RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_GET(x)                 (((x) & RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_MASK) >> RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_LSB)
#define RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_SET(x)                 (((0x0 | (x)) << RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_LSB) & RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_MASK)
#define RXPCU_MISC_MODE2_MPDU_DENSITY_STS_FIX_RESET                  0
#define RXPCU_MISC_MODE2_BUG_28676_MSB                               20
#define RXPCU_MISC_MODE2_BUG_28676_LSB                               20
#define RXPCU_MISC_MODE2_BUG_28676_MASK                              0x00100000
#define RXPCU_MISC_MODE2_BUG_28676_GET(x)                            (((x) & RXPCU_MISC_MODE2_BUG_28676_MASK) >> RXPCU_MISC_MODE2_BUG_28676_LSB)
#define RXPCU_MISC_MODE2_BUG_28676_SET(x)                            (((0x0 | (x)) << RXPCU_MISC_MODE2_BUG_28676_LSB) & RXPCU_MISC_MODE2_BUG_28676_MASK)
#define RXPCU_MISC_MODE2_BUG_28676_RESET                             1
#define RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_MSB         16
#define RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_LSB         16
#define RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_MASK        0x00010000
#define RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_GET(x)      (((x) & RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_MASK) >> RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_LSB)
#define RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_SET(x)      (((0x0 | (x)) << RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_LSB) & RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_MASK)
#define RXPCU_MISC_MODE2_ENABLE_LOAD_NAV_BEACON_DURATION_RESET       0
#define RXPCU_MISC_MODE2_CFP_IGNORE_MSB                              7
#define RXPCU_MISC_MODE2_CFP_IGNORE_LSB                              7
#define RXPCU_MISC_MODE2_CFP_IGNORE_MASK                             0x00000080
#define RXPCU_MISC_MODE2_CFP_IGNORE_GET(x)                           (((x) & RXPCU_MISC_MODE2_CFP_IGNORE_MASK) >> RXPCU_MISC_MODE2_CFP_IGNORE_LSB)
#define RXPCU_MISC_MODE2_CFP_IGNORE_SET(x)                           (((0x0 | (x)) << RXPCU_MISC_MODE2_CFP_IGNORE_LSB) & RXPCU_MISC_MODE2_CFP_IGNORE_MASK)
#define RXPCU_MISC_MODE2_CFP_IGNORE_RESET                            0
#define RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_MSB                    5
#define RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_LSB                    5
#define RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_MASK                   0x00000020
#define RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_GET(x)                 (((x) & RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_MASK) >> RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_LSB)
#define RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_SET(x)                 (((0x0 | (x)) << RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_LSB) & RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_MASK)
#define RXPCU_MISC_MODE2_EV_76218_FIX_DISABLE_RESET                  0
#define RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_MSB                    4
#define RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_LSB                    4
#define RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_MASK                   0x00000010
#define RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_GET(x)                 (((x) & RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_MASK) >> RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_LSB)
#define RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_SET(x)                 (((0x0 | (x)) << RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_LSB) & RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_MASK)
#define RXPCU_MISC_MODE2_BUG_58057_FIX_ENABLE_RESET                  1
#define RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_MSB              2
#define RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_LSB              2
#define RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_MASK             0x00000004
#define RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_GET(x)           (((x) & RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_MASK) >> RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_LSB)
#define RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_SET(x)           (((0x0 | (x)) << RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_LSB) & RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_MASK)
#define RXPCU_MISC_MODE2_NO_CRYPTO_FOR_NON_DATA_PKT_RESET            0
#define RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_MSB                    0
#define RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_LSB                    0
#define RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_MASK                   0x00000001
#define RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_GET(x)                 (((x) & RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_MASK) >> RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_LSB)
#define RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_SET(x)                 (((0x0 | (x)) << RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_LSB) & RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_MASK)
#define RXPCU_MISC_MODE2_BUG_21532_FIX_ENABLE_RESET                  1
#define RXPCU_MISC_MODE2_ADDRESS                                     0x0000d4
#define RXPCU_MISC_MODE2_HW_MASK                                     0x5a9100b5
#define RXPCU_MISC_MODE2_SW_MASK                                     0x5a9100b5
#define RXPCU_MISC_MODE2_RSTMASK                                     0x0a100011
#define RXPCU_MISC_MODE2_RESET                                       0x0a100011

// 0x00d8 (RXPCU_MISC_MODE3)
#define RXPCU_MISC_MODE3_KEY_MISS_FIX_MSB                            29
#define RXPCU_MISC_MODE3_KEY_MISS_FIX_LSB                            29
#define RXPCU_MISC_MODE3_KEY_MISS_FIX_MASK                           0x20000000
#define RXPCU_MISC_MODE3_KEY_MISS_FIX_GET(x)                         (((x) & RXPCU_MISC_MODE3_KEY_MISS_FIX_MASK) >> RXPCU_MISC_MODE3_KEY_MISS_FIX_LSB)
#define RXPCU_MISC_MODE3_KEY_MISS_FIX_SET(x)                         (((0x0 | (x)) << RXPCU_MISC_MODE3_KEY_MISS_FIX_LSB) & RXPCU_MISC_MODE3_KEY_MISS_FIX_MASK)
#define RXPCU_MISC_MODE3_KEY_MISS_FIX_RESET                          1
#define RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_MSB                 23
#define RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_LSB                 23
#define RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_MASK                0x00800000
#define RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_GET(x)              (((x) & RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_MASK) >> RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_LSB)
#define RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_SET(x)              (((0x0 | (x)) << RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_LSB) & RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_MASK)
#define RXPCU_MISC_MODE3_BEACON_FROM_TO_DS_CHECK_RESET               1
#define RXPCU_MISC_MODE3_ALLOW_RAC_MSB                               21
#define RXPCU_MISC_MODE3_ALLOW_RAC_LSB                               21
#define RXPCU_MISC_MODE3_ALLOW_RAC_MASK                              0x00200000
#define RXPCU_MISC_MODE3_ALLOW_RAC_GET(x)                            (((x) & RXPCU_MISC_MODE3_ALLOW_RAC_MASK) >> RXPCU_MISC_MODE3_ALLOW_RAC_LSB)
#define RXPCU_MISC_MODE3_ALLOW_RAC_SET(x)                            (((0x0 | (x)) << RXPCU_MISC_MODE3_ALLOW_RAC_LSB) & RXPCU_MISC_MODE3_ALLOW_RAC_MASK)
#define RXPCU_MISC_MODE3_ALLOW_RAC_RESET                             0
#define RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_MSB                    6
#define RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_LSB                    6
#define RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_MASK                   0x00000040
#define RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_GET(x)                 (((x) & RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_MASK) >> RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_LSB)
#define RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_SET(x)                 (((0x0 | (x)) << RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_LSB) & RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_MASK)
#define RXPCU_MISC_MODE3_BUG_61936_FIX_ENABLE_RESET                  0
#define RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_MSB                    4
#define RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_LSB                    4
#define RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_MASK                   0x00000010
#define RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_GET(x)                 (((x) & RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_MASK) >> RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_LSB)
#define RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_SET(x)                 (((0x0 | (x)) << RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_LSB) & RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_MASK)
#define RXPCU_MISC_MODE3_BUG_56991_FIX_ENABLE_RESET                  1
#define RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_MSB                    0
#define RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_LSB                    0
#define RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_MASK                   0x00000001
#define RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_GET(x)                 (((x) & RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_MASK) >> RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_LSB)
#define RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_SET(x)                 (((0x0 | (x)) << RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_LSB) & RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_MASK)
#define RXPCU_MISC_MODE3_BUG_55702_FIX_ENABLE_RESET                  1
#define RXPCU_MISC_MODE3_ADDRESS                                     0x0000d8
#define RXPCU_MISC_MODE3_HW_MASK                                     0x20a00051
#define RXPCU_MISC_MODE3_SW_MASK                                     0x20a00051
#define RXPCU_MISC_MODE3_RSTMASK                                     0x20800011
#define RXPCU_MISC_MODE3_RESET                                       0x20800011

// 0x00dc (RXPCU_MISC_MODE4)
#define RXPCU_MISC_MODE4_ENABLE_SF_MSB                               11
#define RXPCU_MISC_MODE4_ENABLE_SF_LSB                               11
#define RXPCU_MISC_MODE4_ENABLE_SF_MASK                              0x00000800
#define RXPCU_MISC_MODE4_ENABLE_SF_GET(x)                            (((x) & RXPCU_MISC_MODE4_ENABLE_SF_MASK) >> RXPCU_MISC_MODE4_ENABLE_SF_LSB)
#define RXPCU_MISC_MODE4_ENABLE_SF_SET(x)                            (((0x0 | (x)) << RXPCU_MISC_MODE4_ENABLE_SF_LSB) & RXPCU_MISC_MODE4_ENABLE_SF_MASK)
#define RXPCU_MISC_MODE4_ENABLE_SF_RESET                             0
#define RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_MSB                         10
#define RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_LSB                         10
#define RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_MASK                        0x00000400
#define RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_GET(x)                      (((x) & RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_MASK) >> RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_LSB)
#define RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_SET(x)                      (((0x0 | (x)) << RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_LSB) & RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_MASK)
#define RXPCU_MISC_MODE4_PEER_ENTRY_SWAP_RESET                       0
#define RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_MSB                     8
#define RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_LSB                     8
#define RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_MASK                    0x00000100
#define RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_GET(x)                  (((x) & RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_MASK) >> RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_LSB)
#define RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_SET(x)                  (((0x0 | (x)) << RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_LSB) & RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_MASK)
#define RXPCU_MISC_MODE4_EV_83864_FIX_ENABLE_RESET                   0
#define RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_MSB                     7
#define RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_LSB                     7
#define RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_MASK                    0x00000080
#define RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_GET(x)                  (((x) & RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_MASK) >> RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_LSB)
#define RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_SET(x)                  (((0x0 | (x)) << RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_LSB) & RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_MASK)
#define RXPCU_MISC_MODE4_EV_78561_FIX_ENABLE_RESET                   0
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_MSB                  3
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_LSB                  3
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_MASK                 0x00000008
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_GET(x)               (((x) & RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_MASK) >> RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_LSB)
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_SET(x)               (((0x0 | (x)) << RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_LSB) & RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_MASK)
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_AP_SEL_RESET                0
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_MSB                     2
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_LSB                     2
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_MASK                    0x00000004
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_GET(x)                  (((x) & RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_MASK) >> RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_LSB)
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_SET(x)                  (((0x0 | (x)) << RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_LSB) & RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_MASK)
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_EN_RESET                   0
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_MSB                        1
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_LSB                        1
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_MASK                       0x00000002
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_GET(x)                     (((x) & RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_MASK) >> RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_LSB)
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_SET(x)                     (((0x0 | (x)) << RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_LSB) & RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_MASK)
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE2_RESET                      0
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_MSB                         0
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_LSB                         0
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_MASK                        0x00000001
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_GET(x)                      (((x) & RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_MASK) >> RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_LSB)
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_SET(x)                      (((0x0 | (x)) << RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_LSB) & RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_MASK)
#define RXPCU_MISC_MODE4_BC_MC_WAPI_MODE_RESET                       0
#define RXPCU_MISC_MODE4_ADDRESS                                     0x0000dc
#define RXPCU_MISC_MODE4_HW_MASK                                     0x00000d8f
#define RXPCU_MISC_MODE4_SW_MASK                                     0x00000d8f
#define RXPCU_MISC_MODE4_RSTMASK                                     0x00000000
#define RXPCU_MISC_MODE4_RESET                                       0x00000000

// 0x00e0 (RXPCU_DIAGNOSTIC_MODE)
#define RXPCU_DIAGNOSTIC_MODE_H_CV_MSB                               0
#define RXPCU_DIAGNOSTIC_MODE_H_CV_LSB                               0
#define RXPCU_DIAGNOSTIC_MODE_H_CV_MASK                              0x00000001
#define RXPCU_DIAGNOSTIC_MODE_H_CV_GET(x)                            (((x) & RXPCU_DIAGNOSTIC_MODE_H_CV_MASK) >> RXPCU_DIAGNOSTIC_MODE_H_CV_LSB)
#define RXPCU_DIAGNOSTIC_MODE_H_CV_SET(x)                            (((0x0 | (x)) << RXPCU_DIAGNOSTIC_MODE_H_CV_LSB) & RXPCU_DIAGNOSTIC_MODE_H_CV_MASK)
#define RXPCU_DIAGNOSTIC_MODE_H_CV_RESET                             0
#define RXPCU_DIAGNOSTIC_MODE_ADDRESS                                0x0000e0
#define RXPCU_DIAGNOSTIC_MODE_HW_MASK                                0x00000001
#define RXPCU_DIAGNOSTIC_MODE_SW_MASK                                0x00000001
#define RXPCU_DIAGNOSTIC_MODE_RSTMASK                                0x00000000
#define RXPCU_DIAGNOSTIC_MODE_RESET                                  0x00000000

// 0x00f0 (RXPCU_FCS_FAIL_CNT)
#define RXPCU_FCS_FAIL_CNT_VALUE_MSB                                 15
#define RXPCU_FCS_FAIL_CNT_VALUE_LSB                                 0
#define RXPCU_FCS_FAIL_CNT_VALUE_MASK                                0x0000ffff
#define RXPCU_FCS_FAIL_CNT_VALUE_GET(x)                              (((x) & RXPCU_FCS_FAIL_CNT_VALUE_MASK) >> RXPCU_FCS_FAIL_CNT_VALUE_LSB)
#define RXPCU_FCS_FAIL_CNT_VALUE_SET(x)                              (((0x0 | (x)) << RXPCU_FCS_FAIL_CNT_VALUE_LSB) & RXPCU_FCS_FAIL_CNT_VALUE_MASK)
#define RXPCU_FCS_FAIL_CNT_VALUE_RESET                               0
#define RXPCU_FCS_FAIL_CNT_ADDRESS                                   0x0000f0
#define RXPCU_FCS_FAIL_CNT_HW_MASK                                   0x0000ffff
#define RXPCU_FCS_FAIL_CNT_SW_MASK                                   0x0000ffff
#define RXPCU_FCS_FAIL_CNT_RSTMASK                                   0x00000000
#define RXPCU_FCS_FAIL_CNT_RESET                                     0x00000000

// 0x00f4 (RXPCU_BEACON_CNT)
#define RXPCU_BEACON_CNT_VALUE_MSB                                   15
#define RXPCU_BEACON_CNT_VALUE_LSB                                   0
#define RXPCU_BEACON_CNT_VALUE_MASK                                  0x0000ffff
#define RXPCU_BEACON_CNT_VALUE_GET(x)                                (((x) & RXPCU_BEACON_CNT_VALUE_MASK) >> RXPCU_BEACON_CNT_VALUE_LSB)
#define RXPCU_BEACON_CNT_VALUE_SET(x)                                (((0x0 | (x)) << RXPCU_BEACON_CNT_VALUE_LSB) & RXPCU_BEACON_CNT_VALUE_MASK)
#define RXPCU_BEACON_CNT_VALUE_RESET                                 0
#define RXPCU_BEACON_CNT_ADDRESS                                     0x0000f4
#define RXPCU_BEACON_CNT_HW_MASK                                     0x0000ffff
#define RXPCU_BEACON_CNT_SW_MASK                                     0x0000ffff
#define RXPCU_BEACON_CNT_RSTMASK                                     0x00000000
#define RXPCU_BEACON_CNT_RESET                                       0x00000000

// 0x00f8 (RXPCU_BEACON2_CNT)
#define RXPCU_BEACON2_CNT_VALUE_MSB                                  15
#define RXPCU_BEACON2_CNT_VALUE_LSB                                  0
#define RXPCU_BEACON2_CNT_VALUE_MASK                                 0x0000ffff
#define RXPCU_BEACON2_CNT_VALUE_GET(x)                               (((x) & RXPCU_BEACON2_CNT_VALUE_MASK) >> RXPCU_BEACON2_CNT_VALUE_LSB)
#define RXPCU_BEACON2_CNT_VALUE_SET(x)                               (((0x0 | (x)) << RXPCU_BEACON2_CNT_VALUE_LSB) & RXPCU_BEACON2_CNT_VALUE_MASK)
#define RXPCU_BEACON2_CNT_VALUE_RESET                                0
#define RXPCU_BEACON2_CNT_ADDRESS                                    0x0000f8
#define RXPCU_BEACON2_CNT_HW_MASK                                    0x0000ffff
#define RXPCU_BEACON2_CNT_SW_MASK                                    0x0000ffff
#define RXPCU_BEACON2_CNT_RSTMASK                                    0x00000000
#define RXPCU_BEACON2_CNT_RESET                                      0x00000000

// 0x00fc (RXPCU_RX_FRAME_CNT)
#define RXPCU_RX_FRAME_CNT_VALUE_MSB                                 31
#define RXPCU_RX_FRAME_CNT_VALUE_LSB                                 0
#define RXPCU_RX_FRAME_CNT_VALUE_MASK                                0xffffffff
#define RXPCU_RX_FRAME_CNT_VALUE_GET(x)                              (((x) & RXPCU_RX_FRAME_CNT_VALUE_MASK) >> RXPCU_RX_FRAME_CNT_VALUE_LSB)
#define RXPCU_RX_FRAME_CNT_VALUE_SET(x)                              (((0x0 | (x)) << RXPCU_RX_FRAME_CNT_VALUE_LSB) & RXPCU_RX_FRAME_CNT_VALUE_MASK)
#define RXPCU_RX_FRAME_CNT_VALUE_RESET                               0
#define RXPCU_RX_FRAME_CNT_ADDRESS                                   0x0000fc
#define RXPCU_RX_FRAME_CNT_HW_MASK                                   0xffffffff
#define RXPCU_RX_FRAME_CNT_SW_MASK                                   0xffffffff
#define RXPCU_RX_FRAME_CNT_RSTMASK                                   0x00000000
#define RXPCU_RX_FRAME_CNT_RESET                                     0x00000000

// 0x0100 (RXPCU_RX_CLEAR_CNT)
#define RXPCU_RX_CLEAR_CNT_VALUE_MSB                                 31
#define RXPCU_RX_CLEAR_CNT_VALUE_LSB                                 0
#define RXPCU_RX_CLEAR_CNT_VALUE_MASK                                0xffffffff
#define RXPCU_RX_CLEAR_CNT_VALUE_GET(x)                              (((x) & RXPCU_RX_CLEAR_CNT_VALUE_MASK) >> RXPCU_RX_CLEAR_CNT_VALUE_LSB)
#define RXPCU_RX_CLEAR_CNT_VALUE_SET(x)                              (((0x0 | (x)) << RXPCU_RX_CLEAR_CNT_VALUE_LSB) & RXPCU_RX_CLEAR_CNT_VALUE_MASK)
#define RXPCU_RX_CLEAR_CNT_VALUE_RESET                               0
#define RXPCU_RX_CLEAR_CNT_ADDRESS                                   0x000100
#define RXPCU_RX_CLEAR_CNT_HW_MASK                                   0xffffffff
#define RXPCU_RX_CLEAR_CNT_SW_MASK                                   0xffffffff
#define RXPCU_RX_CLEAR_CNT_RSTMASK                                   0x00000000
#define RXPCU_RX_CLEAR_CNT_RESET                                     0x00000000

// 0x0104 (RXPCU_FILTER_OFDM_CNT)
#define RXPCU_FILTER_OFDM_CNT_VALUE_MSB                              23
#define RXPCU_FILTER_OFDM_CNT_VALUE_LSB                              0
#define RXPCU_FILTER_OFDM_CNT_VALUE_MASK                             0x00ffffff
#define RXPCU_FILTER_OFDM_CNT_VALUE_GET(x)                           (((x) & RXPCU_FILTER_OFDM_CNT_VALUE_MASK) >> RXPCU_FILTER_OFDM_CNT_VALUE_LSB)
#define RXPCU_FILTER_OFDM_CNT_VALUE_SET(x)                           (((0x0 | (x)) << RXPCU_FILTER_OFDM_CNT_VALUE_LSB) & RXPCU_FILTER_OFDM_CNT_VALUE_MASK)
#define RXPCU_FILTER_OFDM_CNT_VALUE_RESET                            0
#define RXPCU_FILTER_OFDM_CNT_ADDRESS                                0x000104
#define RXPCU_FILTER_OFDM_CNT_HW_MASK                                0x00ffffff
#define RXPCU_FILTER_OFDM_CNT_SW_MASK                                0x00ffffff
#define RXPCU_FILTER_OFDM_CNT_RSTMASK                                0x00000000
#define RXPCU_FILTER_OFDM_CNT_RESET                                  0x00000000

// 0x0108 (RXPCU_FILTER_CCK_CNT)
#define RXPCU_FILTER_CCK_CNT_VALUE_MSB                               23
#define RXPCU_FILTER_CCK_CNT_VALUE_LSB                               0
#define RXPCU_FILTER_CCK_CNT_VALUE_MASK                              0x00ffffff
#define RXPCU_FILTER_CCK_CNT_VALUE_GET(x)                            (((x) & RXPCU_FILTER_CCK_CNT_VALUE_MASK) >> RXPCU_FILTER_CCK_CNT_VALUE_LSB)
#define RXPCU_FILTER_CCK_CNT_VALUE_SET(x)                            (((0x0 | (x)) << RXPCU_FILTER_CCK_CNT_VALUE_LSB) & RXPCU_FILTER_CCK_CNT_VALUE_MASK)
#define RXPCU_FILTER_CCK_CNT_VALUE_RESET                             0
#define RXPCU_FILTER_CCK_CNT_ADDRESS                                 0x000108
#define RXPCU_FILTER_CCK_CNT_HW_MASK                                 0x00ffffff
#define RXPCU_FILTER_CCK_CNT_SW_MASK                                 0x00ffffff
#define RXPCU_FILTER_CCK_CNT_RSTMASK                                 0x00000000
#define RXPCU_FILTER_CCK_CNT_RESET                                   0x00000000

// 0x0120 (RXPCU_PHY_ERROR_MASK)
#define RXPCU_PHY_ERROR_MASK_VALUE_MSB                               31
#define RXPCU_PHY_ERROR_MASK_VALUE_LSB                               0
#define RXPCU_PHY_ERROR_MASK_VALUE_MASK                              0xffffffff
#define RXPCU_PHY_ERROR_MASK_VALUE_GET(x)                            (((x) & RXPCU_PHY_ERROR_MASK_VALUE_MASK) >> RXPCU_PHY_ERROR_MASK_VALUE_LSB)
#define RXPCU_PHY_ERROR_MASK_VALUE_SET(x)                            (((0x0 | (x)) << RXPCU_PHY_ERROR_MASK_VALUE_LSB) & RXPCU_PHY_ERROR_MASK_VALUE_MASK)
#define RXPCU_PHY_ERROR_MASK_VALUE_RESET                             0
#define RXPCU_PHY_ERROR_MASK_ADDRESS                                 0x000120
#define RXPCU_PHY_ERROR_MASK_HW_MASK                                 0xffffffff
#define RXPCU_PHY_ERROR_MASK_SW_MASK                                 0xffffffff
#define RXPCU_PHY_ERROR_MASK_RSTMASK                                 0x00000000
#define RXPCU_PHY_ERROR_MASK_RESET                                   0x00000000

// 0x0124 (RXPCU_PHY_ERROR_MASK_CONT)
#define RXPCU_PHY_ERROR_MASK_CONT_VALUE_MSB                          31
#define RXPCU_PHY_ERROR_MASK_CONT_VALUE_LSB                          0
#define RXPCU_PHY_ERROR_MASK_CONT_VALUE_MASK                         0xffffffff
#define RXPCU_PHY_ERROR_MASK_CONT_VALUE_GET(x)                       (((x) & RXPCU_PHY_ERROR_MASK_CONT_VALUE_MASK) >> RXPCU_PHY_ERROR_MASK_CONT_VALUE_LSB)
#define RXPCU_PHY_ERROR_MASK_CONT_VALUE_SET(x)                       (((0x0 | (x)) << RXPCU_PHY_ERROR_MASK_CONT_VALUE_LSB) & RXPCU_PHY_ERROR_MASK_CONT_VALUE_MASK)
#define RXPCU_PHY_ERROR_MASK_CONT_VALUE_RESET                        0
#define RXPCU_PHY_ERROR_MASK_CONT_ADDRESS                            0x000124
#define RXPCU_PHY_ERROR_MASK_CONT_HW_MASK                            0xffffffff
#define RXPCU_PHY_ERROR_MASK_CONT_SW_MASK                            0xffffffff
#define RXPCU_PHY_ERROR_MASK_CONT_RSTMASK                            0x00000000
#define RXPCU_PHY_ERROR_MASK_CONT_RESET                              0x00000000

// 0x0128 (RXPCU_PHY_ERROR_EIFS_MASK)
#define RXPCU_PHY_ERROR_EIFS_MASK_VALUE_MSB                          31
#define RXPCU_PHY_ERROR_EIFS_MASK_VALUE_LSB                          0
#define RXPCU_PHY_ERROR_EIFS_MASK_VALUE_MASK                         0xffffffff
#define RXPCU_PHY_ERROR_EIFS_MASK_VALUE_GET(x)                       (((x) & RXPCU_PHY_ERROR_EIFS_MASK_VALUE_MASK) >> RXPCU_PHY_ERROR_EIFS_MASK_VALUE_LSB)
#define RXPCU_PHY_ERROR_EIFS_MASK_VALUE_SET(x)                       (((0x0 | (x)) << RXPCU_PHY_ERROR_EIFS_MASK_VALUE_LSB) & RXPCU_PHY_ERROR_EIFS_MASK_VALUE_MASK)
#define RXPCU_PHY_ERROR_EIFS_MASK_VALUE_RESET                        0
#define RXPCU_PHY_ERROR_EIFS_MASK_ADDRESS                            0x000128
#define RXPCU_PHY_ERROR_EIFS_MASK_HW_MASK                            0xffffffff
#define RXPCU_PHY_ERROR_EIFS_MASK_SW_MASK                            0xffffffff
#define RXPCU_PHY_ERROR_EIFS_MASK_RSTMASK                            0x00000000
#define RXPCU_PHY_ERROR_EIFS_MASK_RESET                              0x00000000

// 0x012c (RXPCU_PHY_ERROR_EIFS_MASK_CONT)
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_MSB                     31
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_LSB                     0
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_MASK                    0xffffffff
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_GET(x)                  (((x) & RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_MASK) >> RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_LSB)
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_SET(x)                  (((0x0 | (x)) << RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_LSB) & RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_MASK)
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_VALUE_RESET                   0
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_ADDRESS                       0x00012c
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_HW_MASK                       0xffffffff
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_SW_MASK                       0xffffffff
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_RSTMASK                       0x00000000
#define RXPCU_PHY_ERROR_EIFS_MASK_CONT_RESET                         0x00000000

// 0x0130 (RXPCU_PHY_ERROR_AIFS)
#define RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_MSB                         0
#define RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_LSB                         0
#define RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_MASK                        0x00000001
#define RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_GET(x)                      (((x) & RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_MASK) >> RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_LSB)
#define RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_SET(x)                      (((0x0 | (x)) << RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_LSB) & RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_MASK)
#define RXPCU_PHY_ERROR_AIFS_MASK_ENABLE_RESET                       0
#define RXPCU_PHY_ERROR_AIFS_ADDRESS                                 0x000130
#define RXPCU_PHY_ERROR_AIFS_HW_MASK                                 0x00000001
#define RXPCU_PHY_ERROR_AIFS_SW_MASK                                 0x00000001
#define RXPCU_PHY_ERROR_AIFS_RSTMASK                                 0x00000000
#define RXPCU_PHY_ERROR_AIFS_RESET                                   0x00000000

// 0x0134 (RXPCU_PHY_ERROR_AIFS_MASK)
#define RXPCU_PHY_ERROR_AIFS_MASK_VALUE_MSB                          31
#define RXPCU_PHY_ERROR_AIFS_MASK_VALUE_LSB                          0
#define RXPCU_PHY_ERROR_AIFS_MASK_VALUE_MASK                         0xffffffff
#define RXPCU_PHY_ERROR_AIFS_MASK_VALUE_GET(x)                       (((x) & RXPCU_PHY_ERROR_AIFS_MASK_VALUE_MASK) >> RXPCU_PHY_ERROR_AIFS_MASK_VALUE_LSB)
#define RXPCU_PHY_ERROR_AIFS_MASK_VALUE_SET(x)                       (((0x0 | (x)) << RXPCU_PHY_ERROR_AIFS_MASK_VALUE_LSB) & RXPCU_PHY_ERROR_AIFS_MASK_VALUE_MASK)
#define RXPCU_PHY_ERROR_AIFS_MASK_VALUE_RESET                        0
#define RXPCU_PHY_ERROR_AIFS_MASK_ADDRESS                            0x000134
#define RXPCU_PHY_ERROR_AIFS_MASK_HW_MASK                            0xffffffff
#define RXPCU_PHY_ERROR_AIFS_MASK_SW_MASK                            0xffffffff
#define RXPCU_PHY_ERROR_AIFS_MASK_RSTMASK                            0x00000000
#define RXPCU_PHY_ERROR_AIFS_MASK_RESET                              0x00000000

// 0x0138 (RXPCU_PHY_ERROR_AIFS_MASK_CONT)
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_MSB                     31
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_LSB                     0
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_MASK                    0xffffffff
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_GET(x)                  (((x) & RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_MASK) >> RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_LSB)
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_SET(x)                  (((0x0 | (x)) << RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_LSB) & RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_MASK)
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_VALUE_RESET                   0
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_ADDRESS                       0x000138
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_HW_MASK                       0xffffffff
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_SW_MASK                       0xffffffff
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_RSTMASK                       0x00000000
#define RXPCU_PHY_ERROR_AIFS_MASK_CONT_RESET                         0x00000000

// 0x013c (RXPCU_PHY_ERR_CNT_1)
#define RXPCU_PHY_ERR_CNT_1_VALUE_MSB                                23
#define RXPCU_PHY_ERR_CNT_1_VALUE_LSB                                0
#define RXPCU_PHY_ERR_CNT_1_VALUE_MASK                               0x00ffffff
#define RXPCU_PHY_ERR_CNT_1_VALUE_GET(x)                             (((x) & RXPCU_PHY_ERR_CNT_1_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_1_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_1_VALUE_SET(x)                             (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_1_VALUE_LSB) & RXPCU_PHY_ERR_CNT_1_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_1_VALUE_RESET                              0
#define RXPCU_PHY_ERR_CNT_1_ADDRESS                                  0x00013c
#define RXPCU_PHY_ERR_CNT_1_HW_MASK                                  0x00ffffff
#define RXPCU_PHY_ERR_CNT_1_SW_MASK                                  0x00ffffff
#define RXPCU_PHY_ERR_CNT_1_RSTMASK                                  0x00000000
#define RXPCU_PHY_ERR_CNT_1_RESET                                    0x00000000

// 0x0140 (RXPCU_PHY_ERR_CNT_1_MASK)
#define RXPCU_PHY_ERR_CNT_1_MASK_VALUE_MSB                           31
#define RXPCU_PHY_ERR_CNT_1_MASK_VALUE_LSB                           0
#define RXPCU_PHY_ERR_CNT_1_MASK_VALUE_MASK                          0xffffffff
#define RXPCU_PHY_ERR_CNT_1_MASK_VALUE_GET(x)                        (((x) & RXPCU_PHY_ERR_CNT_1_MASK_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_1_MASK_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_1_MASK_VALUE_SET(x)                        (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_1_MASK_VALUE_LSB) & RXPCU_PHY_ERR_CNT_1_MASK_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_1_MASK_VALUE_RESET                         0
#define RXPCU_PHY_ERR_CNT_1_MASK_ADDRESS                             0x000140
#define RXPCU_PHY_ERR_CNT_1_MASK_HW_MASK                             0xffffffff
#define RXPCU_PHY_ERR_CNT_1_MASK_SW_MASK                             0xffffffff
#define RXPCU_PHY_ERR_CNT_1_MASK_RSTMASK                             0x00000000
#define RXPCU_PHY_ERR_CNT_1_MASK_RESET                               0x00000000

// 0x0144 (RXPCU_PHY_ERR_CNT_1_MASK_CONT)
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_MSB                      31
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_LSB                      0
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_MASK                     0xffffffff
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_GET(x)                   (((x) & RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_SET(x)                   (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_LSB) & RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_VALUE_RESET                    0
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_ADDRESS                        0x000144
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_HW_MASK                        0xffffffff
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_SW_MASK                        0xffffffff
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_RSTMASK                        0x00000000
#define RXPCU_PHY_ERR_CNT_1_MASK_CONT_RESET                          0x00000000

// 0x0148 (RXPCU_PHY_ERR_CNT_2)
#define RXPCU_PHY_ERR_CNT_2_VALUE_MSB                                23
#define RXPCU_PHY_ERR_CNT_2_VALUE_LSB                                0
#define RXPCU_PHY_ERR_CNT_2_VALUE_MASK                               0x00ffffff
#define RXPCU_PHY_ERR_CNT_2_VALUE_GET(x)                             (((x) & RXPCU_PHY_ERR_CNT_2_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_2_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_2_VALUE_SET(x)                             (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_2_VALUE_LSB) & RXPCU_PHY_ERR_CNT_2_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_2_VALUE_RESET                              0
#define RXPCU_PHY_ERR_CNT_2_ADDRESS                                  0x000148
#define RXPCU_PHY_ERR_CNT_2_HW_MASK                                  0x00ffffff
#define RXPCU_PHY_ERR_CNT_2_SW_MASK                                  0x00ffffff
#define RXPCU_PHY_ERR_CNT_2_RSTMASK                                  0x00000000
#define RXPCU_PHY_ERR_CNT_2_RESET                                    0x00000000

// 0x014c (RXPCU_PHY_ERR_CNT_2_MASK)
#define RXPCU_PHY_ERR_CNT_2_MASK_VALUE_MSB                           31
#define RXPCU_PHY_ERR_CNT_2_MASK_VALUE_LSB                           0
#define RXPCU_PHY_ERR_CNT_2_MASK_VALUE_MASK                          0xffffffff
#define RXPCU_PHY_ERR_CNT_2_MASK_VALUE_GET(x)                        (((x) & RXPCU_PHY_ERR_CNT_2_MASK_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_2_MASK_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_2_MASK_VALUE_SET(x)                        (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_2_MASK_VALUE_LSB) & RXPCU_PHY_ERR_CNT_2_MASK_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_2_MASK_VALUE_RESET                         0
#define RXPCU_PHY_ERR_CNT_2_MASK_ADDRESS                             0x00014c
#define RXPCU_PHY_ERR_CNT_2_MASK_HW_MASK                             0xffffffff
#define RXPCU_PHY_ERR_CNT_2_MASK_SW_MASK                             0xffffffff
#define RXPCU_PHY_ERR_CNT_2_MASK_RSTMASK                             0x00000000
#define RXPCU_PHY_ERR_CNT_2_MASK_RESET                               0x00000000

// 0x0150 (RXPCU_PHY_ERR_CNT_2_MASK_CONT)
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_MSB                      31
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_LSB                      0
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_MASK                     0xffffffff
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_GET(x)                   (((x) & RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_SET(x)                   (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_LSB) & RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_VALUE_RESET                    0
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_ADDRESS                        0x000150
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_HW_MASK                        0xffffffff
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_SW_MASK                        0xffffffff
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_RSTMASK                        0x00000000
#define RXPCU_PHY_ERR_CNT_2_MASK_CONT_RESET                          0x00000000

// 0x0154 (RXPCU_PHY_ERR_CNT_3)
#define RXPCU_PHY_ERR_CNT_3_VALUE_MSB                                23
#define RXPCU_PHY_ERR_CNT_3_VALUE_LSB                                0
#define RXPCU_PHY_ERR_CNT_3_VALUE_MASK                               0x00ffffff
#define RXPCU_PHY_ERR_CNT_3_VALUE_GET(x)                             (((x) & RXPCU_PHY_ERR_CNT_3_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_3_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_3_VALUE_SET(x)                             (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_3_VALUE_LSB) & RXPCU_PHY_ERR_CNT_3_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_3_VALUE_RESET                              0
#define RXPCU_PHY_ERR_CNT_3_ADDRESS                                  0x000154
#define RXPCU_PHY_ERR_CNT_3_HW_MASK                                  0x00ffffff
#define RXPCU_PHY_ERR_CNT_3_SW_MASK                                  0x00ffffff
#define RXPCU_PHY_ERR_CNT_3_RSTMASK                                  0x00000000
#define RXPCU_PHY_ERR_CNT_3_RESET                                    0x00000000

// 0x0158 (RXPCU_PHY_ERR_CNT_3_MASK)
#define RXPCU_PHY_ERR_CNT_3_MASK_VALUE_MSB                           31
#define RXPCU_PHY_ERR_CNT_3_MASK_VALUE_LSB                           0
#define RXPCU_PHY_ERR_CNT_3_MASK_VALUE_MASK                          0xffffffff
#define RXPCU_PHY_ERR_CNT_3_MASK_VALUE_GET(x)                        (((x) & RXPCU_PHY_ERR_CNT_3_MASK_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_3_MASK_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_3_MASK_VALUE_SET(x)                        (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_3_MASK_VALUE_LSB) & RXPCU_PHY_ERR_CNT_3_MASK_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_3_MASK_VALUE_RESET                         0
#define RXPCU_PHY_ERR_CNT_3_MASK_ADDRESS                             0x000158
#define RXPCU_PHY_ERR_CNT_3_MASK_HW_MASK                             0xffffffff
#define RXPCU_PHY_ERR_CNT_3_MASK_SW_MASK                             0xffffffff
#define RXPCU_PHY_ERR_CNT_3_MASK_RSTMASK                             0x00000000
#define RXPCU_PHY_ERR_CNT_3_MASK_RESET                               0x00000000

// 0x015c (RXPCU_PHY_ERR_CNT_3_MASK_CONT)
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_MSB                      31
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_LSB                      0
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_MASK                     0xffffffff
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_GET(x)                   (((x) & RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_MASK) >> RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_LSB)
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_SET(x)                   (((0x0 | (x)) << RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_LSB) & RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_MASK)
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_VALUE_RESET                    0
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_ADDRESS                        0x00015c
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_HW_MASK                        0xffffffff
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_SW_MASK                        0xffffffff
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_RSTMASK                        0x00000000
#define RXPCU_PHY_ERR_CNT_3_MASK_CONT_RESET                          0x00000000

// 0x0184 (RXPCU_PHY_ERROR_CODE)
#define RXPCU_PHY_ERROR_CODE_VALUE_MSB                               7
#define RXPCU_PHY_ERROR_CODE_VALUE_LSB                               0
#define RXPCU_PHY_ERROR_CODE_VALUE_MASK                              0x000000ff
#define RXPCU_PHY_ERROR_CODE_VALUE_GET(x)                            (((x) & RXPCU_PHY_ERROR_CODE_VALUE_MASK) >> RXPCU_PHY_ERROR_CODE_VALUE_LSB)
#define RXPCU_PHY_ERROR_CODE_VALUE_SET(x)                            (((0x0 | (x)) << RXPCU_PHY_ERROR_CODE_VALUE_LSB) & RXPCU_PHY_ERROR_CODE_VALUE_MASK)
#define RXPCU_PHY_ERROR_CODE_VALUE_RESET                             0
#define RXPCU_PHY_ERROR_CODE_ADDRESS                                 0x000184
#define RXPCU_PHY_ERROR_CODE_HW_MASK                                 0x000000ff
#define RXPCU_PHY_ERROR_CODE_SW_MASK                                 0x000000ff
#define RXPCU_PHY_ERROR_CODE_RSTMASK                                 0x00000000
#define RXPCU_PHY_ERROR_CODE_RESET                                   0x00000000

// 0x0188 (RXPCU_RX_INT_STATUS0)
#define RXPCU_RX_INT_STATUS0_DURATION_H_MSB                          31
#define RXPCU_RX_INT_STATUS0_DURATION_H_LSB                          24
#define RXPCU_RX_INT_STATUS0_DURATION_H_MASK                         0xff000000
#define RXPCU_RX_INT_STATUS0_DURATION_H_GET(x)                       (((x) & RXPCU_RX_INT_STATUS0_DURATION_H_MASK) >> RXPCU_RX_INT_STATUS0_DURATION_H_LSB)
#define RXPCU_RX_INT_STATUS0_DURATION_H_SET(x)                       (((0x0 | (x)) << RXPCU_RX_INT_STATUS0_DURATION_H_LSB) & RXPCU_RX_INT_STATUS0_DURATION_H_MASK)
#define RXPCU_RX_INT_STATUS0_DURATION_H_RESET                        0
#define RXPCU_RX_INT_STATUS0_DURATION_L_MSB                          23
#define RXPCU_RX_INT_STATUS0_DURATION_L_LSB                          16
#define RXPCU_RX_INT_STATUS0_DURATION_L_MASK                         0x00ff0000
#define RXPCU_RX_INT_STATUS0_DURATION_L_GET(x)                       (((x) & RXPCU_RX_INT_STATUS0_DURATION_L_MASK) >> RXPCU_RX_INT_STATUS0_DURATION_L_LSB)
#define RXPCU_RX_INT_STATUS0_DURATION_L_SET(x)                       (((0x0 | (x)) << RXPCU_RX_INT_STATUS0_DURATION_L_LSB) & RXPCU_RX_INT_STATUS0_DURATION_L_MASK)
#define RXPCU_RX_INT_STATUS0_DURATION_L_RESET                        0
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_MSB                     15
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_LSB                     8
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_MASK                    0x0000ff00
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_GET(x)                  (((x) & RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_MASK) >> RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_LSB)
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_SET(x)                  (((0x0 | (x)) << RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_LSB) & RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_MASK)
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_H_RESET                   0
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_MSB                     7
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_LSB                     0
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_MASK                    0x000000ff
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_GET(x)                  (((x) & RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_MASK) >> RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_LSB)
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_SET(x)                  (((0x0 | (x)) << RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_LSB) & RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_MASK)
#define RXPCU_RX_INT_STATUS0_FRAME_CONTROL_L_RESET                   0
#define RXPCU_RX_INT_STATUS0_ADDRESS                                 0x000188
#define RXPCU_RX_INT_STATUS0_HW_MASK                                 0xffffffff
#define RXPCU_RX_INT_STATUS0_SW_MASK                                 0xffffffff
#define RXPCU_RX_INT_STATUS0_RSTMASK                                 0x00000000
#define RXPCU_RX_INT_STATUS0_RESET                                   0x00000000

// 0x018c (RXPCU_RX_INT_STATUS1)
#define RXPCU_RX_INT_STATUS1_VALUE_MSB                               17
#define RXPCU_RX_INT_STATUS1_VALUE_LSB                               0
#define RXPCU_RX_INT_STATUS1_VALUE_MASK                              0x0003ffff
#define RXPCU_RX_INT_STATUS1_VALUE_GET(x)                            (((x) & RXPCU_RX_INT_STATUS1_VALUE_MASK) >> RXPCU_RX_INT_STATUS1_VALUE_LSB)
#define RXPCU_RX_INT_STATUS1_VALUE_SET(x)                            (((0x0 | (x)) << RXPCU_RX_INT_STATUS1_VALUE_LSB) & RXPCU_RX_INT_STATUS1_VALUE_MASK)
#define RXPCU_RX_INT_STATUS1_VALUE_RESET                             0
#define RXPCU_RX_INT_STATUS1_ADDRESS                                 0x00018c
#define RXPCU_RX_INT_STATUS1_HW_MASK                                 0x0003ffff
#define RXPCU_RX_INT_STATUS1_SW_MASK                                 0x0003ffff
#define RXPCU_RX_INT_STATUS1_RSTMASK                                 0x00000000
#define RXPCU_RX_INT_STATUS1_RESET                                   0x00000000

// 0x0190 (RXPCU_RX_INT_STATUS2)
#define RXPCU_RX_INT_STATUS2_VALUE_MSB                               26
#define RXPCU_RX_INT_STATUS2_VALUE_LSB                               0
#define RXPCU_RX_INT_STATUS2_VALUE_MASK                              0x07ffffff
#define RXPCU_RX_INT_STATUS2_VALUE_GET(x)                            (((x) & RXPCU_RX_INT_STATUS2_VALUE_MASK) >> RXPCU_RX_INT_STATUS2_VALUE_LSB)
#define RXPCU_RX_INT_STATUS2_VALUE_SET(x)                            (((0x0 | (x)) << RXPCU_RX_INT_STATUS2_VALUE_LSB) & RXPCU_RX_INT_STATUS2_VALUE_MASK)
#define RXPCU_RX_INT_STATUS2_VALUE_RESET                             0
#define RXPCU_RX_INT_STATUS2_ADDRESS                                 0x000190
#define RXPCU_RX_INT_STATUS2_HW_MASK                                 0x07ffffff
#define RXPCU_RX_INT_STATUS2_SW_MASK                                 0x07ffffff
#define RXPCU_RX_INT_STATUS2_RSTMASK                                 0x00000000
#define RXPCU_RX_INT_STATUS2_RESET                                   0x00000000

// 0x0194 (RXPCU_RX_INT_STATUS3)
#define RXPCU_RX_INT_STATUS3_VALUE_MSB                               23
#define RXPCU_RX_INT_STATUS3_VALUE_LSB                               0
#define RXPCU_RX_INT_STATUS3_VALUE_MASK                              0x00ffffff
#define RXPCU_RX_INT_STATUS3_VALUE_GET(x)                            (((x) & RXPCU_RX_INT_STATUS3_VALUE_MASK) >> RXPCU_RX_INT_STATUS3_VALUE_LSB)
#define RXPCU_RX_INT_STATUS3_VALUE_SET(x)                            (((0x0 | (x)) << RXPCU_RX_INT_STATUS3_VALUE_LSB) & RXPCU_RX_INT_STATUS3_VALUE_MASK)
#define RXPCU_RX_INT_STATUS3_VALUE_RESET                             0
#define RXPCU_RX_INT_STATUS3_ADDRESS                                 0x000194
#define RXPCU_RX_INT_STATUS3_HW_MASK                                 0x00ffffff
#define RXPCU_RX_INT_STATUS3_SW_MASK                                 0x00ffffff
#define RXPCU_RX_INT_STATUS3_RSTMASK                                 0x00000000
#define RXPCU_RX_INT_STATUS3_RESET                                   0x00000000

// 0x0198 (RX_INT_FILTER)
#define RX_INT_FILTER_RSSI_HIGH_MSB                                  18
#define RX_INT_FILTER_RSSI_HIGH_LSB                                  18
#define RX_INT_FILTER_RSSI_HIGH_MASK                                 0x00040000
#define RX_INT_FILTER_RSSI_HIGH_GET(x)                               (((x) & RX_INT_FILTER_RSSI_HIGH_MASK) >> RX_INT_FILTER_RSSI_HIGH_LSB)
#define RX_INT_FILTER_RSSI_HIGH_SET(x)                               (((0x0 | (x)) << RX_INT_FILTER_RSSI_HIGH_LSB) & RX_INT_FILTER_RSSI_HIGH_MASK)
#define RX_INT_FILTER_RSSI_HIGH_RESET                                1
#define RX_INT_FILTER_BEACON_MSB                                     17
#define RX_INT_FILTER_BEACON_LSB                                     17
#define RX_INT_FILTER_BEACON_MASK                                    0x00020000
#define RX_INT_FILTER_BEACON_GET(x)                                  (((x) & RX_INT_FILTER_BEACON_MASK) >> RX_INT_FILTER_BEACON_LSB)
#define RX_INT_FILTER_BEACON_SET(x)                                  (((0x0 | (x)) << RX_INT_FILTER_BEACON_LSB) & RX_INT_FILTER_BEACON_MASK)
#define RX_INT_FILTER_BEACON_RESET                                   1
#define RX_INT_FILTER_AMPDU_MSB                                      16
#define RX_INT_FILTER_AMPDU_LSB                                      16
#define RX_INT_FILTER_AMPDU_MASK                                     0x00010000
#define RX_INT_FILTER_AMPDU_GET(x)                                   (((x) & RX_INT_FILTER_AMPDU_MASK) >> RX_INT_FILTER_AMPDU_LSB)
#define RX_INT_FILTER_AMPDU_SET(x)                                   (((0x0 | (x)) << RX_INT_FILTER_AMPDU_LSB) & RX_INT_FILTER_AMPDU_MASK)
#define RX_INT_FILTER_AMPDU_RESET                                    1
#define RX_INT_FILTER_EOSP_MSB                                       15
#define RX_INT_FILTER_EOSP_LSB                                       15
#define RX_INT_FILTER_EOSP_MASK                                      0x00008000
#define RX_INT_FILTER_EOSP_GET(x)                                    (((x) & RX_INT_FILTER_EOSP_MASK) >> RX_INT_FILTER_EOSP_LSB)
#define RX_INT_FILTER_EOSP_SET(x)                                    (((0x0 | (x)) << RX_INT_FILTER_EOSP_LSB) & RX_INT_FILTER_EOSP_MASK)
#define RX_INT_FILTER_EOSP_RESET                                     1
#define RX_INT_FILTER_LENGTH_LOW_MSB                                 14
#define RX_INT_FILTER_LENGTH_LOW_LSB                                 14
#define RX_INT_FILTER_LENGTH_LOW_MASK                                0x00004000
#define RX_INT_FILTER_LENGTH_LOW_GET(x)                              (((x) & RX_INT_FILTER_LENGTH_LOW_MASK) >> RX_INT_FILTER_LENGTH_LOW_LSB)
#define RX_INT_FILTER_LENGTH_LOW_SET(x)                              (((0x0 | (x)) << RX_INT_FILTER_LENGTH_LOW_LSB) & RX_INT_FILTER_LENGTH_LOW_MASK)
#define RX_INT_FILTER_LENGTH_LOW_RESET                               1
#define RX_INT_FILTER_LENGTH_HIGH_MSB                                13
#define RX_INT_FILTER_LENGTH_HIGH_LSB                                13
#define RX_INT_FILTER_LENGTH_HIGH_MASK                               0x00002000
#define RX_INT_FILTER_LENGTH_HIGH_GET(x)                             (((x) & RX_INT_FILTER_LENGTH_HIGH_MASK) >> RX_INT_FILTER_LENGTH_HIGH_LSB)
#define RX_INT_FILTER_LENGTH_HIGH_SET(x)                             (((0x0 | (x)) << RX_INT_FILTER_LENGTH_HIGH_LSB) & RX_INT_FILTER_LENGTH_HIGH_MASK)
#define RX_INT_FILTER_LENGTH_HIGH_RESET                              1
#define RX_INT_FILTER_RSSI_MSB                                       12
#define RX_INT_FILTER_RSSI_LSB                                       12
#define RX_INT_FILTER_RSSI_MASK                                      0x00001000
#define RX_INT_FILTER_RSSI_GET(x)                                    (((x) & RX_INT_FILTER_RSSI_MASK) >> RX_INT_FILTER_RSSI_LSB)
#define RX_INT_FILTER_RSSI_SET(x)                                    (((0x0 | (x)) << RX_INT_FILTER_RSSI_LSB) & RX_INT_FILTER_RSSI_MASK)
#define RX_INT_FILTER_RSSI_RESET                                     1
#define RX_INT_FILTER_RATE_LOW_MSB                                   11
#define RX_INT_FILTER_RATE_LOW_LSB                                   11
#define RX_INT_FILTER_RATE_LOW_MASK                                  0x00000800
#define RX_INT_FILTER_RATE_LOW_GET(x)                                (((x) & RX_INT_FILTER_RATE_LOW_MASK) >> RX_INT_FILTER_RATE_LOW_LSB)
#define RX_INT_FILTER_RATE_LOW_SET(x)                                (((0x0 | (x)) << RX_INT_FILTER_RATE_LOW_LSB) & RX_INT_FILTER_RATE_LOW_MASK)
#define RX_INT_FILTER_RATE_LOW_RESET                                 1
#define RX_INT_FILTER_RATE_HIGH_MSB                                  10
#define RX_INT_FILTER_RATE_HIGH_LSB                                  10
#define RX_INT_FILTER_RATE_HIGH_MASK                                 0x00000400
#define RX_INT_FILTER_RATE_HIGH_GET(x)                               (((x) & RX_INT_FILTER_RATE_HIGH_MASK) >> RX_INT_FILTER_RATE_HIGH_LSB)
#define RX_INT_FILTER_RATE_HIGH_SET(x)                               (((0x0 | (x)) << RX_INT_FILTER_RATE_HIGH_LSB) & RX_INT_FILTER_RATE_HIGH_MASK)
#define RX_INT_FILTER_RATE_HIGH_RESET                                1
#define RX_INT_FILTER_MORE_FRAG_MSB                                  9
#define RX_INT_FILTER_MORE_FRAG_LSB                                  9
#define RX_INT_FILTER_MORE_FRAG_MASK                                 0x00000200
#define RX_INT_FILTER_MORE_FRAG_GET(x)                               (((x) & RX_INT_FILTER_MORE_FRAG_MASK) >> RX_INT_FILTER_MORE_FRAG_LSB)
#define RX_INT_FILTER_MORE_FRAG_SET(x)                               (((0x0 | (x)) << RX_INT_FILTER_MORE_FRAG_LSB) & RX_INT_FILTER_MORE_FRAG_MASK)
#define RX_INT_FILTER_MORE_FRAG_RESET                                1
#define RX_INT_FILTER_MORE_DATA_MSB                                  8
#define RX_INT_FILTER_MORE_DATA_LSB                                  8
#define RX_INT_FILTER_MORE_DATA_MASK                                 0x00000100
#define RX_INT_FILTER_MORE_DATA_GET(x)                               (((x) & RX_INT_FILTER_MORE_DATA_MASK) >> RX_INT_FILTER_MORE_DATA_LSB)
#define RX_INT_FILTER_MORE_DATA_SET(x)                               (((0x0 | (x)) << RX_INT_FILTER_MORE_DATA_LSB) & RX_INT_FILTER_MORE_DATA_MASK)
#define RX_INT_FILTER_MORE_DATA_RESET                                1
#define RX_INT_FILTER_RETRY_MSB                                      7
#define RX_INT_FILTER_RETRY_LSB                                      7
#define RX_INT_FILTER_RETRY_MASK                                     0x00000080
#define RX_INT_FILTER_RETRY_GET(x)                                   (((x) & RX_INT_FILTER_RETRY_MASK) >> RX_INT_FILTER_RETRY_LSB)
#define RX_INT_FILTER_RETRY_SET(x)                                   (((0x0 | (x)) << RX_INT_FILTER_RETRY_LSB) & RX_INT_FILTER_RETRY_MASK)
#define RX_INT_FILTER_RETRY_RESET                                    1
#define RX_INT_FILTER_CTS_MSB                                        6
#define RX_INT_FILTER_CTS_LSB                                        6
#define RX_INT_FILTER_CTS_MASK                                       0x00000040
#define RX_INT_FILTER_CTS_GET(x)                                     (((x) & RX_INT_FILTER_CTS_MASK) >> RX_INT_FILTER_CTS_LSB)
#define RX_INT_FILTER_CTS_SET(x)                                     (((0x0 | (x)) << RX_INT_FILTER_CTS_LSB) & RX_INT_FILTER_CTS_MASK)
#define RX_INT_FILTER_CTS_RESET                                      1
#define RX_INT_FILTER_ACK_MSB                                        5
#define RX_INT_FILTER_ACK_LSB                                        5
#define RX_INT_FILTER_ACK_MASK                                       0x00000020
#define RX_INT_FILTER_ACK_GET(x)                                     (((x) & RX_INT_FILTER_ACK_MASK) >> RX_INT_FILTER_ACK_LSB)
#define RX_INT_FILTER_ACK_SET(x)                                     (((0x0 | (x)) << RX_INT_FILTER_ACK_LSB) & RX_INT_FILTER_ACK_MASK)
#define RX_INT_FILTER_ACK_RESET                                      1
#define RX_INT_FILTER_RTS_MSB                                        4
#define RX_INT_FILTER_RTS_LSB                                        4
#define RX_INT_FILTER_RTS_MASK                                       0x00000010
#define RX_INT_FILTER_RTS_GET(x)                                     (((x) & RX_INT_FILTER_RTS_MASK) >> RX_INT_FILTER_RTS_LSB)
#define RX_INT_FILTER_RTS_SET(x)                                     (((0x0 | (x)) << RX_INT_FILTER_RTS_LSB) & RX_INT_FILTER_RTS_MASK)
#define RX_INT_FILTER_RTS_RESET                                      1
#define RX_INT_FILTER_MCAST_MSB                                      3
#define RX_INT_FILTER_MCAST_LSB                                      3
#define RX_INT_FILTER_MCAST_MASK                                     0x00000008
#define RX_INT_FILTER_MCAST_GET(x)                                   (((x) & RX_INT_FILTER_MCAST_MASK) >> RX_INT_FILTER_MCAST_LSB)
#define RX_INT_FILTER_MCAST_SET(x)                                   (((0x0 | (x)) << RX_INT_FILTER_MCAST_LSB) & RX_INT_FILTER_MCAST_MASK)
#define RX_INT_FILTER_MCAST_RESET                                    1
#define RX_INT_FILTER_BCAST_MSB                                      2
#define RX_INT_FILTER_BCAST_LSB                                      2
#define RX_INT_FILTER_BCAST_MASK                                     0x00000004
#define RX_INT_FILTER_BCAST_GET(x)                                   (((x) & RX_INT_FILTER_BCAST_MASK) >> RX_INT_FILTER_BCAST_LSB)
#define RX_INT_FILTER_BCAST_SET(x)                                   (((0x0 | (x)) << RX_INT_FILTER_BCAST_LSB) & RX_INT_FILTER_BCAST_MASK)
#define RX_INT_FILTER_BCAST_RESET                                    1
#define RX_INT_FILTER_DIRECTED_MSB                                   1
#define RX_INT_FILTER_DIRECTED_LSB                                   1
#define RX_INT_FILTER_DIRECTED_MASK                                  0x00000002
#define RX_INT_FILTER_DIRECTED_GET(x)                                (((x) & RX_INT_FILTER_DIRECTED_MASK) >> RX_INT_FILTER_DIRECTED_LSB)
#define RX_INT_FILTER_DIRECTED_SET(x)                                (((0x0 | (x)) << RX_INT_FILTER_DIRECTED_LSB) & RX_INT_FILTER_DIRECTED_MASK)
#define RX_INT_FILTER_DIRECTED_RESET                                 1
#define RX_INT_FILTER_ENABLE_MSB                                     0
#define RX_INT_FILTER_ENABLE_LSB                                     0
#define RX_INT_FILTER_ENABLE_MASK                                    0x00000001
#define RX_INT_FILTER_ENABLE_GET(x)                                  (((x) & RX_INT_FILTER_ENABLE_MASK) >> RX_INT_FILTER_ENABLE_LSB)
#define RX_INT_FILTER_ENABLE_SET(x)                                  (((0x0 | (x)) << RX_INT_FILTER_ENABLE_LSB) & RX_INT_FILTER_ENABLE_MASK)
#define RX_INT_FILTER_ENABLE_RESET                                   1
#define RX_INT_FILTER_ADDRESS                                        0x000198
#define RX_INT_FILTER_HW_MASK                                        0x0007ffff
#define RX_INT_FILTER_SW_MASK                                        0x0007ffff
#define RX_INT_FILTER_RSTMASK                                        0x0007ffff
#define RX_INT_FILTER_RESET                                          0x0007ffff

// 0x019c (RX_INT_OVERFLOW)
#define RX_INT_OVERFLOW_STATUS_MSB                                   0
#define RX_INT_OVERFLOW_STATUS_LSB                                   0
#define RX_INT_OVERFLOW_STATUS_MASK                                  0x00000001
#define RX_INT_OVERFLOW_STATUS_GET(x)                                (((x) & RX_INT_OVERFLOW_STATUS_MASK) >> RX_INT_OVERFLOW_STATUS_LSB)
#define RX_INT_OVERFLOW_STATUS_SET(x)                                (((0x0 | (x)) << RX_INT_OVERFLOW_STATUS_LSB) & RX_INT_OVERFLOW_STATUS_MASK)
#define RX_INT_OVERFLOW_STATUS_RESET                                 1
#define RX_INT_OVERFLOW_ADDRESS                                      0x00019c
#define RX_INT_OVERFLOW_HW_MASK                                      0x00000001
#define RX_INT_OVERFLOW_SW_MASK                                      0x00000001
#define RX_INT_OVERFLOW_RSTMASK                                      0x00000001
#define RX_INT_OVERFLOW_RESET                                        0x00000001

// 0x01a0 (RX_FILTER_THRESH0)
#define RX_FILTER_THRESH0_RSSI_HIGH_MSB                              31
#define RX_FILTER_THRESH0_RSSI_HIGH_LSB                              24
#define RX_FILTER_THRESH0_RSSI_HIGH_MASK                             0xff000000
#define RX_FILTER_THRESH0_RSSI_HIGH_GET(x)                           (((x) & RX_FILTER_THRESH0_RSSI_HIGH_MASK) >> RX_FILTER_THRESH0_RSSI_HIGH_LSB)
#define RX_FILTER_THRESH0_RSSI_HIGH_SET(x)                           (((0x0 | (x)) << RX_FILTER_THRESH0_RSSI_HIGH_LSB) & RX_FILTER_THRESH0_RSSI_HIGH_MASK)
#define RX_FILTER_THRESH0_RSSI_HIGH_RESET                            1
#define RX_FILTER_THRESH0_RSSI_LOW_MSB                               23
#define RX_FILTER_THRESH0_RSSI_LOW_LSB                               16
#define RX_FILTER_THRESH0_RSSI_LOW_MASK                              0x00ff0000
#define RX_FILTER_THRESH0_RSSI_LOW_GET(x)                            (((x) & RX_FILTER_THRESH0_RSSI_LOW_MASK) >> RX_FILTER_THRESH0_RSSI_LOW_LSB)
#define RX_FILTER_THRESH0_RSSI_LOW_SET(x)                            (((0x0 | (x)) << RX_FILTER_THRESH0_RSSI_LOW_LSB) & RX_FILTER_THRESH0_RSSI_LOW_MASK)
#define RX_FILTER_THRESH0_RSSI_LOW_RESET                             1
#define RX_FILTER_THRESH0_RATE_LOW_MSB                               15
#define RX_FILTER_THRESH0_RATE_LOW_LSB                               8
#define RX_FILTER_THRESH0_RATE_LOW_MASK                              0x0000ff00
#define RX_FILTER_THRESH0_RATE_LOW_GET(x)                            (((x) & RX_FILTER_THRESH0_RATE_LOW_MASK) >> RX_FILTER_THRESH0_RATE_LOW_LSB)
#define RX_FILTER_THRESH0_RATE_LOW_SET(x)                            (((0x0 | (x)) << RX_FILTER_THRESH0_RATE_LOW_LSB) & RX_FILTER_THRESH0_RATE_LOW_MASK)
#define RX_FILTER_THRESH0_RATE_LOW_RESET                             0
#define RX_FILTER_THRESH0_RATE_HIGH_MSB                              7
#define RX_FILTER_THRESH0_RATE_HIGH_LSB                              0
#define RX_FILTER_THRESH0_RATE_HIGH_MASK                             0x000000ff
#define RX_FILTER_THRESH0_RATE_HIGH_GET(x)                           (((x) & RX_FILTER_THRESH0_RATE_HIGH_MASK) >> RX_FILTER_THRESH0_RATE_HIGH_LSB)
#define RX_FILTER_THRESH0_RATE_HIGH_SET(x)                           (((0x0 | (x)) << RX_FILTER_THRESH0_RATE_HIGH_LSB) & RX_FILTER_THRESH0_RATE_HIGH_MASK)
#define RX_FILTER_THRESH0_RATE_HIGH_RESET                            0
#define RX_FILTER_THRESH0_ADDRESS                                    0x0001a0
#define RX_FILTER_THRESH0_HW_MASK                                    0xffffffff
#define RX_FILTER_THRESH0_SW_MASK                                    0xffffffff
#define RX_FILTER_THRESH0_RSTMASK                                    0xffff0000
#define RX_FILTER_THRESH0_RESET                                      0x01010000

// 0x01a4 (RX_FILTER_THRESH1)
#define RX_FILTER_THRESH1_LENGTH_LOW_MSB                             23
#define RX_FILTER_THRESH1_LENGTH_LOW_LSB                             12
#define RX_FILTER_THRESH1_LENGTH_LOW_MASK                            0x00fff000
#define RX_FILTER_THRESH1_LENGTH_LOW_GET(x)                          (((x) & RX_FILTER_THRESH1_LENGTH_LOW_MASK) >> RX_FILTER_THRESH1_LENGTH_LOW_LSB)
#define RX_FILTER_THRESH1_LENGTH_LOW_SET(x)                          (((0x0 | (x)) << RX_FILTER_THRESH1_LENGTH_LOW_LSB) & RX_FILTER_THRESH1_LENGTH_LOW_MASK)
#define RX_FILTER_THRESH1_LENGTH_LOW_RESET                           0
#define RX_FILTER_THRESH1_LENGTH_HIGH_MSB                            11
#define RX_FILTER_THRESH1_LENGTH_HIGH_LSB                            0
#define RX_FILTER_THRESH1_LENGTH_HIGH_MASK                           0x00000fff
#define RX_FILTER_THRESH1_LENGTH_HIGH_GET(x)                         (((x) & RX_FILTER_THRESH1_LENGTH_HIGH_MASK) >> RX_FILTER_THRESH1_LENGTH_HIGH_LSB)
#define RX_FILTER_THRESH1_LENGTH_HIGH_SET(x)                         (((0x0 | (x)) << RX_FILTER_THRESH1_LENGTH_HIGH_LSB) & RX_FILTER_THRESH1_LENGTH_HIGH_MASK)
#define RX_FILTER_THRESH1_LENGTH_HIGH_RESET                          4095
#define RX_FILTER_THRESH1_ADDRESS                                    0x0001a4
#define RX_FILTER_THRESH1_HW_MASK                                    0x00ffffff
#define RX_FILTER_THRESH1_SW_MASK                                    0x00ffffff
#define RX_FILTER_THRESH1_RSTMASK                                    0x00000fff
#define RX_FILTER_THRESH1_RESET                                      0x00000fff

// 0x01a8 (RX_PRIORITY_THRESH0)
#define RX_PRIORITY_THRESH0_RSSI_LOW_MSB                             31
#define RX_PRIORITY_THRESH0_RSSI_LOW_LSB                             24
#define RX_PRIORITY_THRESH0_RSSI_LOW_MASK                            0xff000000
#define RX_PRIORITY_THRESH0_RSSI_LOW_GET(x)                          (((x) & RX_PRIORITY_THRESH0_RSSI_LOW_MASK) >> RX_PRIORITY_THRESH0_RSSI_LOW_LSB)
#define RX_PRIORITY_THRESH0_RSSI_LOW_SET(x)                          (((0x0 | (x)) << RX_PRIORITY_THRESH0_RSSI_LOW_LSB) & RX_PRIORITY_THRESH0_RSSI_LOW_MASK)
#define RX_PRIORITY_THRESH0_RSSI_LOW_RESET                           1
#define RX_PRIORITY_THRESH0_RSSI_HIGH_MSB                            23
#define RX_PRIORITY_THRESH0_RSSI_HIGH_LSB                            16
#define RX_PRIORITY_THRESH0_RSSI_HIGH_MASK                           0x00ff0000
#define RX_PRIORITY_THRESH0_RSSI_HIGH_GET(x)                         (((x) & RX_PRIORITY_THRESH0_RSSI_HIGH_MASK) >> RX_PRIORITY_THRESH0_RSSI_HIGH_LSB)
#define RX_PRIORITY_THRESH0_RSSI_HIGH_SET(x)                         (((0x0 | (x)) << RX_PRIORITY_THRESH0_RSSI_HIGH_LSB) & RX_PRIORITY_THRESH0_RSSI_HIGH_MASK)
#define RX_PRIORITY_THRESH0_RSSI_HIGH_RESET                          1
#define RX_PRIORITY_THRESH0_RATE_LOW_MSB                             15
#define RX_PRIORITY_THRESH0_RATE_LOW_LSB                             8
#define RX_PRIORITY_THRESH0_RATE_LOW_MASK                            0x0000ff00
#define RX_PRIORITY_THRESH0_RATE_LOW_GET(x)                          (((x) & RX_PRIORITY_THRESH0_RATE_LOW_MASK) >> RX_PRIORITY_THRESH0_RATE_LOW_LSB)
#define RX_PRIORITY_THRESH0_RATE_LOW_SET(x)                          (((0x0 | (x)) << RX_PRIORITY_THRESH0_RATE_LOW_LSB) & RX_PRIORITY_THRESH0_RATE_LOW_MASK)
#define RX_PRIORITY_THRESH0_RATE_LOW_RESET                           0
#define RX_PRIORITY_THRESH0_RATE_HIGH_MSB                            7
#define RX_PRIORITY_THRESH0_RATE_HIGH_LSB                            0
#define RX_PRIORITY_THRESH0_RATE_HIGH_MASK                           0x000000ff
#define RX_PRIORITY_THRESH0_RATE_HIGH_GET(x)                         (((x) & RX_PRIORITY_THRESH0_RATE_HIGH_MASK) >> RX_PRIORITY_THRESH0_RATE_HIGH_LSB)
#define RX_PRIORITY_THRESH0_RATE_HIGH_SET(x)                         (((0x0 | (x)) << RX_PRIORITY_THRESH0_RATE_HIGH_LSB) & RX_PRIORITY_THRESH0_RATE_HIGH_MASK)
#define RX_PRIORITY_THRESH0_RATE_HIGH_RESET                          0
#define RX_PRIORITY_THRESH0_ADDRESS                                  0x0001a8
#define RX_PRIORITY_THRESH0_HW_MASK                                  0xffffffff
#define RX_PRIORITY_THRESH0_SW_MASK                                  0xffffffff
#define RX_PRIORITY_THRESH0_RSTMASK                                  0xffff0000
#define RX_PRIORITY_THRESH0_RESET                                    0x01010000

// 0x01ac (RX_PRIORITY_THRESH1)
#define RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_MSB                      31
#define RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_LSB                      24
#define RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_MASK                     0xff000000
#define RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_GET(x)                   (((x) & RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_MASK) >> RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_LSB)
#define RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_SET(x)                   (((0x0 | (x)) << RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_LSB) & RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_MASK)
#define RX_PRIORITY_THRESH1_XCAST_RSSI_HIGH_RESET                    1
#define RX_PRIORITY_THRESH1_LENGTH_LOW_MSB                           23
#define RX_PRIORITY_THRESH1_LENGTH_LOW_LSB                           12
#define RX_PRIORITY_THRESH1_LENGTH_LOW_MASK                          0x00fff000
#define RX_PRIORITY_THRESH1_LENGTH_LOW_GET(x)                        (((x) & RX_PRIORITY_THRESH1_LENGTH_LOW_MASK) >> RX_PRIORITY_THRESH1_LENGTH_LOW_LSB)
#define RX_PRIORITY_THRESH1_LENGTH_LOW_SET(x)                        (((0x0 | (x)) << RX_PRIORITY_THRESH1_LENGTH_LOW_LSB) & RX_PRIORITY_THRESH1_LENGTH_LOW_MASK)
#define RX_PRIORITY_THRESH1_LENGTH_LOW_RESET                         0
#define RX_PRIORITY_THRESH1_LENGTH_HIGH_MSB                          11
#define RX_PRIORITY_THRESH1_LENGTH_HIGH_LSB                          0
#define RX_PRIORITY_THRESH1_LENGTH_HIGH_MASK                         0x00000fff
#define RX_PRIORITY_THRESH1_LENGTH_HIGH_GET(x)                       (((x) & RX_PRIORITY_THRESH1_LENGTH_HIGH_MASK) >> RX_PRIORITY_THRESH1_LENGTH_HIGH_LSB)
#define RX_PRIORITY_THRESH1_LENGTH_HIGH_SET(x)                       (((0x0 | (x)) << RX_PRIORITY_THRESH1_LENGTH_HIGH_LSB) & RX_PRIORITY_THRESH1_LENGTH_HIGH_MASK)
#define RX_PRIORITY_THRESH1_LENGTH_HIGH_RESET                        4095
#define RX_PRIORITY_THRESH1_ADDRESS                                  0x0001ac
#define RX_PRIORITY_THRESH1_HW_MASK                                  0xffffffff
#define RX_PRIORITY_THRESH1_SW_MASK                                  0xffffffff
#define RX_PRIORITY_THRESH1_RSTMASK                                  0xff000fff
#define RX_PRIORITY_THRESH1_RESET                                    0x01000fff

// 0x01b0 (RX_PRIORITY_THRESH2)
#define RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_MSB                       31
#define RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_LSB                       24
#define RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_MASK                      0xff000000
#define RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_GET(x)                    (((x) & RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_MASK) >> RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_LSB)
#define RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_SET(x)                    (((0x0 | (x)) << RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_LSB) & RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_MASK)
#define RX_PRIORITY_THRESH2_NULL_RSSI_HIGH_RESET                     1
#define RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_MSB                     23
#define RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_LSB                     16
#define RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_MASK                    0x00ff0000
#define RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_GET(x)                  (((x) & RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_MASK) >> RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_LSB)
#define RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_SET(x)                  (((0x0 | (x)) << RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_LSB) & RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_MASK)
#define RX_PRIORITY_THRESH2_BEACON_RSSI_HIGH_RESET                   1
#define RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_MSB                       15
#define RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_LSB                       8
#define RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_MASK                      0x0000ff00
#define RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_GET(x)                    (((x) & RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_MASK) >> RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_LSB)
#define RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_SET(x)                    (((0x0 | (x)) << RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_LSB) & RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_MASK)
#define RX_PRIORITY_THRESH2_MGMT_RSSI_HIGH_RESET                     1
#define RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_MSB                      7
#define RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_LSB                      0
#define RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_MASK                     0x000000ff
#define RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_GET(x)                   (((x) & RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_MASK) >> RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_LSB)
#define RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_SET(x)                   (((0x0 | (x)) << RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_LSB) & RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_MASK)
#define RX_PRIORITY_THRESH2_PRESP_RSSI_HIGH_RESET                    1
#define RX_PRIORITY_THRESH2_ADDRESS                                  0x0001b0
#define RX_PRIORITY_THRESH2_HW_MASK                                  0xffffffff
#define RX_PRIORITY_THRESH2_SW_MASK                                  0xffffffff
#define RX_PRIORITY_THRESH2_RSTMASK                                  0xffffffff
#define RX_PRIORITY_THRESH2_RESET                                    0x01010101

// 0x01b4 (RX_PRIORITY_THRESH3)
#define RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_MSB                    15
#define RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_LSB                    8
#define RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_MASK                   0x0000ff00
#define RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_GET(x)                 (((x) & RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_MASK) >> RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_LSB)
#define RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_SET(x)                 (((0x0 | (x)) << RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_LSB) & RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_MASK)
#define RX_PRIORITY_THRESH3_PS_POLL_RSSI_HIGH_RESET                  1
#define RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_MSB                       7
#define RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_LSB                       0
#define RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_MASK                      0x000000ff
#define RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_GET(x)                    (((x) & RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_MASK) >> RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_LSB)
#define RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_SET(x)                    (((0x0 | (x)) << RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_LSB) & RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_MASK)
#define RX_PRIORITY_THRESH3_PREQ_RSSI_HIGH_RESET                     1
#define RX_PRIORITY_THRESH3_ADDRESS                                  0x0001b4
#define RX_PRIORITY_THRESH3_HW_MASK                                  0x0000ffff
#define RX_PRIORITY_THRESH3_SW_MASK                                  0x0000ffff
#define RX_PRIORITY_THRESH3_RSTMASK                                  0x0000ffff
#define RX_PRIORITY_THRESH3_RESET                                    0x00000101

// 0x01b8 (RX_PRIORITY_OFFSET0)
#define RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_MSB                      29
#define RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_LSB                      24
#define RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_MASK                     0x3f000000
#define RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_GET(x)                   (((x) & RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_MASK) >> RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_LSB)
#define RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_SET(x)                   (((0x0 | (x)) << RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_LSB) & RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_MASK)
#define RX_PRIORITY_OFFSET0_XCAST_RSSI_HIGH_RESET                    0
#define RX_PRIORITY_OFFSET0_RSSI_LOW_MSB                             23
#define RX_PRIORITY_OFFSET0_RSSI_LOW_LSB                             18
#define RX_PRIORITY_OFFSET0_RSSI_LOW_MASK                            0x00fc0000
#define RX_PRIORITY_OFFSET0_RSSI_LOW_GET(x)                          (((x) & RX_PRIORITY_OFFSET0_RSSI_LOW_MASK) >> RX_PRIORITY_OFFSET0_RSSI_LOW_LSB)
#define RX_PRIORITY_OFFSET0_RSSI_LOW_SET(x)                          (((0x0 | (x)) << RX_PRIORITY_OFFSET0_RSSI_LOW_LSB) & RX_PRIORITY_OFFSET0_RSSI_LOW_MASK)
#define RX_PRIORITY_OFFSET0_RSSI_LOW_RESET                           0
#define RX_PRIORITY_OFFSET0_RSSI_HIGH_MSB                            17
#define RX_PRIORITY_OFFSET0_RSSI_HIGH_LSB                            12
#define RX_PRIORITY_OFFSET0_RSSI_HIGH_MASK                           0x0003f000
#define RX_PRIORITY_OFFSET0_RSSI_HIGH_GET(x)                         (((x) & RX_PRIORITY_OFFSET0_RSSI_HIGH_MASK) >> RX_PRIORITY_OFFSET0_RSSI_HIGH_LSB)
#define RX_PRIORITY_OFFSET0_RSSI_HIGH_SET(x)                         (((0x0 | (x)) << RX_PRIORITY_OFFSET0_RSSI_HIGH_LSB) & RX_PRIORITY_OFFSET0_RSSI_HIGH_MASK)
#define RX_PRIORITY_OFFSET0_RSSI_HIGH_RESET                          0
#define RX_PRIORITY_OFFSET0_PHY_RATE_LOW_MSB                         11
#define RX_PRIORITY_OFFSET0_PHY_RATE_LOW_LSB                         6
#define RX_PRIORITY_OFFSET0_PHY_RATE_LOW_MASK                        0x00000fc0
#define RX_PRIORITY_OFFSET0_PHY_RATE_LOW_GET(x)                      (((x) & RX_PRIORITY_OFFSET0_PHY_RATE_LOW_MASK) >> RX_PRIORITY_OFFSET0_PHY_RATE_LOW_LSB)
#define RX_PRIORITY_OFFSET0_PHY_RATE_LOW_SET(x)                      (((0x0 | (x)) << RX_PRIORITY_OFFSET0_PHY_RATE_LOW_LSB) & RX_PRIORITY_OFFSET0_PHY_RATE_LOW_MASK)
#define RX_PRIORITY_OFFSET0_PHY_RATE_LOW_RESET                       0
#define RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_MSB                        5
#define RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_LSB                        0
#define RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_MASK                       0x0000003f
#define RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_GET(x)                     (((x) & RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_MASK) >> RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_LSB)
#define RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_SET(x)                     (((0x0 | (x)) << RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_LSB) & RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_MASK)
#define RX_PRIORITY_OFFSET0_PHY_RATE_HIGH_RESET                      0
#define RX_PRIORITY_OFFSET0_ADDRESS                                  0x0001b8
#define RX_PRIORITY_OFFSET0_HW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET0_SW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET0_RSTMASK                                  0x00000000
#define RX_PRIORITY_OFFSET0_RESET                                    0x00000000

// 0x01bc (RX_PRIORITY_OFFSET1)
#define RX_PRIORITY_OFFSET1_RTS_MSB                                  29
#define RX_PRIORITY_OFFSET1_RTS_LSB                                  24
#define RX_PRIORITY_OFFSET1_RTS_MASK                                 0x3f000000
#define RX_PRIORITY_OFFSET1_RTS_GET(x)                               (((x) & RX_PRIORITY_OFFSET1_RTS_MASK) >> RX_PRIORITY_OFFSET1_RTS_LSB)
#define RX_PRIORITY_OFFSET1_RTS_SET(x)                               (((0x0 | (x)) << RX_PRIORITY_OFFSET1_RTS_LSB) & RX_PRIORITY_OFFSET1_RTS_MASK)
#define RX_PRIORITY_OFFSET1_RTS_RESET                                0
#define RX_PRIORITY_OFFSET1_RETX_MSB                                 23
#define RX_PRIORITY_OFFSET1_RETX_LSB                                 18
#define RX_PRIORITY_OFFSET1_RETX_MASK                                0x00fc0000
#define RX_PRIORITY_OFFSET1_RETX_GET(x)                              (((x) & RX_PRIORITY_OFFSET1_RETX_MASK) >> RX_PRIORITY_OFFSET1_RETX_LSB)
#define RX_PRIORITY_OFFSET1_RETX_SET(x)                              (((0x0 | (x)) << RX_PRIORITY_OFFSET1_RETX_LSB) & RX_PRIORITY_OFFSET1_RETX_MASK)
#define RX_PRIORITY_OFFSET1_RETX_RESET                               0
#define RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_MSB                      17
#define RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_LSB                      12
#define RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_MASK                     0x0003f000
#define RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_GET(x)                   (((x) & RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_MASK) >> RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_LSB)
#define RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_SET(x)                   (((0x0 | (x)) << RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_LSB) & RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_MASK)
#define RX_PRIORITY_OFFSET1_PRESP_RSSI_HIGH_RESET                    0
#define RX_PRIORITY_OFFSET1_LENGTH_LOW_MSB                           11
#define RX_PRIORITY_OFFSET1_LENGTH_LOW_LSB                           6
#define RX_PRIORITY_OFFSET1_LENGTH_LOW_MASK                          0x00000fc0
#define RX_PRIORITY_OFFSET1_LENGTH_LOW_GET(x)                        (((x) & RX_PRIORITY_OFFSET1_LENGTH_LOW_MASK) >> RX_PRIORITY_OFFSET1_LENGTH_LOW_LSB)
#define RX_PRIORITY_OFFSET1_LENGTH_LOW_SET(x)                        (((0x0 | (x)) << RX_PRIORITY_OFFSET1_LENGTH_LOW_LSB) & RX_PRIORITY_OFFSET1_LENGTH_LOW_MASK)
#define RX_PRIORITY_OFFSET1_LENGTH_LOW_RESET                         0
#define RX_PRIORITY_OFFSET1_LENGTH_HIGH_MSB                          5
#define RX_PRIORITY_OFFSET1_LENGTH_HIGH_LSB                          0
#define RX_PRIORITY_OFFSET1_LENGTH_HIGH_MASK                         0x0000003f
#define RX_PRIORITY_OFFSET1_LENGTH_HIGH_GET(x)                       (((x) & RX_PRIORITY_OFFSET1_LENGTH_HIGH_MASK) >> RX_PRIORITY_OFFSET1_LENGTH_HIGH_LSB)
#define RX_PRIORITY_OFFSET1_LENGTH_HIGH_SET(x)                       (((0x0 | (x)) << RX_PRIORITY_OFFSET1_LENGTH_HIGH_LSB) & RX_PRIORITY_OFFSET1_LENGTH_HIGH_MASK)
#define RX_PRIORITY_OFFSET1_LENGTH_HIGH_RESET                        0
#define RX_PRIORITY_OFFSET1_ADDRESS                                  0x0001bc
#define RX_PRIORITY_OFFSET1_HW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET1_SW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET1_RSTMASK                                  0x00000000
#define RX_PRIORITY_OFFSET1_RESET                                    0x00000000

// 0x01c0 (RX_PRIORITY_OFFSET2)
#define RX_PRIORITY_OFFSET2_BEACON_MSB                               29
#define RX_PRIORITY_OFFSET2_BEACON_LSB                               24
#define RX_PRIORITY_OFFSET2_BEACON_MASK                              0x3f000000
#define RX_PRIORITY_OFFSET2_BEACON_GET(x)                            (((x) & RX_PRIORITY_OFFSET2_BEACON_MASK) >> RX_PRIORITY_OFFSET2_BEACON_LSB)
#define RX_PRIORITY_OFFSET2_BEACON_SET(x)                            (((0x0 | (x)) << RX_PRIORITY_OFFSET2_BEACON_LSB) & RX_PRIORITY_OFFSET2_BEACON_MASK)
#define RX_PRIORITY_OFFSET2_BEACON_RESET                             0
#define RX_PRIORITY_OFFSET2_MGMT_MSB                                 23
#define RX_PRIORITY_OFFSET2_MGMT_LSB                                 18
#define RX_PRIORITY_OFFSET2_MGMT_MASK                                0x00fc0000
#define RX_PRIORITY_OFFSET2_MGMT_GET(x)                              (((x) & RX_PRIORITY_OFFSET2_MGMT_MASK) >> RX_PRIORITY_OFFSET2_MGMT_LSB)
#define RX_PRIORITY_OFFSET2_MGMT_SET(x)                              (((0x0 | (x)) << RX_PRIORITY_OFFSET2_MGMT_LSB) & RX_PRIORITY_OFFSET2_MGMT_MASK)
#define RX_PRIORITY_OFFSET2_MGMT_RESET                               0
#define RX_PRIORITY_OFFSET2_ATIM_MSB                                 17
#define RX_PRIORITY_OFFSET2_ATIM_LSB                                 12
#define RX_PRIORITY_OFFSET2_ATIM_MASK                                0x0003f000
#define RX_PRIORITY_OFFSET2_ATIM_GET(x)                              (((x) & RX_PRIORITY_OFFSET2_ATIM_MASK) >> RX_PRIORITY_OFFSET2_ATIM_LSB)
#define RX_PRIORITY_OFFSET2_ATIM_SET(x)                              (((0x0 | (x)) << RX_PRIORITY_OFFSET2_ATIM_LSB) & RX_PRIORITY_OFFSET2_ATIM_MASK)
#define RX_PRIORITY_OFFSET2_ATIM_RESET                               0
#define RX_PRIORITY_OFFSET2_PRESP_MSB                                11
#define RX_PRIORITY_OFFSET2_PRESP_LSB                                6
#define RX_PRIORITY_OFFSET2_PRESP_MASK                               0x00000fc0
#define RX_PRIORITY_OFFSET2_PRESP_GET(x)                             (((x) & RX_PRIORITY_OFFSET2_PRESP_MASK) >> RX_PRIORITY_OFFSET2_PRESP_LSB)
#define RX_PRIORITY_OFFSET2_PRESP_SET(x)                             (((0x0 | (x)) << RX_PRIORITY_OFFSET2_PRESP_LSB) & RX_PRIORITY_OFFSET2_PRESP_MASK)
#define RX_PRIORITY_OFFSET2_PRESP_RESET                              0
#define RX_PRIORITY_OFFSET2_XCAST_MSB                                5
#define RX_PRIORITY_OFFSET2_XCAST_LSB                                0
#define RX_PRIORITY_OFFSET2_XCAST_MASK                               0x0000003f
#define RX_PRIORITY_OFFSET2_XCAST_GET(x)                             (((x) & RX_PRIORITY_OFFSET2_XCAST_MASK) >> RX_PRIORITY_OFFSET2_XCAST_LSB)
#define RX_PRIORITY_OFFSET2_XCAST_SET(x)                             (((0x0 | (x)) << RX_PRIORITY_OFFSET2_XCAST_LSB) & RX_PRIORITY_OFFSET2_XCAST_MASK)
#define RX_PRIORITY_OFFSET2_XCAST_RESET                              0
#define RX_PRIORITY_OFFSET2_ADDRESS                                  0x0001c0
#define RX_PRIORITY_OFFSET2_HW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET2_SW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET2_RSTMASK                                  0x00000000
#define RX_PRIORITY_OFFSET2_RESET                                    0x00000000

// 0x01c4 (RX_PRIORITY_OFFSET3)
#define RX_PRIORITY_OFFSET3_PS_POLL_MSB                              29
#define RX_PRIORITY_OFFSET3_PS_POLL_LSB                              24
#define RX_PRIORITY_OFFSET3_PS_POLL_MASK                             0x3f000000
#define RX_PRIORITY_OFFSET3_PS_POLL_GET(x)                           (((x) & RX_PRIORITY_OFFSET3_PS_POLL_MASK) >> RX_PRIORITY_OFFSET3_PS_POLL_LSB)
#define RX_PRIORITY_OFFSET3_PS_POLL_SET(x)                           (((0x0 | (x)) << RX_PRIORITY_OFFSET3_PS_POLL_LSB) & RX_PRIORITY_OFFSET3_PS_POLL_MASK)
#define RX_PRIORITY_OFFSET3_PS_POLL_RESET                            0
#define RX_PRIORITY_OFFSET3_AMSDU_MSB                                23
#define RX_PRIORITY_OFFSET3_AMSDU_LSB                                18
#define RX_PRIORITY_OFFSET3_AMSDU_MASK                               0x00fc0000
#define RX_PRIORITY_OFFSET3_AMSDU_GET(x)                             (((x) & RX_PRIORITY_OFFSET3_AMSDU_MASK) >> RX_PRIORITY_OFFSET3_AMSDU_LSB)
#define RX_PRIORITY_OFFSET3_AMSDU_SET(x)                             (((0x0 | (x)) << RX_PRIORITY_OFFSET3_AMSDU_LSB) & RX_PRIORITY_OFFSET3_AMSDU_MASK)
#define RX_PRIORITY_OFFSET3_AMSDU_RESET                              0
#define RX_PRIORITY_OFFSET3_AMPDU_MSB                                17
#define RX_PRIORITY_OFFSET3_AMPDU_LSB                                12
#define RX_PRIORITY_OFFSET3_AMPDU_MASK                               0x0003f000
#define RX_PRIORITY_OFFSET3_AMPDU_GET(x)                             (((x) & RX_PRIORITY_OFFSET3_AMPDU_MASK) >> RX_PRIORITY_OFFSET3_AMPDU_LSB)
#define RX_PRIORITY_OFFSET3_AMPDU_SET(x)                             (((0x0 | (x)) << RX_PRIORITY_OFFSET3_AMPDU_LSB) & RX_PRIORITY_OFFSET3_AMPDU_MASK)
#define RX_PRIORITY_OFFSET3_AMPDU_RESET                              0
#define RX_PRIORITY_OFFSET3_EOSP_MSB                                 11
#define RX_PRIORITY_OFFSET3_EOSP_LSB                                 6
#define RX_PRIORITY_OFFSET3_EOSP_MASK                                0x00000fc0
#define RX_PRIORITY_OFFSET3_EOSP_GET(x)                              (((x) & RX_PRIORITY_OFFSET3_EOSP_MASK) >> RX_PRIORITY_OFFSET3_EOSP_LSB)
#define RX_PRIORITY_OFFSET3_EOSP_SET(x)                              (((0x0 | (x)) << RX_PRIORITY_OFFSET3_EOSP_LSB) & RX_PRIORITY_OFFSET3_EOSP_MASK)
#define RX_PRIORITY_OFFSET3_EOSP_RESET                               0
#define RX_PRIORITY_OFFSET3_MORE_MSB                                 5
#define RX_PRIORITY_OFFSET3_MORE_LSB                                 0
#define RX_PRIORITY_OFFSET3_MORE_MASK                                0x0000003f
#define RX_PRIORITY_OFFSET3_MORE_GET(x)                              (((x) & RX_PRIORITY_OFFSET3_MORE_MASK) >> RX_PRIORITY_OFFSET3_MORE_LSB)
#define RX_PRIORITY_OFFSET3_MORE_SET(x)                              (((0x0 | (x)) << RX_PRIORITY_OFFSET3_MORE_LSB) & RX_PRIORITY_OFFSET3_MORE_MASK)
#define RX_PRIORITY_OFFSET3_MORE_RESET                               0
#define RX_PRIORITY_OFFSET3_ADDRESS                                  0x0001c4
#define RX_PRIORITY_OFFSET3_HW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET3_SW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET3_RSTMASK                                  0x00000000
#define RX_PRIORITY_OFFSET3_RESET                                    0x00000000

// 0x01c8 (RX_PRIORITY_OFFSET4)
#define RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_MSB                     29
#define RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_LSB                     24
#define RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_MASK                    0x3f000000
#define RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_GET(x)                  (((x) & RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_MASK) >> RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_LSB)
#define RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_SET(x)                  (((0x0 | (x)) << RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_LSB) & RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_MASK)
#define RX_PRIORITY_OFFSET4_BEACON_RSSI_HIGH_RESET                   0
#define RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_MSB                       23
#define RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_LSB                       18
#define RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_MASK                      0x00fc0000
#define RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_GET(x)                    (((x) & RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_MASK) >> RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_LSB)
#define RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_SET(x)                    (((0x0 | (x)) << RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_LSB) & RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_MASK)
#define RX_PRIORITY_OFFSET4_MGMT_RSSI_HIGH_RESET                     0
#define RX_PRIORITY_OFFSET4_BEACON_SSID_MSB                          17
#define RX_PRIORITY_OFFSET4_BEACON_SSID_LSB                          12
#define RX_PRIORITY_OFFSET4_BEACON_SSID_MASK                         0x0003f000
#define RX_PRIORITY_OFFSET4_BEACON_SSID_GET(x)                       (((x) & RX_PRIORITY_OFFSET4_BEACON_SSID_MASK) >> RX_PRIORITY_OFFSET4_BEACON_SSID_LSB)
#define RX_PRIORITY_OFFSET4_BEACON_SSID_SET(x)                       (((0x0 | (x)) << RX_PRIORITY_OFFSET4_BEACON_SSID_LSB) & RX_PRIORITY_OFFSET4_BEACON_SSID_MASK)
#define RX_PRIORITY_OFFSET4_BEACON_SSID_RESET                        0
#define RX_PRIORITY_OFFSET4_NULLDATA_MSB                             11
#define RX_PRIORITY_OFFSET4_NULLDATA_LSB                             6
#define RX_PRIORITY_OFFSET4_NULLDATA_MASK                            0x00000fc0
#define RX_PRIORITY_OFFSET4_NULLDATA_GET(x)                          (((x) & RX_PRIORITY_OFFSET4_NULLDATA_MASK) >> RX_PRIORITY_OFFSET4_NULLDATA_LSB)
#define RX_PRIORITY_OFFSET4_NULLDATA_SET(x)                          (((0x0 | (x)) << RX_PRIORITY_OFFSET4_NULLDATA_LSB) & RX_PRIORITY_OFFSET4_NULLDATA_MASK)
#define RX_PRIORITY_OFFSET4_NULLDATA_RESET                           0
#define RX_PRIORITY_OFFSET4_PREQ_MSB                                 5
#define RX_PRIORITY_OFFSET4_PREQ_LSB                                 0
#define RX_PRIORITY_OFFSET4_PREQ_MASK                                0x0000003f
#define RX_PRIORITY_OFFSET4_PREQ_GET(x)                              (((x) & RX_PRIORITY_OFFSET4_PREQ_MASK) >> RX_PRIORITY_OFFSET4_PREQ_LSB)
#define RX_PRIORITY_OFFSET4_PREQ_SET(x)                              (((0x0 | (x)) << RX_PRIORITY_OFFSET4_PREQ_LSB) & RX_PRIORITY_OFFSET4_PREQ_MASK)
#define RX_PRIORITY_OFFSET4_PREQ_RESET                               0
#define RX_PRIORITY_OFFSET4_ADDRESS                                  0x0001c8
#define RX_PRIORITY_OFFSET4_HW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET4_SW_MASK                                  0x3fffffff
#define RX_PRIORITY_OFFSET4_RSTMASK                                  0x00000000
#define RX_PRIORITY_OFFSET4_RESET                                    0x00000000

// 0x01cc (RX_PRIORITY_OFFSET5)
#define RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_MSB                    17
#define RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_LSB                    12
#define RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_MASK                   0x0003f000
#define RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_GET(x)                 (((x) & RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_MASK) >> RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_LSB)
#define RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_SET(x)                 (((0x0 | (x)) << RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_LSB) & RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_MASK)
#define RX_PRIORITY_OFFSET5_PS_POLL_RSSI_HIGH_RESET                  0
#define RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_MSB                       11
#define RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_LSB                       6
#define RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_MASK                      0x00000fc0
#define RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_GET(x)                    (((x) & RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_MASK) >> RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_LSB)
#define RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_SET(x)                    (((0x0 | (x)) << RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_LSB) & RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_MASK)
#define RX_PRIORITY_OFFSET5_PREQ_RSSI_HIGH_RESET                     0
#define RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_MSB                       5
#define RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_LSB                       0
#define RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_MASK                      0x0000003f
#define RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_GET(x)                    (((x) & RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_MASK) >> RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_LSB)
#define RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_SET(x)                    (((0x0 | (x)) << RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_LSB) & RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_MASK)
#define RX_PRIORITY_OFFSET5_NULL_RSSI_HIGH_RESET                     0
#define RX_PRIORITY_OFFSET5_ADDRESS                                  0x0001cc
#define RX_PRIORITY_OFFSET5_HW_MASK                                  0x0003ffff
#define RX_PRIORITY_OFFSET5_SW_MASK                                  0x0003ffff
#define RX_PRIORITY_OFFSET5_RSTMASK                                  0x00000000
#define RX_PRIORITY_OFFSET5_RESET                                    0x00000000

// 0x01d0 (RXPCU_NDP_TIMEOUT)
#define RXPCU_NDP_TIMEOUT_VALUE_MSB                                  31
#define RXPCU_NDP_TIMEOUT_VALUE_LSB                                  0
#define RXPCU_NDP_TIMEOUT_VALUE_MASK                                 0xffffffff
#define RXPCU_NDP_TIMEOUT_VALUE_GET(x)                               (((x) & RXPCU_NDP_TIMEOUT_VALUE_MASK) >> RXPCU_NDP_TIMEOUT_VALUE_LSB)
#define RXPCU_NDP_TIMEOUT_VALUE_SET(x)                               (((0x0 | (x)) << RXPCU_NDP_TIMEOUT_VALUE_LSB) & RXPCU_NDP_TIMEOUT_VALUE_MASK)
#define RXPCU_NDP_TIMEOUT_VALUE_RESET                                6144
#define RXPCU_NDP_TIMEOUT_ADDRESS                                    0x0001d0
#define RXPCU_NDP_TIMEOUT_HW_MASK                                    0xffffffff
#define RXPCU_NDP_TIMEOUT_SW_MASK                                    0xffffffff
#define RXPCU_NDP_TIMEOUT_RSTMASK                                    0xffffffff
#define RXPCU_NDP_TIMEOUT_RESET                                      0x00001800


#endif /* _MAC_RXPCU_REG_H_ */
