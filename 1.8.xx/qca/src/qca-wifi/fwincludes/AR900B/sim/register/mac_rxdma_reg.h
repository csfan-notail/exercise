//                                                                             
// File:       ./headers/mac_rxdma_reg.h                                       
// Creator:    afang                                                           
// Time:       Friday Oct 5, 2012 [2:07:08 pm]                                 
//                                                                             
// Path:       /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl
// Arguments:  /cad/denali/blueprint/3.7.4//Linux-64bit/blueprint -odir        
//             ./headers -codegen ath_ch.codegen -ath_ch -Wdesc                
//             mac_rxdma_reg.rdl                                               
//                                                                             
// Sources:    /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl/mac_rxdma_reg.rdl
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


#ifndef _MAC_RXDMA_REG_H_
#define _MAC_RXDMA_REG_H_

// 0x0000 (RXDMA_CFG_RING0_0)
#define RXDMA_CFG_RING0_0_BASE_ADDR_MSB                              31
#define RXDMA_CFG_RING0_0_BASE_ADDR_LSB                              0
#define RXDMA_CFG_RING0_0_BASE_ADDR_MASK                             0xffffffff
#define RXDMA_CFG_RING0_0_BASE_ADDR_GET(x)                           (((x) & RXDMA_CFG_RING0_0_BASE_ADDR_MASK) >> RXDMA_CFG_RING0_0_BASE_ADDR_LSB)
#define RXDMA_CFG_RING0_0_BASE_ADDR_SET(x)                           (((0x0 | (x)) << RXDMA_CFG_RING0_0_BASE_ADDR_LSB) & RXDMA_CFG_RING0_0_BASE_ADDR_MASK)
#define RXDMA_CFG_RING0_0_BASE_ADDR_RESET                            0
#define RXDMA_CFG_RING0_0_ADDRESS                                    0x000000
#define RXDMA_CFG_RING0_0_HW_MASK                                    0xffffffff
#define RXDMA_CFG_RING0_0_SW_MASK                                    0xffffffff
#define RXDMA_CFG_RING0_0_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING0_0_RESET                                      0x00000000

// 0x0004 (RXDMA_CFG_RING0_1)
#define RXDMA_CFG_RING0_1_BUF_SIZE_MSB                               31
#define RXDMA_CFG_RING0_1_BUF_SIZE_LSB                               16
#define RXDMA_CFG_RING0_1_BUF_SIZE_MASK                              0xffff0000
#define RXDMA_CFG_RING0_1_BUF_SIZE_GET(x)                            (((x) & RXDMA_CFG_RING0_1_BUF_SIZE_MASK) >> RXDMA_CFG_RING0_1_BUF_SIZE_LSB)
#define RXDMA_CFG_RING0_1_BUF_SIZE_SET(x)                            (((0x0 | (x)) << RXDMA_CFG_RING0_1_BUF_SIZE_LSB) & RXDMA_CFG_RING0_1_BUF_SIZE_MASK)
#define RXDMA_CFG_RING0_1_BUF_SIZE_RESET                             0
#define RXDMA_CFG_RING0_1_NUM_DESCS_MSB                              14
#define RXDMA_CFG_RING0_1_NUM_DESCS_LSB                              0
#define RXDMA_CFG_RING0_1_NUM_DESCS_MASK                             0x00007fff
#define RXDMA_CFG_RING0_1_NUM_DESCS_GET(x)                           (((x) & RXDMA_CFG_RING0_1_NUM_DESCS_MASK) >> RXDMA_CFG_RING0_1_NUM_DESCS_LSB)
#define RXDMA_CFG_RING0_1_NUM_DESCS_SET(x)                           (((0x0 | (x)) << RXDMA_CFG_RING0_1_NUM_DESCS_LSB) & RXDMA_CFG_RING0_1_NUM_DESCS_MASK)
#define RXDMA_CFG_RING0_1_NUM_DESCS_RESET                            0
#define RXDMA_CFG_RING0_1_ADDRESS                                    0x000004
#define RXDMA_CFG_RING0_1_HW_MASK                                    0xffff7fff
#define RXDMA_CFG_RING0_1_SW_MASK                                    0xffff7fff
#define RXDMA_CFG_RING0_1_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING0_1_RESET                                      0x00000000

// 0x0008 (RXDMA_CFG_RING0_2)
#define RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_MSB                       25
#define RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_LSB                       25
#define RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_MASK                      0x02000000
#define RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_FRAGINFO_SWAP_RESET                     0
#define RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_MSB                      24
#define RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_LSB                      24
#define RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_MASK                     0x01000000
#define RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_GET(x)                   (((x) & RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_SET(x)                   (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_ATTENTION_SWAP_RESET                    0
#define RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_MSB                       23
#define RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_LSB                       23
#define RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_MASK                      0x00800000
#define RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_MSDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_MSB                     22
#define RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_LSB                     22
#define RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_MASK                    0x00400000
#define RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_MSDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_MSB                       21
#define RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_LSB                       21
#define RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_MASK                      0x00200000
#define RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_MPDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_MSB                     20
#define RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_LSB                     20
#define RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_MASK                    0x00100000
#define RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_MPDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_MSB                       19
#define RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_LSB                       19
#define RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_MASK                      0x00080000
#define RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_PPDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_MSB                     18
#define RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_LSB                     18
#define RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_MASK                    0x00040000
#define RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_PPDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING0_2_RX_PACKET_SWAP_MSB                         17
#define RXDMA_CFG_RING0_2_RX_PACKET_SWAP_LSB                         17
#define RXDMA_CFG_RING0_2_RX_PACKET_SWAP_MASK                        0x00020000
#define RXDMA_CFG_RING0_2_RX_PACKET_SWAP_GET(x)                      (((x) & RXDMA_CFG_RING0_2_RX_PACKET_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_PACKET_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_PACKET_SWAP_SET(x)                      (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_PACKET_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_PACKET_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_PACKET_SWAP_RESET                       0
#define RXDMA_CFG_RING0_2_RX_HEADER_SWAP_MSB                         16
#define RXDMA_CFG_RING0_2_RX_HEADER_SWAP_LSB                         16
#define RXDMA_CFG_RING0_2_RX_HEADER_SWAP_MASK                        0x00010000
#define RXDMA_CFG_RING0_2_RX_HEADER_SWAP_GET(x)                      (((x) & RXDMA_CFG_RING0_2_RX_HEADER_SWAP_MASK) >> RXDMA_CFG_RING0_2_RX_HEADER_SWAP_LSB)
#define RXDMA_CFG_RING0_2_RX_HEADER_SWAP_SET(x)                      (((0x0 | (x)) << RXDMA_CFG_RING0_2_RX_HEADER_SWAP_LSB) & RXDMA_CFG_RING0_2_RX_HEADER_SWAP_MASK)
#define RXDMA_CFG_RING0_2_RX_HEADER_SWAP_RESET                       0
#define RXDMA_CFG_RING0_2_INTR_LWM_THRESH_MSB                        7
#define RXDMA_CFG_RING0_2_INTR_LWM_THRESH_LSB                        0
#define RXDMA_CFG_RING0_2_INTR_LWM_THRESH_MASK                       0x000000ff
#define RXDMA_CFG_RING0_2_INTR_LWM_THRESH_GET(x)                     (((x) & RXDMA_CFG_RING0_2_INTR_LWM_THRESH_MASK) >> RXDMA_CFG_RING0_2_INTR_LWM_THRESH_LSB)
#define RXDMA_CFG_RING0_2_INTR_LWM_THRESH_SET(x)                     (((0x0 | (x)) << RXDMA_CFG_RING0_2_INTR_LWM_THRESH_LSB) & RXDMA_CFG_RING0_2_INTR_LWM_THRESH_MASK)
#define RXDMA_CFG_RING0_2_INTR_LWM_THRESH_RESET                      5
#define RXDMA_CFG_RING0_2_ADDRESS                                    0x000008
#define RXDMA_CFG_RING0_2_HW_MASK                                    0x03ff00ff
#define RXDMA_CFG_RING0_2_SW_MASK                                    0x03ff00ff
#define RXDMA_CFG_RING0_2_RSTMASK                                    0x000000ff
#define RXDMA_CFG_RING0_2_RESET                                      0x00000005

// 0x000c (RXDMA_CFG_RING0_3)
#define RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_MSB                       25
#define RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_LSB                       18
#define RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_MASK                      0x03fc0000
#define RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_GET(x)                    (((x) & RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_MASK) >> RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_LSB)
#define RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_LSB) & RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_MASK)
#define RXDMA_CFG_RING0_3_RX_PACKET_OFFSET_RESET                     0
#define RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_MSB                       9
#define RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_LSB                       2
#define RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_MASK                      0x000003fc
#define RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_GET(x)                    (((x) & RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_MASK) >> RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_LSB)
#define RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_LSB) & RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_MASK)
#define RXDMA_CFG_RING0_3_RX_HEADER_OFFSET_RESET                     0
#define RXDMA_CFG_RING0_3_ADDRESS                                    0x00000c
#define RXDMA_CFG_RING0_3_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_3_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_3_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING0_3_RESET                                      0x00000000

// 0x0010 (RXDMA_CFG_RING0_4)
#define RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_MASK) >> RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_LSB) & RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING0_4_RX_PPDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_MASK) >> RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_LSB) & RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING0_4_RX_PPDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING0_4_ADDRESS                                    0x000010
#define RXDMA_CFG_RING0_4_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_4_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_4_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING0_4_RESET                                      0x00000000

// 0x0014 (RXDMA_CFG_RING0_5)
#define RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_MASK) >> RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_LSB) & RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING0_5_RX_MPDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_MASK) >> RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_LSB) & RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING0_5_RX_MPDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING0_5_ADDRESS                                    0x000014
#define RXDMA_CFG_RING0_5_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_5_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_5_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING0_5_RESET                                      0x00000000

// 0x0018 (RXDMA_CFG_RING0_6)
#define RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_MASK) >> RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_LSB) & RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING0_6_RX_MSDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_MASK) >> RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_LSB) & RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING0_6_RX_MSDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING0_6_ADDRESS                                    0x000018
#define RXDMA_CFG_RING0_6_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_6_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_6_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING0_6_RESET                                      0x00000000

// 0x001c (RXDMA_CFG_RING0_7)
#define RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_MSB                     25
#define RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_LSB                     18
#define RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_MASK) >> RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_LSB)
#define RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_LSB) & RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_MASK)
#define RXDMA_CFG_RING0_7_RX_FRAGINFO_OFFSET_RESET                   0
#define RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_MSB                    9
#define RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_LSB                    2
#define RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_MASK                   0x000003fc
#define RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_GET(x)                 (((x) & RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_MASK) >> RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_LSB)
#define RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_SET(x)                 (((0x0 | (x)) << RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_LSB) & RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_MASK)
#define RXDMA_CFG_RING0_7_RX_ATTENTION_OFFSET_RESET                  0
#define RXDMA_CFG_RING0_7_ADDRESS                                    0x00001c
#define RXDMA_CFG_RING0_7_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_7_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING0_7_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING0_7_RESET                                      0x00000000

// 0x0020 (RXDMA_CFG_RING1_0)
#define RXDMA_CFG_RING1_0_BASE_ADDR_MSB                              31
#define RXDMA_CFG_RING1_0_BASE_ADDR_LSB                              0
#define RXDMA_CFG_RING1_0_BASE_ADDR_MASK                             0xffffffff
#define RXDMA_CFG_RING1_0_BASE_ADDR_GET(x)                           (((x) & RXDMA_CFG_RING1_0_BASE_ADDR_MASK) >> RXDMA_CFG_RING1_0_BASE_ADDR_LSB)
#define RXDMA_CFG_RING1_0_BASE_ADDR_SET(x)                           (((0x0 | (x)) << RXDMA_CFG_RING1_0_BASE_ADDR_LSB) & RXDMA_CFG_RING1_0_BASE_ADDR_MASK)
#define RXDMA_CFG_RING1_0_BASE_ADDR_RESET                            0
#define RXDMA_CFG_RING1_0_ADDRESS                                    0x000020
#define RXDMA_CFG_RING1_0_HW_MASK                                    0xffffffff
#define RXDMA_CFG_RING1_0_SW_MASK                                    0xffffffff
#define RXDMA_CFG_RING1_0_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING1_0_RESET                                      0x00000000

// 0x0024 (RXDMA_CFG_RING1_1)
#define RXDMA_CFG_RING1_1_BUF_SIZE_MSB                               31
#define RXDMA_CFG_RING1_1_BUF_SIZE_LSB                               16
#define RXDMA_CFG_RING1_1_BUF_SIZE_MASK                              0xffff0000
#define RXDMA_CFG_RING1_1_BUF_SIZE_GET(x)                            (((x) & RXDMA_CFG_RING1_1_BUF_SIZE_MASK) >> RXDMA_CFG_RING1_1_BUF_SIZE_LSB)
#define RXDMA_CFG_RING1_1_BUF_SIZE_SET(x)                            (((0x0 | (x)) << RXDMA_CFG_RING1_1_BUF_SIZE_LSB) & RXDMA_CFG_RING1_1_BUF_SIZE_MASK)
#define RXDMA_CFG_RING1_1_BUF_SIZE_RESET                             0
#define RXDMA_CFG_RING1_1_NUM_DESCS_MSB                              15
#define RXDMA_CFG_RING1_1_NUM_DESCS_LSB                              0
#define RXDMA_CFG_RING1_1_NUM_DESCS_MASK                             0x0000ffff
#define RXDMA_CFG_RING1_1_NUM_DESCS_GET(x)                           (((x) & RXDMA_CFG_RING1_1_NUM_DESCS_MASK) >> RXDMA_CFG_RING1_1_NUM_DESCS_LSB)
#define RXDMA_CFG_RING1_1_NUM_DESCS_SET(x)                           (((0x0 | (x)) << RXDMA_CFG_RING1_1_NUM_DESCS_LSB) & RXDMA_CFG_RING1_1_NUM_DESCS_MASK)
#define RXDMA_CFG_RING1_1_NUM_DESCS_RESET                            0
#define RXDMA_CFG_RING1_1_ADDRESS                                    0x000024
#define RXDMA_CFG_RING1_1_HW_MASK                                    0xffffffff
#define RXDMA_CFG_RING1_1_SW_MASK                                    0xffffffff
#define RXDMA_CFG_RING1_1_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING1_1_RESET                                      0x00000000

// 0x0028 (RXDMA_CFG_RING1_2)
#define RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_MSB                       25
#define RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_LSB                       25
#define RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_MASK                      0x02000000
#define RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_FRAGINFO_SWAP_RESET                     0
#define RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_MSB                      24
#define RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_LSB                      24
#define RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_MASK                     0x01000000
#define RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_GET(x)                   (((x) & RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_SET(x)                   (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_ATTENTION_SWAP_RESET                    0
#define RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_MSB                       23
#define RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_LSB                       23
#define RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_MASK                      0x00800000
#define RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_MSDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_MSB                     22
#define RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_LSB                     22
#define RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_MASK                    0x00400000
#define RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_MSDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_MSB                       21
#define RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_LSB                       21
#define RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_MASK                      0x00200000
#define RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_MPDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_MSB                     20
#define RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_LSB                     20
#define RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_MASK                    0x00100000
#define RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_MPDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_MSB                       19
#define RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_LSB                       19
#define RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_MASK                      0x00080000
#define RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_PPDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_MSB                     18
#define RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_LSB                     18
#define RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_MASK                    0x00040000
#define RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_PPDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING1_2_RX_PACKET_SWAP_MSB                         17
#define RXDMA_CFG_RING1_2_RX_PACKET_SWAP_LSB                         17
#define RXDMA_CFG_RING1_2_RX_PACKET_SWAP_MASK                        0x00020000
#define RXDMA_CFG_RING1_2_RX_PACKET_SWAP_GET(x)                      (((x) & RXDMA_CFG_RING1_2_RX_PACKET_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_PACKET_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_PACKET_SWAP_SET(x)                      (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_PACKET_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_PACKET_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_PACKET_SWAP_RESET                       0
#define RXDMA_CFG_RING1_2_RX_HEADER_SWAP_MSB                         16
#define RXDMA_CFG_RING1_2_RX_HEADER_SWAP_LSB                         16
#define RXDMA_CFG_RING1_2_RX_HEADER_SWAP_MASK                        0x00010000
#define RXDMA_CFG_RING1_2_RX_HEADER_SWAP_GET(x)                      (((x) & RXDMA_CFG_RING1_2_RX_HEADER_SWAP_MASK) >> RXDMA_CFG_RING1_2_RX_HEADER_SWAP_LSB)
#define RXDMA_CFG_RING1_2_RX_HEADER_SWAP_SET(x)                      (((0x0 | (x)) << RXDMA_CFG_RING1_2_RX_HEADER_SWAP_LSB) & RXDMA_CFG_RING1_2_RX_HEADER_SWAP_MASK)
#define RXDMA_CFG_RING1_2_RX_HEADER_SWAP_RESET                       0
#define RXDMA_CFG_RING1_2_INTR_LWM_THRESH_MSB                        7
#define RXDMA_CFG_RING1_2_INTR_LWM_THRESH_LSB                        0
#define RXDMA_CFG_RING1_2_INTR_LWM_THRESH_MASK                       0x000000ff
#define RXDMA_CFG_RING1_2_INTR_LWM_THRESH_GET(x)                     (((x) & RXDMA_CFG_RING1_2_INTR_LWM_THRESH_MASK) >> RXDMA_CFG_RING1_2_INTR_LWM_THRESH_LSB)
#define RXDMA_CFG_RING1_2_INTR_LWM_THRESH_SET(x)                     (((0x0 | (x)) << RXDMA_CFG_RING1_2_INTR_LWM_THRESH_LSB) & RXDMA_CFG_RING1_2_INTR_LWM_THRESH_MASK)
#define RXDMA_CFG_RING1_2_INTR_LWM_THRESH_RESET                      5
#define RXDMA_CFG_RING1_2_ADDRESS                                    0x000028
#define RXDMA_CFG_RING1_2_HW_MASK                                    0x03ff00ff
#define RXDMA_CFG_RING1_2_SW_MASK                                    0x03ff00ff
#define RXDMA_CFG_RING1_2_RSTMASK                                    0x000000ff
#define RXDMA_CFG_RING1_2_RESET                                      0x00000005

// 0x002c (RXDMA_CFG_RING1_3)
#define RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_MSB                       25
#define RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_LSB                       18
#define RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_MASK                      0x03fc0000
#define RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_GET(x)                    (((x) & RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_MASK) >> RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_LSB)
#define RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_LSB) & RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_MASK)
#define RXDMA_CFG_RING1_3_RX_PACKET_OFFSET_RESET                     0
#define RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_MSB                       9
#define RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_LSB                       2
#define RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_MASK                      0x000003fc
#define RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_GET(x)                    (((x) & RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_MASK) >> RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_LSB)
#define RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_LSB) & RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_MASK)
#define RXDMA_CFG_RING1_3_RX_HEADER_OFFSET_RESET                     0
#define RXDMA_CFG_RING1_3_ADDRESS                                    0x00002c
#define RXDMA_CFG_RING1_3_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_3_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_3_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING1_3_RESET                                      0x00000000

// 0x0030 (RXDMA_CFG_RING1_4)
#define RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_MASK) >> RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_LSB) & RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING1_4_RX_PPDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_MASK) >> RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_LSB) & RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING1_4_RX_PPDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING1_4_ADDRESS                                    0x000030
#define RXDMA_CFG_RING1_4_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_4_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_4_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING1_4_RESET                                      0x00000000

// 0x0034 (RXDMA_CFG_RING1_5)
#define RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_MASK) >> RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_LSB) & RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING1_5_RX_MPDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_MASK) >> RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_LSB) & RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING1_5_RX_MPDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING1_5_ADDRESS                                    0x000034
#define RXDMA_CFG_RING1_5_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_5_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_5_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING1_5_RESET                                      0x00000000

// 0x0038 (RXDMA_CFG_RING1_6)
#define RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_MASK) >> RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_LSB) & RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING1_6_RX_MSDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_MASK) >> RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_LSB) & RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING1_6_RX_MSDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING1_6_ADDRESS                                    0x000038
#define RXDMA_CFG_RING1_6_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_6_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_6_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING1_6_RESET                                      0x00000000

// 0x003c (RXDMA_CFG_RING1_7)
#define RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_MSB                     25
#define RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_LSB                     18
#define RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_MASK) >> RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_LSB)
#define RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_LSB) & RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_MASK)
#define RXDMA_CFG_RING1_7_RX_FRAGINFO_OFFSET_RESET                   0
#define RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_MSB                    9
#define RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_LSB                    2
#define RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_MASK                   0x000003fc
#define RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_GET(x)                 (((x) & RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_MASK) >> RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_LSB)
#define RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_SET(x)                 (((0x0 | (x)) << RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_LSB) & RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_MASK)
#define RXDMA_CFG_RING1_7_RX_ATTENTION_OFFSET_RESET                  0
#define RXDMA_CFG_RING1_7_ADDRESS                                    0x00003c
#define RXDMA_CFG_RING1_7_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_7_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING1_7_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING1_7_RESET                                      0x00000000

// 0x0040 (RXDMA_CFG_RING2_0)
#define RXDMA_CFG_RING2_0_BASE_ADDR_MSB                              31
#define RXDMA_CFG_RING2_0_BASE_ADDR_LSB                              0
#define RXDMA_CFG_RING2_0_BASE_ADDR_MASK                             0xffffffff
#define RXDMA_CFG_RING2_0_BASE_ADDR_GET(x)                           (((x) & RXDMA_CFG_RING2_0_BASE_ADDR_MASK) >> RXDMA_CFG_RING2_0_BASE_ADDR_LSB)
#define RXDMA_CFG_RING2_0_BASE_ADDR_SET(x)                           (((0x0 | (x)) << RXDMA_CFG_RING2_0_BASE_ADDR_LSB) & RXDMA_CFG_RING2_0_BASE_ADDR_MASK)
#define RXDMA_CFG_RING2_0_BASE_ADDR_RESET                            0
#define RXDMA_CFG_RING2_0_ADDRESS                                    0x000040
#define RXDMA_CFG_RING2_0_HW_MASK                                    0xffffffff
#define RXDMA_CFG_RING2_0_SW_MASK                                    0xffffffff
#define RXDMA_CFG_RING2_0_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING2_0_RESET                                      0x00000000

// 0x0044 (RXDMA_CFG_RING2_1)
#define RXDMA_CFG_RING2_1_BUF_SIZE_MSB                               31
#define RXDMA_CFG_RING2_1_BUF_SIZE_LSB                               16
#define RXDMA_CFG_RING2_1_BUF_SIZE_MASK                              0xffff0000
#define RXDMA_CFG_RING2_1_BUF_SIZE_GET(x)                            (((x) & RXDMA_CFG_RING2_1_BUF_SIZE_MASK) >> RXDMA_CFG_RING2_1_BUF_SIZE_LSB)
#define RXDMA_CFG_RING2_1_BUF_SIZE_SET(x)                            (((0x0 | (x)) << RXDMA_CFG_RING2_1_BUF_SIZE_LSB) & RXDMA_CFG_RING2_1_BUF_SIZE_MASK)
#define RXDMA_CFG_RING2_1_BUF_SIZE_RESET                             0
#define RXDMA_CFG_RING2_1_NUM_DESCS_MSB                              15
#define RXDMA_CFG_RING2_1_NUM_DESCS_LSB                              0
#define RXDMA_CFG_RING2_1_NUM_DESCS_MASK                             0x0000ffff
#define RXDMA_CFG_RING2_1_NUM_DESCS_GET(x)                           (((x) & RXDMA_CFG_RING2_1_NUM_DESCS_MASK) >> RXDMA_CFG_RING2_1_NUM_DESCS_LSB)
#define RXDMA_CFG_RING2_1_NUM_DESCS_SET(x)                           (((0x0 | (x)) << RXDMA_CFG_RING2_1_NUM_DESCS_LSB) & RXDMA_CFG_RING2_1_NUM_DESCS_MASK)
#define RXDMA_CFG_RING2_1_NUM_DESCS_RESET                            0
#define RXDMA_CFG_RING2_1_ADDRESS                                    0x000044
#define RXDMA_CFG_RING2_1_HW_MASK                                    0xffffffff
#define RXDMA_CFG_RING2_1_SW_MASK                                    0xffffffff
#define RXDMA_CFG_RING2_1_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING2_1_RESET                                      0x00000000

// 0x0048 (RXDMA_CFG_RING2_2)
#define RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_MSB                       25
#define RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_LSB                       25
#define RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_MASK                      0x02000000
#define RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_FRAGINFO_SWAP_RESET                     0
#define RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_MSB                      24
#define RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_LSB                      24
#define RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_MASK                     0x01000000
#define RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_GET(x)                   (((x) & RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_SET(x)                   (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_ATTENTION_SWAP_RESET                    0
#define RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_MSB                       23
#define RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_LSB                       23
#define RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_MASK                      0x00800000
#define RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_MSDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_MSB                     22
#define RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_LSB                     22
#define RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_MASK                    0x00400000
#define RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_MSDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_MSB                       21
#define RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_LSB                       21
#define RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_MASK                      0x00200000
#define RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_MPDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_MSB                     20
#define RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_LSB                     20
#define RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_MASK                    0x00100000
#define RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_MPDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_MSB                       19
#define RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_LSB                       19
#define RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_MASK                      0x00080000
#define RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_PPDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_MSB                     18
#define RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_LSB                     18
#define RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_MASK                    0x00040000
#define RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_PPDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING2_2_RX_PACKET_SWAP_MSB                         17
#define RXDMA_CFG_RING2_2_RX_PACKET_SWAP_LSB                         17
#define RXDMA_CFG_RING2_2_RX_PACKET_SWAP_MASK                        0x00020000
#define RXDMA_CFG_RING2_2_RX_PACKET_SWAP_GET(x)                      (((x) & RXDMA_CFG_RING2_2_RX_PACKET_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_PACKET_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_PACKET_SWAP_SET(x)                      (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_PACKET_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_PACKET_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_PACKET_SWAP_RESET                       0
#define RXDMA_CFG_RING2_2_RX_HEADER_SWAP_MSB                         16
#define RXDMA_CFG_RING2_2_RX_HEADER_SWAP_LSB                         16
#define RXDMA_CFG_RING2_2_RX_HEADER_SWAP_MASK                        0x00010000
#define RXDMA_CFG_RING2_2_RX_HEADER_SWAP_GET(x)                      (((x) & RXDMA_CFG_RING2_2_RX_HEADER_SWAP_MASK) >> RXDMA_CFG_RING2_2_RX_HEADER_SWAP_LSB)
#define RXDMA_CFG_RING2_2_RX_HEADER_SWAP_SET(x)                      (((0x0 | (x)) << RXDMA_CFG_RING2_2_RX_HEADER_SWAP_LSB) & RXDMA_CFG_RING2_2_RX_HEADER_SWAP_MASK)
#define RXDMA_CFG_RING2_2_RX_HEADER_SWAP_RESET                       0
#define RXDMA_CFG_RING2_2_INTR_LWM_THRESH_MSB                        7
#define RXDMA_CFG_RING2_2_INTR_LWM_THRESH_LSB                        0
#define RXDMA_CFG_RING2_2_INTR_LWM_THRESH_MASK                       0x000000ff
#define RXDMA_CFG_RING2_2_INTR_LWM_THRESH_GET(x)                     (((x) & RXDMA_CFG_RING2_2_INTR_LWM_THRESH_MASK) >> RXDMA_CFG_RING2_2_INTR_LWM_THRESH_LSB)
#define RXDMA_CFG_RING2_2_INTR_LWM_THRESH_SET(x)                     (((0x0 | (x)) << RXDMA_CFG_RING2_2_INTR_LWM_THRESH_LSB) & RXDMA_CFG_RING2_2_INTR_LWM_THRESH_MASK)
#define RXDMA_CFG_RING2_2_INTR_LWM_THRESH_RESET                      5
#define RXDMA_CFG_RING2_2_ADDRESS                                    0x000048
#define RXDMA_CFG_RING2_2_HW_MASK                                    0x03ff00ff
#define RXDMA_CFG_RING2_2_SW_MASK                                    0x03ff00ff
#define RXDMA_CFG_RING2_2_RSTMASK                                    0x000000ff
#define RXDMA_CFG_RING2_2_RESET                                      0x00000005

// 0x004c (RXDMA_CFG_RING2_3)
#define RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_MSB                       25
#define RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_LSB                       18
#define RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_MASK                      0x03fc0000
#define RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_GET(x)                    (((x) & RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_MASK) >> RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_LSB)
#define RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_LSB) & RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_MASK)
#define RXDMA_CFG_RING2_3_RX_PACKET_OFFSET_RESET                     0
#define RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_MSB                       9
#define RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_LSB                       2
#define RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_MASK                      0x000003fc
#define RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_GET(x)                    (((x) & RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_MASK) >> RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_LSB)
#define RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_LSB) & RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_MASK)
#define RXDMA_CFG_RING2_3_RX_HEADER_OFFSET_RESET                     0
#define RXDMA_CFG_RING2_3_ADDRESS                                    0x00004c
#define RXDMA_CFG_RING2_3_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_3_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_3_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING2_3_RESET                                      0x00000000

// 0x0050 (RXDMA_CFG_RING2_4)
#define RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_MASK) >> RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_LSB) & RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING2_4_RX_PPDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_MASK) >> RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_LSB) & RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING2_4_RX_PPDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING2_4_ADDRESS                                    0x000050
#define RXDMA_CFG_RING2_4_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_4_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_4_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING2_4_RESET                                      0x00000000

// 0x0054 (RXDMA_CFG_RING2_5)
#define RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_MASK) >> RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_LSB) & RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING2_5_RX_MPDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_MASK) >> RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_LSB) & RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING2_5_RX_MPDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING2_5_ADDRESS                                    0x000054
#define RXDMA_CFG_RING2_5_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_5_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_5_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING2_5_RESET                                      0x00000000

// 0x0058 (RXDMA_CFG_RING2_6)
#define RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_MASK) >> RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_LSB) & RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING2_6_RX_MSDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_MASK) >> RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_LSB) & RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING2_6_RX_MSDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING2_6_ADDRESS                                    0x000058
#define RXDMA_CFG_RING2_6_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_6_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_6_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING2_6_RESET                                      0x00000000

// 0x005c (RXDMA_CFG_RING2_7)
#define RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_MSB                     25
#define RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_LSB                     18
#define RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_MASK) >> RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_LSB)
#define RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_LSB) & RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_MASK)
#define RXDMA_CFG_RING2_7_RX_FRAGINFO_OFFSET_RESET                   0
#define RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_MSB                    9
#define RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_LSB                    2
#define RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_MASK                   0x000003fc
#define RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_GET(x)                 (((x) & RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_MASK) >> RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_LSB)
#define RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_SET(x)                 (((0x0 | (x)) << RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_LSB) & RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_MASK)
#define RXDMA_CFG_RING2_7_RX_ATTENTION_OFFSET_RESET                  0
#define RXDMA_CFG_RING2_7_ADDRESS                                    0x00005c
#define RXDMA_CFG_RING2_7_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_7_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING2_7_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING2_7_RESET                                      0x00000000

// 0x0060 (RXDMA_CFG_RING3_0)
#define RXDMA_CFG_RING3_0_BASE_ADDR_MSB                              31
#define RXDMA_CFG_RING3_0_BASE_ADDR_LSB                              0
#define RXDMA_CFG_RING3_0_BASE_ADDR_MASK                             0xffffffff
#define RXDMA_CFG_RING3_0_BASE_ADDR_GET(x)                           (((x) & RXDMA_CFG_RING3_0_BASE_ADDR_MASK) >> RXDMA_CFG_RING3_0_BASE_ADDR_LSB)
#define RXDMA_CFG_RING3_0_BASE_ADDR_SET(x)                           (((0x0 | (x)) << RXDMA_CFG_RING3_0_BASE_ADDR_LSB) & RXDMA_CFG_RING3_0_BASE_ADDR_MASK)
#define RXDMA_CFG_RING3_0_BASE_ADDR_RESET                            0
#define RXDMA_CFG_RING3_0_ADDRESS                                    0x000060
#define RXDMA_CFG_RING3_0_HW_MASK                                    0xffffffff
#define RXDMA_CFG_RING3_0_SW_MASK                                    0xffffffff
#define RXDMA_CFG_RING3_0_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING3_0_RESET                                      0x00000000

// 0x0064 (RXDMA_CFG_RING3_1)
#define RXDMA_CFG_RING3_1_BUF_SIZE_MSB                               31
#define RXDMA_CFG_RING3_1_BUF_SIZE_LSB                               16
#define RXDMA_CFG_RING3_1_BUF_SIZE_MASK                              0xffff0000
#define RXDMA_CFG_RING3_1_BUF_SIZE_GET(x)                            (((x) & RXDMA_CFG_RING3_1_BUF_SIZE_MASK) >> RXDMA_CFG_RING3_1_BUF_SIZE_LSB)
#define RXDMA_CFG_RING3_1_BUF_SIZE_SET(x)                            (((0x0 | (x)) << RXDMA_CFG_RING3_1_BUF_SIZE_LSB) & RXDMA_CFG_RING3_1_BUF_SIZE_MASK)
#define RXDMA_CFG_RING3_1_BUF_SIZE_RESET                             0
#define RXDMA_CFG_RING3_1_NUM_DESCS_MSB                              15
#define RXDMA_CFG_RING3_1_NUM_DESCS_LSB                              0
#define RXDMA_CFG_RING3_1_NUM_DESCS_MASK                             0x0000ffff
#define RXDMA_CFG_RING3_1_NUM_DESCS_GET(x)                           (((x) & RXDMA_CFG_RING3_1_NUM_DESCS_MASK) >> RXDMA_CFG_RING3_1_NUM_DESCS_LSB)
#define RXDMA_CFG_RING3_1_NUM_DESCS_SET(x)                           (((0x0 | (x)) << RXDMA_CFG_RING3_1_NUM_DESCS_LSB) & RXDMA_CFG_RING3_1_NUM_DESCS_MASK)
#define RXDMA_CFG_RING3_1_NUM_DESCS_RESET                            0
#define RXDMA_CFG_RING3_1_ADDRESS                                    0x000064
#define RXDMA_CFG_RING3_1_HW_MASK                                    0xffffffff
#define RXDMA_CFG_RING3_1_SW_MASK                                    0xffffffff
#define RXDMA_CFG_RING3_1_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING3_1_RESET                                      0x00000000

// 0x0068 (RXDMA_CFG_RING3_2)
#define RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_MSB                       25
#define RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_LSB                       25
#define RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_MASK                      0x02000000
#define RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_FRAGINFO_SWAP_RESET                     0
#define RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_MSB                      24
#define RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_LSB                      24
#define RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_MASK                     0x01000000
#define RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_GET(x)                   (((x) & RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_SET(x)                   (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_ATTENTION_SWAP_RESET                    0
#define RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_MSB                       23
#define RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_LSB                       23
#define RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_MASK                      0x00800000
#define RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_MSDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_MSB                     22
#define RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_LSB                     22
#define RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_MASK                    0x00400000
#define RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_MSDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_MSB                       21
#define RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_LSB                       21
#define RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_MASK                      0x00200000
#define RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_MPDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_MSB                     20
#define RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_LSB                     20
#define RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_MASK                    0x00100000
#define RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_MPDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_MSB                       19
#define RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_LSB                       19
#define RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_MASK                      0x00080000
#define RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_GET(x)                    (((x) & RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_PPDU_END_SWAP_RESET                     0
#define RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_MSB                     18
#define RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_LSB                     18
#define RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_MASK                    0x00040000
#define RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_GET(x)                  (((x) & RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_PPDU_START_SWAP_RESET                   0
#define RXDMA_CFG_RING3_2_RX_PACKET_SWAP_MSB                         17
#define RXDMA_CFG_RING3_2_RX_PACKET_SWAP_LSB                         17
#define RXDMA_CFG_RING3_2_RX_PACKET_SWAP_MASK                        0x00020000
#define RXDMA_CFG_RING3_2_RX_PACKET_SWAP_GET(x)                      (((x) & RXDMA_CFG_RING3_2_RX_PACKET_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_PACKET_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_PACKET_SWAP_SET(x)                      (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_PACKET_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_PACKET_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_PACKET_SWAP_RESET                       0
#define RXDMA_CFG_RING3_2_RX_HEADER_SWAP_MSB                         16
#define RXDMA_CFG_RING3_2_RX_HEADER_SWAP_LSB                         16
#define RXDMA_CFG_RING3_2_RX_HEADER_SWAP_MASK                        0x00010000
#define RXDMA_CFG_RING3_2_RX_HEADER_SWAP_GET(x)                      (((x) & RXDMA_CFG_RING3_2_RX_HEADER_SWAP_MASK) >> RXDMA_CFG_RING3_2_RX_HEADER_SWAP_LSB)
#define RXDMA_CFG_RING3_2_RX_HEADER_SWAP_SET(x)                      (((0x0 | (x)) << RXDMA_CFG_RING3_2_RX_HEADER_SWAP_LSB) & RXDMA_CFG_RING3_2_RX_HEADER_SWAP_MASK)
#define RXDMA_CFG_RING3_2_RX_HEADER_SWAP_RESET                       0
#define RXDMA_CFG_RING3_2_INTR_LWM_THRESH_MSB                        7
#define RXDMA_CFG_RING3_2_INTR_LWM_THRESH_LSB                        0
#define RXDMA_CFG_RING3_2_INTR_LWM_THRESH_MASK                       0x000000ff
#define RXDMA_CFG_RING3_2_INTR_LWM_THRESH_GET(x)                     (((x) & RXDMA_CFG_RING3_2_INTR_LWM_THRESH_MASK) >> RXDMA_CFG_RING3_2_INTR_LWM_THRESH_LSB)
#define RXDMA_CFG_RING3_2_INTR_LWM_THRESH_SET(x)                     (((0x0 | (x)) << RXDMA_CFG_RING3_2_INTR_LWM_THRESH_LSB) & RXDMA_CFG_RING3_2_INTR_LWM_THRESH_MASK)
#define RXDMA_CFG_RING3_2_INTR_LWM_THRESH_RESET                      5
#define RXDMA_CFG_RING3_2_ADDRESS                                    0x000068
#define RXDMA_CFG_RING3_2_HW_MASK                                    0x03ff00ff
#define RXDMA_CFG_RING3_2_SW_MASK                                    0x03ff00ff
#define RXDMA_CFG_RING3_2_RSTMASK                                    0x000000ff
#define RXDMA_CFG_RING3_2_RESET                                      0x00000005

// 0x006c (RXDMA_CFG_RING3_3)
#define RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_MSB                       25
#define RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_LSB                       18
#define RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_MASK                      0x03fc0000
#define RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_GET(x)                    (((x) & RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_MASK) >> RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_LSB)
#define RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_LSB) & RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_MASK)
#define RXDMA_CFG_RING3_3_RX_PACKET_OFFSET_RESET                     0
#define RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_MSB                       9
#define RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_LSB                       2
#define RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_MASK                      0x000003fc
#define RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_GET(x)                    (((x) & RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_MASK) >> RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_LSB)
#define RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_SET(x)                    (((0x0 | (x)) << RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_LSB) & RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_MASK)
#define RXDMA_CFG_RING3_3_RX_HEADER_OFFSET_RESET                     0
#define RXDMA_CFG_RING3_3_ADDRESS                                    0x00006c
#define RXDMA_CFG_RING3_3_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_3_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_3_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING3_3_RESET                                      0x00000000

// 0x0070 (RXDMA_CFG_RING3_4)
#define RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_MASK) >> RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_LSB) & RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING3_4_RX_PPDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_MASK) >> RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_LSB) & RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING3_4_RX_PPDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING3_4_ADDRESS                                    0x000070
#define RXDMA_CFG_RING3_4_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_4_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_4_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING3_4_RESET                                      0x00000000

// 0x0074 (RXDMA_CFG_RING3_5)
#define RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_MASK) >> RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_LSB) & RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING3_5_RX_MPDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_MASK) >> RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_LSB) & RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING3_5_RX_MPDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING3_5_ADDRESS                                    0x000074
#define RXDMA_CFG_RING3_5_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_5_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_5_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING3_5_RESET                                      0x00000000

// 0x0078 (RXDMA_CFG_RING3_6)
#define RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_MSB                     25
#define RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_LSB                     18
#define RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_MASK) >> RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_LSB)
#define RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_LSB) & RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_MASK)
#define RXDMA_CFG_RING3_6_RX_MSDU_END_OFFSET_RESET                   0
#define RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_MSB                   9
#define RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_LSB                   2
#define RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_MASK                  0x000003fc
#define RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_GET(x)                (((x) & RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_MASK) >> RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_LSB)
#define RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_SET(x)                (((0x0 | (x)) << RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_LSB) & RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_MASK)
#define RXDMA_CFG_RING3_6_RX_MSDU_START_OFFSET_RESET                 0
#define RXDMA_CFG_RING3_6_ADDRESS                                    0x000078
#define RXDMA_CFG_RING3_6_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_6_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_6_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING3_6_RESET                                      0x00000000

// 0x007c (RXDMA_CFG_RING3_7)
#define RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_MSB                     25
#define RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_LSB                     18
#define RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_MASK                    0x03fc0000
#define RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_GET(x)                  (((x) & RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_MASK) >> RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_LSB)
#define RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_SET(x)                  (((0x0 | (x)) << RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_LSB) & RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_MASK)
#define RXDMA_CFG_RING3_7_RX_FRAGINFO_OFFSET_RESET                   0
#define RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_MSB                    9
#define RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_LSB                    2
#define RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_MASK                   0x000003fc
#define RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_GET(x)                 (((x) & RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_MASK) >> RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_LSB)
#define RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_SET(x)                 (((0x0 | (x)) << RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_LSB) & RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_MASK)
#define RXDMA_CFG_RING3_7_RX_ATTENTION_OFFSET_RESET                  0
#define RXDMA_CFG_RING3_7_ADDRESS                                    0x00007c
#define RXDMA_CFG_RING3_7_HW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_7_SW_MASK                                    0x03fc03fc
#define RXDMA_CFG_RING3_7_RSTMASK                                    0x00000000
#define RXDMA_CFG_RING3_7_RESET                                      0x00000000

// 0x0080 (RXDMA_EN_ACT_0)
#define RXDMA_EN_ACT_0_RING_ACTIVE_MSB                               1
#define RXDMA_EN_ACT_0_RING_ACTIVE_LSB                               1
#define RXDMA_EN_ACT_0_RING_ACTIVE_MASK                              0x00000002
#define RXDMA_EN_ACT_0_RING_ACTIVE_GET(x)                            (((x) & RXDMA_EN_ACT_0_RING_ACTIVE_MASK) >> RXDMA_EN_ACT_0_RING_ACTIVE_LSB)
#define RXDMA_EN_ACT_0_RING_ACTIVE_SET(x)                            (((0x0 | (x)) << RXDMA_EN_ACT_0_RING_ACTIVE_LSB) & RXDMA_EN_ACT_0_RING_ACTIVE_MASK)
#define RXDMA_EN_ACT_0_RING_ACTIVE_RESET                             0
#define RXDMA_EN_ACT_0_RING_ENABLE_MSB                               0
#define RXDMA_EN_ACT_0_RING_ENABLE_LSB                               0
#define RXDMA_EN_ACT_0_RING_ENABLE_MASK                              0x00000001
#define RXDMA_EN_ACT_0_RING_ENABLE_GET(x)                            (((x) & RXDMA_EN_ACT_0_RING_ENABLE_MASK) >> RXDMA_EN_ACT_0_RING_ENABLE_LSB)
#define RXDMA_EN_ACT_0_RING_ENABLE_SET(x)                            (((0x0 | (x)) << RXDMA_EN_ACT_0_RING_ENABLE_LSB) & RXDMA_EN_ACT_0_RING_ENABLE_MASK)
#define RXDMA_EN_ACT_0_RING_ENABLE_RESET                             0
#define RXDMA_EN_ACT_0_ADDRESS                                       0x000080
#define RXDMA_EN_ACT_0_HW_MASK                                       0x00000003
#define RXDMA_EN_ACT_0_SW_MASK                                       0x00000003
#define RXDMA_EN_ACT_0_RSTMASK                                       0x00000000
#define RXDMA_EN_ACT_0_RESET                                         0x00000000

// 0x0084 (RXDMA_EN_ACT_1)
#define RXDMA_EN_ACT_1_RING_ACTIVE_MSB                               1
#define RXDMA_EN_ACT_1_RING_ACTIVE_LSB                               1
#define RXDMA_EN_ACT_1_RING_ACTIVE_MASK                              0x00000002
#define RXDMA_EN_ACT_1_RING_ACTIVE_GET(x)                            (((x) & RXDMA_EN_ACT_1_RING_ACTIVE_MASK) >> RXDMA_EN_ACT_1_RING_ACTIVE_LSB)
#define RXDMA_EN_ACT_1_RING_ACTIVE_SET(x)                            (((0x0 | (x)) << RXDMA_EN_ACT_1_RING_ACTIVE_LSB) & RXDMA_EN_ACT_1_RING_ACTIVE_MASK)
#define RXDMA_EN_ACT_1_RING_ACTIVE_RESET                             0
#define RXDMA_EN_ACT_1_RING_ENABLE_MSB                               0
#define RXDMA_EN_ACT_1_RING_ENABLE_LSB                               0
#define RXDMA_EN_ACT_1_RING_ENABLE_MASK                              0x00000001
#define RXDMA_EN_ACT_1_RING_ENABLE_GET(x)                            (((x) & RXDMA_EN_ACT_1_RING_ENABLE_MASK) >> RXDMA_EN_ACT_1_RING_ENABLE_LSB)
#define RXDMA_EN_ACT_1_RING_ENABLE_SET(x)                            (((0x0 | (x)) << RXDMA_EN_ACT_1_RING_ENABLE_LSB) & RXDMA_EN_ACT_1_RING_ENABLE_MASK)
#define RXDMA_EN_ACT_1_RING_ENABLE_RESET                             0
#define RXDMA_EN_ACT_1_ADDRESS                                       0x000084
#define RXDMA_EN_ACT_1_HW_MASK                                       0x00000003
#define RXDMA_EN_ACT_1_SW_MASK                                       0x00000003
#define RXDMA_EN_ACT_1_RSTMASK                                       0x00000000
#define RXDMA_EN_ACT_1_RESET                                         0x00000000

// 0x0088 (RXDMA_EN_ACT_2)
#define RXDMA_EN_ACT_2_RING_ACTIVE_MSB                               1
#define RXDMA_EN_ACT_2_RING_ACTIVE_LSB                               1
#define RXDMA_EN_ACT_2_RING_ACTIVE_MASK                              0x00000002
#define RXDMA_EN_ACT_2_RING_ACTIVE_GET(x)                            (((x) & RXDMA_EN_ACT_2_RING_ACTIVE_MASK) >> RXDMA_EN_ACT_2_RING_ACTIVE_LSB)
#define RXDMA_EN_ACT_2_RING_ACTIVE_SET(x)                            (((0x0 | (x)) << RXDMA_EN_ACT_2_RING_ACTIVE_LSB) & RXDMA_EN_ACT_2_RING_ACTIVE_MASK)
#define RXDMA_EN_ACT_2_RING_ACTIVE_RESET                             0
#define RXDMA_EN_ACT_2_RING_ENABLE_MSB                               0
#define RXDMA_EN_ACT_2_RING_ENABLE_LSB                               0
#define RXDMA_EN_ACT_2_RING_ENABLE_MASK                              0x00000001
#define RXDMA_EN_ACT_2_RING_ENABLE_GET(x)                            (((x) & RXDMA_EN_ACT_2_RING_ENABLE_MASK) >> RXDMA_EN_ACT_2_RING_ENABLE_LSB)
#define RXDMA_EN_ACT_2_RING_ENABLE_SET(x)                            (((0x0 | (x)) << RXDMA_EN_ACT_2_RING_ENABLE_LSB) & RXDMA_EN_ACT_2_RING_ENABLE_MASK)
#define RXDMA_EN_ACT_2_RING_ENABLE_RESET                             0
#define RXDMA_EN_ACT_2_ADDRESS                                       0x000088
#define RXDMA_EN_ACT_2_HW_MASK                                       0x00000003
#define RXDMA_EN_ACT_2_SW_MASK                                       0x00000003
#define RXDMA_EN_ACT_2_RSTMASK                                       0x00000000
#define RXDMA_EN_ACT_2_RESET                                         0x00000000

// 0x008c (RXDMA_EN_ACT_3)
#define RXDMA_EN_ACT_3_RING_ACTIVE_MSB                               1
#define RXDMA_EN_ACT_3_RING_ACTIVE_LSB                               1
#define RXDMA_EN_ACT_3_RING_ACTIVE_MASK                              0x00000002
#define RXDMA_EN_ACT_3_RING_ACTIVE_GET(x)                            (((x) & RXDMA_EN_ACT_3_RING_ACTIVE_MASK) >> RXDMA_EN_ACT_3_RING_ACTIVE_LSB)
#define RXDMA_EN_ACT_3_RING_ACTIVE_SET(x)                            (((0x0 | (x)) << RXDMA_EN_ACT_3_RING_ACTIVE_LSB) & RXDMA_EN_ACT_3_RING_ACTIVE_MASK)
#define RXDMA_EN_ACT_3_RING_ACTIVE_RESET                             0
#define RXDMA_EN_ACT_3_RING_ENABLE_MSB                               0
#define RXDMA_EN_ACT_3_RING_ENABLE_LSB                               0
#define RXDMA_EN_ACT_3_RING_ENABLE_MASK                              0x00000001
#define RXDMA_EN_ACT_3_RING_ENABLE_GET(x)                            (((x) & RXDMA_EN_ACT_3_RING_ENABLE_MASK) >> RXDMA_EN_ACT_3_RING_ENABLE_LSB)
#define RXDMA_EN_ACT_3_RING_ENABLE_SET(x)                            (((0x0 | (x)) << RXDMA_EN_ACT_3_RING_ENABLE_LSB) & RXDMA_EN_ACT_3_RING_ENABLE_MASK)
#define RXDMA_EN_ACT_3_RING_ENABLE_RESET                             0
#define RXDMA_EN_ACT_3_ADDRESS                                       0x00008c
#define RXDMA_EN_ACT_3_HW_MASK                                       0x00000003
#define RXDMA_EN_ACT_3_SW_MASK                                       0x00000003
#define RXDMA_EN_ACT_3_RSTMASK                                       0x00000000
#define RXDMA_EN_ACT_3_RESET                                         0x00000000

// 0x0090 (RXDMA_IDX_0)
#define RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_MSB                        31
#define RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_LSB                        31
#define RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_MASK                       0x80000000
#define RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_GET(x)                     (((x) & RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_MASK) >> RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_LSB)
#define RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_SET(x)                     (((0x0 | (x)) << RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_LSB) & RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_MASK)
#define RXDMA_IDX_0_RING_HW_IDX_OVERWRITE_RESET                      0
#define RXDMA_IDX_0_RING_HW_IDX_MSB                                  30
#define RXDMA_IDX_0_RING_HW_IDX_LSB                                  16
#define RXDMA_IDX_0_RING_HW_IDX_MASK                                 0x7fff0000
#define RXDMA_IDX_0_RING_HW_IDX_GET(x)                               (((x) & RXDMA_IDX_0_RING_HW_IDX_MASK) >> RXDMA_IDX_0_RING_HW_IDX_LSB)
#define RXDMA_IDX_0_RING_HW_IDX_SET(x)                               (((0x0 | (x)) << RXDMA_IDX_0_RING_HW_IDX_LSB) & RXDMA_IDX_0_RING_HW_IDX_MASK)
#define RXDMA_IDX_0_RING_HW_IDX_RESET                                0
#define RXDMA_IDX_0_RING_FW_IDX_MSB                                  14
#define RXDMA_IDX_0_RING_FW_IDX_LSB                                  0
#define RXDMA_IDX_0_RING_FW_IDX_MASK                                 0x00007fff
#define RXDMA_IDX_0_RING_FW_IDX_GET(x)                               (((x) & RXDMA_IDX_0_RING_FW_IDX_MASK) >> RXDMA_IDX_0_RING_FW_IDX_LSB)
#define RXDMA_IDX_0_RING_FW_IDX_SET(x)                               (((0x0 | (x)) << RXDMA_IDX_0_RING_FW_IDX_LSB) & RXDMA_IDX_0_RING_FW_IDX_MASK)
#define RXDMA_IDX_0_RING_FW_IDX_RESET                                0
#define RXDMA_IDX_0_ADDRESS                                          0x000090
#define RXDMA_IDX_0_HW_MASK                                          0xffff7fff
#define RXDMA_IDX_0_SW_MASK                                          0xffff7fff
#define RXDMA_IDX_0_RSTMASK                                          0x00000000
#define RXDMA_IDX_0_RESET                                            0x00000000

// 0x0094 (RXDMA_IDX_1)
#define RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_MSB                        31
#define RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_LSB                        31
#define RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_MASK                       0x80000000
#define RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_GET(x)                     (((x) & RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_MASK) >> RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_LSB)
#define RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_SET(x)                     (((0x0 | (x)) << RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_LSB) & RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_MASK)
#define RXDMA_IDX_1_RING_HW_IDX_OVERWRITE_RESET                      0
#define RXDMA_IDX_1_RING_HW_IDX_MSB                                  30
#define RXDMA_IDX_1_RING_HW_IDX_LSB                                  16
#define RXDMA_IDX_1_RING_HW_IDX_MASK                                 0x7fff0000
#define RXDMA_IDX_1_RING_HW_IDX_GET(x)                               (((x) & RXDMA_IDX_1_RING_HW_IDX_MASK) >> RXDMA_IDX_1_RING_HW_IDX_LSB)
#define RXDMA_IDX_1_RING_HW_IDX_SET(x)                               (((0x0 | (x)) << RXDMA_IDX_1_RING_HW_IDX_LSB) & RXDMA_IDX_1_RING_HW_IDX_MASK)
#define RXDMA_IDX_1_RING_HW_IDX_RESET                                0
#define RXDMA_IDX_1_RING_FW_IDX_MSB                                  14
#define RXDMA_IDX_1_RING_FW_IDX_LSB                                  0
#define RXDMA_IDX_1_RING_FW_IDX_MASK                                 0x00007fff
#define RXDMA_IDX_1_RING_FW_IDX_GET(x)                               (((x) & RXDMA_IDX_1_RING_FW_IDX_MASK) >> RXDMA_IDX_1_RING_FW_IDX_LSB)
#define RXDMA_IDX_1_RING_FW_IDX_SET(x)                               (((0x0 | (x)) << RXDMA_IDX_1_RING_FW_IDX_LSB) & RXDMA_IDX_1_RING_FW_IDX_MASK)
#define RXDMA_IDX_1_RING_FW_IDX_RESET                                0
#define RXDMA_IDX_1_ADDRESS                                          0x000094
#define RXDMA_IDX_1_HW_MASK                                          0xffff7fff
#define RXDMA_IDX_1_SW_MASK                                          0xffff7fff
#define RXDMA_IDX_1_RSTMASK                                          0x00000000
#define RXDMA_IDX_1_RESET                                            0x00000000

// 0x0098 (RXDMA_IDX_2)
#define RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_MSB                        31
#define RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_LSB                        31
#define RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_MASK                       0x80000000
#define RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_GET(x)                     (((x) & RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_MASK) >> RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_LSB)
#define RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_SET(x)                     (((0x0 | (x)) << RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_LSB) & RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_MASK)
#define RXDMA_IDX_2_RING_HW_IDX_OVERWRITE_RESET                      0
#define RXDMA_IDX_2_RING_HW_IDX_MSB                                  30
#define RXDMA_IDX_2_RING_HW_IDX_LSB                                  16
#define RXDMA_IDX_2_RING_HW_IDX_MASK                                 0x7fff0000
#define RXDMA_IDX_2_RING_HW_IDX_GET(x)                               (((x) & RXDMA_IDX_2_RING_HW_IDX_MASK) >> RXDMA_IDX_2_RING_HW_IDX_LSB)
#define RXDMA_IDX_2_RING_HW_IDX_SET(x)                               (((0x0 | (x)) << RXDMA_IDX_2_RING_HW_IDX_LSB) & RXDMA_IDX_2_RING_HW_IDX_MASK)
#define RXDMA_IDX_2_RING_HW_IDX_RESET                                0
#define RXDMA_IDX_2_RING_FW_IDX_MSB                                  14
#define RXDMA_IDX_2_RING_FW_IDX_LSB                                  0
#define RXDMA_IDX_2_RING_FW_IDX_MASK                                 0x00007fff
#define RXDMA_IDX_2_RING_FW_IDX_GET(x)                               (((x) & RXDMA_IDX_2_RING_FW_IDX_MASK) >> RXDMA_IDX_2_RING_FW_IDX_LSB)
#define RXDMA_IDX_2_RING_FW_IDX_SET(x)                               (((0x0 | (x)) << RXDMA_IDX_2_RING_FW_IDX_LSB) & RXDMA_IDX_2_RING_FW_IDX_MASK)
#define RXDMA_IDX_2_RING_FW_IDX_RESET                                0
#define RXDMA_IDX_2_ADDRESS                                          0x000098
#define RXDMA_IDX_2_HW_MASK                                          0xffff7fff
#define RXDMA_IDX_2_SW_MASK                                          0xffff7fff
#define RXDMA_IDX_2_RSTMASK                                          0x00000000
#define RXDMA_IDX_2_RESET                                            0x00000000

// 0x009c (RXDMA_IDX_3)
#define RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_MSB                        31
#define RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_LSB                        31
#define RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_MASK                       0x80000000
#define RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_GET(x)                     (((x) & RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_MASK) >> RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_LSB)
#define RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_SET(x)                     (((0x0 | (x)) << RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_LSB) & RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_MASK)
#define RXDMA_IDX_3_RING_HW_IDX_OVERWRITE_RESET                      0
#define RXDMA_IDX_3_RING_HW_IDX_MSB                                  30
#define RXDMA_IDX_3_RING_HW_IDX_LSB                                  16
#define RXDMA_IDX_3_RING_HW_IDX_MASK                                 0x7fff0000
#define RXDMA_IDX_3_RING_HW_IDX_GET(x)                               (((x) & RXDMA_IDX_3_RING_HW_IDX_MASK) >> RXDMA_IDX_3_RING_HW_IDX_LSB)
#define RXDMA_IDX_3_RING_HW_IDX_SET(x)                               (((0x0 | (x)) << RXDMA_IDX_3_RING_HW_IDX_LSB) & RXDMA_IDX_3_RING_HW_IDX_MASK)
#define RXDMA_IDX_3_RING_HW_IDX_RESET                                0
#define RXDMA_IDX_3_RING_FW_IDX_MSB                                  14
#define RXDMA_IDX_3_RING_FW_IDX_LSB                                  0
#define RXDMA_IDX_3_RING_FW_IDX_MASK                                 0x00007fff
#define RXDMA_IDX_3_RING_FW_IDX_GET(x)                               (((x) & RXDMA_IDX_3_RING_FW_IDX_MASK) >> RXDMA_IDX_3_RING_FW_IDX_LSB)
#define RXDMA_IDX_3_RING_FW_IDX_SET(x)                               (((0x0 | (x)) << RXDMA_IDX_3_RING_FW_IDX_LSB) & RXDMA_IDX_3_RING_FW_IDX_MASK)
#define RXDMA_IDX_3_RING_FW_IDX_RESET                                0
#define RXDMA_IDX_3_ADDRESS                                          0x00009c
#define RXDMA_IDX_3_HW_MASK                                          0xffff7fff
#define RXDMA_IDX_3_SW_MASK                                          0xffff7fff
#define RXDMA_IDX_3_RSTMASK                                          0x00000000
#define RXDMA_IDX_3_RESET                                            0x00000000

// 0x00a0 (RXDMA_CFG_MISC_0)
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_MSB                    31
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_LSB                    16
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_MASK                   0xffff0000
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_GET(x)                 (((x) & RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_MASK) >> RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_LSB)
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_SET(x)                 (((0x0 | (x)) << RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_LSB) & RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_MASK)
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_1_RESET                  128
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_MSB                    15
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_LSB                    0
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_MASK                   0x0000ffff
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_GET(x)                 (((x) & RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_MASK) >> RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_LSB)
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_SET(x)                 (((0x0 | (x)) << RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_LSB) & RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_MASK)
#define RXDMA_CFG_MISC_0_MEM_BOUNDARY_BYTES_0_RESET                  128
#define RXDMA_CFG_MISC_0_ADDRESS                                     0x0000a0
#define RXDMA_CFG_MISC_0_HW_MASK                                     0xffffffff
#define RXDMA_CFG_MISC_0_SW_MASK                                     0xffffffff
#define RXDMA_CFG_MISC_0_RSTMASK                                     0xffffffff
#define RXDMA_CFG_MISC_0_RESET                                       0x00800080

// 0x00a4 (RXDMA_CFG_MISC_1)
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_MSB                    31
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_LSB                    16
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_MASK                   0xffff0000
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_GET(x)                 (((x) & RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_MASK) >> RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_LSB)
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_SET(x)                 (((0x0 | (x)) << RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_LSB) & RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_MASK)
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_3_RESET                  128
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_MSB                    15
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_LSB                    0
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_MASK                   0x0000ffff
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_GET(x)                 (((x) & RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_MASK) >> RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_LSB)
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_SET(x)                 (((0x0 | (x)) << RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_LSB) & RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_MASK)
#define RXDMA_CFG_MISC_1_MEM_BOUNDARY_BYTES_2_RESET                  128
#define RXDMA_CFG_MISC_1_ADDRESS                                     0x0000a4
#define RXDMA_CFG_MISC_1_HW_MASK                                     0xffffffff
#define RXDMA_CFG_MISC_1_SW_MASK                                     0xffffffff
#define RXDMA_CFG_MISC_1_RSTMASK                                     0xffffffff
#define RXDMA_CFG_MISC_1_RESET                                       0x00800080

// 0x00a8 (RXDMA_CFG_MISC_2)
#define RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_MSB                  0
#define RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_LSB                  0
#define RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_MASK                 0x00000001
#define RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_GET(x)               (((x) & RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_MASK) >> RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_LSB)
#define RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_SET(x)               (((0x0 | (x)) << RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_LSB) & RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_MASK)
#define RXDMA_CFG_MISC_2_LONG_DMA_PAUSE_FIX_DIS_RESET                0
#define RXDMA_CFG_MISC_2_ADDRESS                                     0x0000a8
#define RXDMA_CFG_MISC_2_HW_MASK                                     0x00000001
#define RXDMA_CFG_MISC_2_SW_MASK                                     0x00000001
#define RXDMA_CFG_MISC_2_RSTMASK                                     0x00000000
#define RXDMA_CFG_MISC_2_RESET                                       0x00000000

// 0x00ac (RXDMA_FETCH_IDX_0)
#define RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_MSB                      14
#define RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_LSB                      0
#define RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_MASK                     0x00007fff
#define RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_GET(x)                   (((x) & RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_MASK) >> RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_LSB)
#define RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_SET(x)                   (((0x0 | (x)) << RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_LSB) & RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_MASK)
#define RXDMA_FETCH_IDX_0_RING_HW_FETCH_IDX_RESET                    0
#define RXDMA_FETCH_IDX_0_ADDRESS                                    0x0000ac
#define RXDMA_FETCH_IDX_0_HW_MASK                                    0x00007fff
#define RXDMA_FETCH_IDX_0_SW_MASK                                    0x00007fff
#define RXDMA_FETCH_IDX_0_RSTMASK                                    0x00000000
#define RXDMA_FETCH_IDX_0_RESET                                      0x00000000

// 0x00b0 (RXDMA_FETCH_IDX_1)
#define RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_MSB                      14
#define RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_LSB                      0
#define RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_MASK                     0x00007fff
#define RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_GET(x)                   (((x) & RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_MASK) >> RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_LSB)
#define RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_SET(x)                   (((0x0 | (x)) << RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_LSB) & RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_MASK)
#define RXDMA_FETCH_IDX_1_RING_HW_FETCH_IDX_RESET                    0
#define RXDMA_FETCH_IDX_1_ADDRESS                                    0x0000b0
#define RXDMA_FETCH_IDX_1_HW_MASK                                    0x00007fff
#define RXDMA_FETCH_IDX_1_SW_MASK                                    0x00007fff
#define RXDMA_FETCH_IDX_1_RSTMASK                                    0x00000000
#define RXDMA_FETCH_IDX_1_RESET                                      0x00000000

// 0x00b4 (RXDMA_FETCH_IDX_2)
#define RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_MSB                      14
#define RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_LSB                      0
#define RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_MASK                     0x00007fff
#define RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_GET(x)                   (((x) & RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_MASK) >> RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_LSB)
#define RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_SET(x)                   (((0x0 | (x)) << RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_LSB) & RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_MASK)
#define RXDMA_FETCH_IDX_2_RING_HW_FETCH_IDX_RESET                    0
#define RXDMA_FETCH_IDX_2_ADDRESS                                    0x0000b4
#define RXDMA_FETCH_IDX_2_HW_MASK                                    0x00007fff
#define RXDMA_FETCH_IDX_2_SW_MASK                                    0x00007fff
#define RXDMA_FETCH_IDX_2_RSTMASK                                    0x00000000
#define RXDMA_FETCH_IDX_2_RESET                                      0x00000000

// 0x00b8 (RXDMA_FETCH_IDX_3)
#define RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_MSB                      14
#define RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_LSB                      0
#define RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_MASK                     0x00007fff
#define RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_GET(x)                   (((x) & RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_MASK) >> RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_LSB)
#define RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_SET(x)                   (((0x0 | (x)) << RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_LSB) & RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_MASK)
#define RXDMA_FETCH_IDX_3_RING_HW_FETCH_IDX_RESET                    0
#define RXDMA_FETCH_IDX_3_ADDRESS                                    0x0000b8
#define RXDMA_FETCH_IDX_3_HW_MASK                                    0x00007fff
#define RXDMA_FETCH_IDX_3_SW_MASK                                    0x00007fff
#define RXDMA_FETCH_IDX_3_RSTMASK                                    0x00000000
#define RXDMA_FETCH_IDX_3_RESET                                      0x00000000

// 0x00bc (RXDMA_GLOBAL_RER)
#define RXDMA_GLOBAL_RER_ENABLE_MSB                                  0
#define RXDMA_GLOBAL_RER_ENABLE_LSB                                  0
#define RXDMA_GLOBAL_RER_ENABLE_MASK                                 0x00000001
#define RXDMA_GLOBAL_RER_ENABLE_GET(x)                               (((x) & RXDMA_GLOBAL_RER_ENABLE_MASK) >> RXDMA_GLOBAL_RER_ENABLE_LSB)
#define RXDMA_GLOBAL_RER_ENABLE_SET(x)                               (((0x0 | (x)) << RXDMA_GLOBAL_RER_ENABLE_LSB) & RXDMA_GLOBAL_RER_ENABLE_MASK)
#define RXDMA_GLOBAL_RER_ENABLE_RESET                                1
#define RXDMA_GLOBAL_RER_ADDRESS                                     0x0000bc
#define RXDMA_GLOBAL_RER_HW_MASK                                     0x00000001
#define RXDMA_GLOBAL_RER_SW_MASK                                     0x00000001
#define RXDMA_GLOBAL_RER_RSTMASK                                     0x00000001
#define RXDMA_GLOBAL_RER_RESET                                       0x00000001

// 0x00c0 (RXDMA_SOFT_RST)
#define RXDMA_SOFT_RST_SET_MSB                                       0
#define RXDMA_SOFT_RST_SET_LSB                                       0
#define RXDMA_SOFT_RST_SET_MASK                                      0x00000001
#define RXDMA_SOFT_RST_SET_GET(x)                                    (((x) & RXDMA_SOFT_RST_SET_MASK) >> RXDMA_SOFT_RST_SET_LSB)
#define RXDMA_SOFT_RST_SET_SET(x)                                    (((0x0 | (x)) << RXDMA_SOFT_RST_SET_LSB) & RXDMA_SOFT_RST_SET_MASK)
#define RXDMA_SOFT_RST_SET_RESET                                     1
#define RXDMA_SOFT_RST_ADDRESS                                       0x0000c0
#define RXDMA_SOFT_RST_HW_MASK                                       0x00000001
#define RXDMA_SOFT_RST_SW_MASK                                       0x00000001
#define RXDMA_SOFT_RST_RSTMASK                                       0x00000001
#define RXDMA_SOFT_RST_RESET                                         0x00000001

// 0x00c4 (RXDMA_WDOG_LIMIT)
#define RXDMA_WDOG_LIMIT_VAL_MSB                                     31
#define RXDMA_WDOG_LIMIT_VAL_LSB                                     0
#define RXDMA_WDOG_LIMIT_VAL_MASK                                    0xffffffff
#define RXDMA_WDOG_LIMIT_VAL_GET(x)                                  (((x) & RXDMA_WDOG_LIMIT_VAL_MASK) >> RXDMA_WDOG_LIMIT_VAL_LSB)
#define RXDMA_WDOG_LIMIT_VAL_SET(x)                                  (((0x0 | (x)) << RXDMA_WDOG_LIMIT_VAL_LSB) & RXDMA_WDOG_LIMIT_VAL_MASK)
#define RXDMA_WDOG_LIMIT_VAL_RESET                                   0
#define RXDMA_WDOG_LIMIT_ADDRESS                                     0x0000c4
#define RXDMA_WDOG_LIMIT_HW_MASK                                     0xffffffff
#define RXDMA_WDOG_LIMIT_SW_MASK                                     0xffffffff
#define RXDMA_WDOG_LIMIT_RSTMASK                                     0x00000000
#define RXDMA_WDOG_LIMIT_RESET                                       0x00000000

// 0x00c8 (RXDMA_TESTBUS_SAMPLE)
#define RXDMA_TESTBUS_SAMPLE_VAL_MSB                                 31
#define RXDMA_TESTBUS_SAMPLE_VAL_LSB                                 0
#define RXDMA_TESTBUS_SAMPLE_VAL_MASK                                0xffffffff
#define RXDMA_TESTBUS_SAMPLE_VAL_GET(x)                              (((x) & RXDMA_TESTBUS_SAMPLE_VAL_MASK) >> RXDMA_TESTBUS_SAMPLE_VAL_LSB)
#define RXDMA_TESTBUS_SAMPLE_VAL_SET(x)                              (((0x0 | (x)) << RXDMA_TESTBUS_SAMPLE_VAL_LSB) & RXDMA_TESTBUS_SAMPLE_VAL_MASK)
#define RXDMA_TESTBUS_SAMPLE_VAL_RESET                               0
#define RXDMA_TESTBUS_SAMPLE_ADDRESS                                 0x0000c8
#define RXDMA_TESTBUS_SAMPLE_HW_MASK                                 0xffffffff
#define RXDMA_TESTBUS_SAMPLE_SW_MASK                                 0xffffffff
#define RXDMA_TESTBUS_SAMPLE_RSTMASK                                 0x00000000
#define RXDMA_TESTBUS_SAMPLE_RESET                                   0x00000000

// 0x00cc (RXDMA_DEBUG)
#define RXDMA_DEBUG_RING_FETCH_FSM_MSB                               8
#define RXDMA_DEBUG_RING_FETCH_FSM_LSB                               5
#define RXDMA_DEBUG_RING_FETCH_FSM_MASK                              0x000001e0
#define RXDMA_DEBUG_RING_FETCH_FSM_GET(x)                            (((x) & RXDMA_DEBUG_RING_FETCH_FSM_MASK) >> RXDMA_DEBUG_RING_FETCH_FSM_LSB)
#define RXDMA_DEBUG_RING_FETCH_FSM_SET(x)                            (((0x0 | (x)) << RXDMA_DEBUG_RING_FETCH_FSM_LSB) & RXDMA_DEBUG_RING_FETCH_FSM_MASK)
#define RXDMA_DEBUG_RING_FETCH_FSM_RESET                             0
#define RXDMA_DEBUG_CTL_FSM_MSB                                      4
#define RXDMA_DEBUG_CTL_FSM_LSB                                      0
#define RXDMA_DEBUG_CTL_FSM_MASK                                     0x0000001f
#define RXDMA_DEBUG_CTL_FSM_GET(x)                                   (((x) & RXDMA_DEBUG_CTL_FSM_MASK) >> RXDMA_DEBUG_CTL_FSM_LSB)
#define RXDMA_DEBUG_CTL_FSM_SET(x)                                   (((0x0 | (x)) << RXDMA_DEBUG_CTL_FSM_LSB) & RXDMA_DEBUG_CTL_FSM_MASK)
#define RXDMA_DEBUG_CTL_FSM_RESET                                    0
#define RXDMA_DEBUG_ADDRESS                                          0x0000cc
#define RXDMA_DEBUG_HW_MASK                                          0x000001ff
#define RXDMA_DEBUG_SW_MASK                                          0x000001ff
#define RXDMA_DEBUG_RSTMASK                                          0x00000000
#define RXDMA_DEBUG_RESET                                            0x00000000


#endif /* _MAC_RXDMA_REG_H_ */
