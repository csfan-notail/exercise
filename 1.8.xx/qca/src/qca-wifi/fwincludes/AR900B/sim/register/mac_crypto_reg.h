//                                                                             
// File:       ./headers/mac_crypto_reg.h                                      
// Creator:    afang                                                           
// Time:       Friday Oct 5, 2012 [2:06:08 pm]                                 
//                                                                             
// Path:       /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl
// Arguments:  /cad/denali/blueprint/3.7.4//Linux-64bit/blueprint -odir        
//             ./headers -codegen ath_ch.codegen -ath_ch -Wdesc                
//             mac_crypto_reg.rdl                                              
//                                                                             
// Sources:    /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl/mac_crypto_reg.rdl
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


#ifndef _MAC_CRYPTO_REG_H_
#define _MAC_CRYPTO_REG_H_

// 0x0000 (AES_MUTE_MASK_0)
#define AES_MUTE_MASK_0_QOS_MSB                                      31
#define AES_MUTE_MASK_0_QOS_LSB                                      16
#define AES_MUTE_MASK_0_QOS_MASK                                     0xffff0000
#define AES_MUTE_MASK_0_QOS_GET(x)                                   (((x) & AES_MUTE_MASK_0_QOS_MASK) >> AES_MUTE_MASK_0_QOS_LSB)
#define AES_MUTE_MASK_0_QOS_SET(x)                                   (((0x0 | (x)) << AES_MUTE_MASK_0_QOS_LSB) & AES_MUTE_MASK_0_QOS_MASK)
#define AES_MUTE_MASK_0_QOS_RESET                                    65535
#define AES_MUTE_MASK_0_FC_MSB                                       15
#define AES_MUTE_MASK_0_FC_LSB                                       0
#define AES_MUTE_MASK_0_FC_MASK                                      0x0000ffff
#define AES_MUTE_MASK_0_FC_GET(x)                                    (((x) & AES_MUTE_MASK_0_FC_MASK) >> AES_MUTE_MASK_0_FC_LSB)
#define AES_MUTE_MASK_0_FC_SET(x)                                    (((0x0 | (x)) << AES_MUTE_MASK_0_FC_LSB) & AES_MUTE_MASK_0_FC_MASK)
#define AES_MUTE_MASK_0_FC_RESET                                     18319
#define AES_MUTE_MASK_0_ADDRESS                                      0x000000
#define AES_MUTE_MASK_0_HW_MASK                                      0xffffffff
#define AES_MUTE_MASK_0_SW_MASK                                      0xffffffff
#define AES_MUTE_MASK_0_RSTMASK                                      0xffffffff
#define AES_MUTE_MASK_0_RESET                                        0xffff478f

// 0x0004 (AES_MUTE_MASK_1)
#define AES_MUTE_MASK_1_FC_MGMT_MSB                                  31
#define AES_MUTE_MASK_1_FC_MGMT_LSB                                  16
#define AES_MUTE_MASK_1_FC_MGMT_MASK                                 0xffff0000
#define AES_MUTE_MASK_1_FC_MGMT_GET(x)                               (((x) & AES_MUTE_MASK_1_FC_MGMT_MASK) >> AES_MUTE_MASK_1_FC_MGMT_LSB)
#define AES_MUTE_MASK_1_FC_MGMT_SET(x)                               (((0x0 | (x)) << AES_MUTE_MASK_1_FC_MGMT_LSB) & AES_MUTE_MASK_1_FC_MGMT_MASK)
#define AES_MUTE_MASK_1_FC_MGMT_RESET                                59391
#define AES_MUTE_MASK_1_SEQ_MSB                                      15
#define AES_MUTE_MASK_1_SEQ_LSB                                      0
#define AES_MUTE_MASK_1_SEQ_MASK                                     0x0000ffff
#define AES_MUTE_MASK_1_SEQ_GET(x)                                   (((x) & AES_MUTE_MASK_1_SEQ_MASK) >> AES_MUTE_MASK_1_SEQ_LSB)
#define AES_MUTE_MASK_1_SEQ_SET(x)                                   (((0x0 | (x)) << AES_MUTE_MASK_1_SEQ_LSB) & AES_MUTE_MASK_1_SEQ_MASK)
#define AES_MUTE_MASK_1_SEQ_RESET                                    15
#define AES_MUTE_MASK_1_ADDRESS                                      0x000004
#define AES_MUTE_MASK_1_HW_MASK                                      0xffffffff
#define AES_MUTE_MASK_1_SW_MASK                                      0xffffffff
#define AES_MUTE_MASK_1_RSTMASK                                      0xffffffff
#define AES_MUTE_MASK_1_RESET                                        0xe7ff000f

// 0x0008 (MIC_QOS_CONTROL)
#define MIC_QOS_CONTROL_MIC_QOS_CONTROL_MSB                          15
#define MIC_QOS_CONTROL_MIC_QOS_CONTROL_LSB                          0
#define MIC_QOS_CONTROL_MIC_QOS_CONTROL_MASK                         0x0000ffff
#define MIC_QOS_CONTROL_MIC_QOS_CONTROL_GET(x)                       (((x) & MIC_QOS_CONTROL_MIC_QOS_CONTROL_MASK) >> MIC_QOS_CONTROL_MIC_QOS_CONTROL_LSB)
#define MIC_QOS_CONTROL_MIC_QOS_CONTROL_SET(x)                       (((0x0 | (x)) << MIC_QOS_CONTROL_MIC_QOS_CONTROL_LSB) & MIC_QOS_CONTROL_MIC_QOS_CONTROL_MASK)
#define MIC_QOS_CONTROL_MIC_QOS_CONTROL_RESET                        0
#define MIC_QOS_CONTROL_ADDRESS                                      0x000008
#define MIC_QOS_CONTROL_HW_MASK                                      0x0000ffff
#define MIC_QOS_CONTROL_SW_MASK                                      0x0000ffff
#define MIC_QOS_CONTROL_RSTMASK                                      0x00000000
#define MIC_QOS_CONTROL_RESET                                        0x00000000

// 0x000c (MIC_QOS_SELECT)
#define MIC_QOS_SELECT_MIC_QOS_SELECT_MSB                            31
#define MIC_QOS_SELECT_MIC_QOS_SELECT_LSB                            0
#define MIC_QOS_SELECT_MIC_QOS_SELECT_MASK                           0xffffffff
#define MIC_QOS_SELECT_MIC_QOS_SELECT_GET(x)                         (((x) & MIC_QOS_SELECT_MIC_QOS_SELECT_MASK) >> MIC_QOS_SELECT_MIC_QOS_SELECT_LSB)
#define MIC_QOS_SELECT_MIC_QOS_SELECT_SET(x)                         (((0x0 | (x)) << MIC_QOS_SELECT_MIC_QOS_SELECT_LSB) & MIC_QOS_SELECT_MIC_QOS_SELECT_MASK)
#define MIC_QOS_SELECT_MIC_QOS_SELECT_RESET                          0
#define MIC_QOS_SELECT_ADDRESS                                       0x00000c
#define MIC_QOS_SELECT_HW_MASK                                       0xffffffff
#define MIC_QOS_SELECT_SW_MASK                                       0xffffffff
#define MIC_QOS_SELECT_RSTMASK                                       0x00000000
#define MIC_QOS_SELECT_RESET                                         0x00000000

// 0x0010 (ALT_AES_MUTE_MASK)
#define ALT_AES_MUTE_MASK_QOS_MSB                                    15
#define ALT_AES_MUTE_MASK_QOS_LSB                                    0
#define ALT_AES_MUTE_MASK_QOS_MASK                                   0x0000ffff
#define ALT_AES_MUTE_MASK_QOS_GET(x)                                 (((x) & ALT_AES_MUTE_MASK_QOS_MASK) >> ALT_AES_MUTE_MASK_QOS_LSB)
#define ALT_AES_MUTE_MASK_QOS_SET(x)                                 (((0x0 | (x)) << ALT_AES_MUTE_MASK_QOS_LSB) & ALT_AES_MUTE_MASK_QOS_MASK)
#define ALT_AES_MUTE_MASK_QOS_RESET                                  143
#define ALT_AES_MUTE_MASK_ADDRESS                                    0x000010
#define ALT_AES_MUTE_MASK_HW_MASK                                    0x0000ffff
#define ALT_AES_MUTE_MASK_SW_MASK                                    0x0000ffff
#define ALT_AES_MUTE_MASK_RSTMASK                                    0x0000ffff
#define ALT_AES_MUTE_MASK_RESET                                      0x0000008f

// 0x0014 (CRYPTO_MISC_CONTROL)
#define CRYPTO_MISC_CONTROL_MGMT_QOS_MSB                             23
#define CRYPTO_MISC_CONTROL_MGMT_QOS_LSB                             16
#define CRYPTO_MISC_CONTROL_MGMT_QOS_MASK                            0x00ff0000
#define CRYPTO_MISC_CONTROL_MGMT_QOS_GET(x)                          (((x) & CRYPTO_MISC_CONTROL_MGMT_QOS_MASK) >> CRYPTO_MISC_CONTROL_MGMT_QOS_LSB)
#define CRYPTO_MISC_CONTROL_MGMT_QOS_SET(x)                          (((0x0 | (x)) << CRYPTO_MISC_CONTROL_MGMT_QOS_LSB) & CRYPTO_MISC_CONTROL_MGMT_QOS_MASK)
#define CRYPTO_MISC_CONTROL_MGMT_QOS_RESET                           16
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_MSB                     7
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_LSB                     7
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_MASK                    0x00000080
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_GET(x)                  (((x) & CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_MASK) >> CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_LSB)
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_SET(x)                  (((0x0 | (x)) << CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_LSB) & CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_MASK)
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_RESET                   0
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_MSB                  6
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_LSB                  6
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_MASK                 0x00000040
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_GET(x)               (((x) & CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_MASK) >> CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_LSB)
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_SET(x)               (((0x0 | (x)) << CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_LSB) & CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_MASK)
#define CRYPTO_MISC_CONTROL_BC_MC_WAPI_MODE2_EN_RESET                0
#define CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_MSB           5
#define CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_LSB           5
#define CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_MASK          0x00000020
#define CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_GET(x)        (((x) & CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_MASK) >> CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_LSB)
#define CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_SET(x)        (((0x0 | (x)) << CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_LSB) & CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_MASK)
#define CRYPTO_MISC_CONTROL_NO_CRYPTO_FOR_NON_DATA_PKT_RESET         0
#define CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_MSB                      4
#define CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_LSB                      4
#define CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_MASK                     0x00000010
#define CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_GET(x)                   (((x) & CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_MASK) >> CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_LSB)
#define CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_SET(x)                   (((0x0 | (x)) << CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_LSB) & CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_MASK)
#define CRYPTO_MISC_CONTROL_CRPT_MIC_ENABLE_RESET                    0
#define CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_MSB                3
#define CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_LSB                3
#define CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_MASK               0x00000008
#define CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_GET(x)             (((x) & CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_MASK) >> CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_LSB)
#define CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_SET(x)             (((0x0 | (x)) << CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_LSB) & CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_MASK)
#define CRYPTO_MISC_CONTROL_ZEROLEN_CRYPTO_ENABLE_RESET              0
#define CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_MSB                      2
#define CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_LSB                      2
#define CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_MASK                     0x00000004
#define CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_GET(x)                   (((x) & CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_MASK) >> CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_LSB)
#define CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_SET(x)                   (((0x0 | (x)) << CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_LSB) & CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_MASK)
#define CRYPTO_MISC_CONTROL_WAPI_ORDER_MASK_RESET                    0
#define CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_MSB                     1
#define CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_LSB                     1
#define CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_MASK                    0x00000002
#define CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_GET(x)                  (((x) & CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_MASK) >> CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_LSB)
#define CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_SET(x)                  (((0x0 | (x)) << CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_LSB) & CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_MASK)
#define CRYPTO_MISC_CONTROL_SM_STATE_CONTROL_RESET                   0
#define CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_MSB                   0
#define CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_LSB                   0
#define CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_MASK                  0x00000001
#define CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_GET(x)                (((x) & CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_MASK) >> CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_LSB)
#define CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_SET(x)                (((0x0 | (x)) << CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_LSB) & CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_MASK)
#define CRYPTO_MISC_CONTROL_MGMT_CRYPTO_ENABLE_RESET                 1
#define CRYPTO_MISC_CONTROL_ADDRESS                                  0x000014
#define CRYPTO_MISC_CONTROL_HW_MASK                                  0x00ff00ff
#define CRYPTO_MISC_CONTROL_SW_MASK                                  0x00ff00ff
#define CRYPTO_MISC_CONTROL_RSTMASK                                  0x00ff0001
#define CRYPTO_MISC_CONTROL_RESET                                    0x00100001

// 0x0018 (CRYPTO_SM_STATES)
#define CRYPTO_SM_STATES_SM_STATES_MSB                               31
#define CRYPTO_SM_STATES_SM_STATES_LSB                               0
#define CRYPTO_SM_STATES_SM_STATES_MASK                              0xffffffff
#define CRYPTO_SM_STATES_SM_STATES_GET(x)                            (((x) & CRYPTO_SM_STATES_SM_STATES_MASK) >> CRYPTO_SM_STATES_SM_STATES_LSB)
#define CRYPTO_SM_STATES_SM_STATES_SET(x)                            (((0x0 | (x)) << CRYPTO_SM_STATES_SM_STATES_LSB) & CRYPTO_SM_STATES_SM_STATES_MASK)
#define CRYPTO_SM_STATES_SM_STATES_RESET                             0
#define CRYPTO_SM_STATES_ADDRESS                                     0x000018
#define CRYPTO_SM_STATES_HW_MASK                                     0xffffffff
#define CRYPTO_SM_STATES_SW_MASK                                     0xffffffff
#define CRYPTO_SM_STATES_RSTMASK                                     0x00000000
#define CRYPTO_SM_STATES_RESET                                       0x00000000

// 0x001c (CRYPTO_TESTBUS_SEL)
#define CRYPTO_TESTBUS_SEL_TESTBUS_SEL_MSB                           4
#define CRYPTO_TESTBUS_SEL_TESTBUS_SEL_LSB                           0
#define CRYPTO_TESTBUS_SEL_TESTBUS_SEL_MASK                          0x0000001f
#define CRYPTO_TESTBUS_SEL_TESTBUS_SEL_GET(x)                        (((x) & CRYPTO_TESTBUS_SEL_TESTBUS_SEL_MASK) >> CRYPTO_TESTBUS_SEL_TESTBUS_SEL_LSB)
#define CRYPTO_TESTBUS_SEL_TESTBUS_SEL_SET(x)                        (((0x0 | (x)) << CRYPTO_TESTBUS_SEL_TESTBUS_SEL_LSB) & CRYPTO_TESTBUS_SEL_TESTBUS_SEL_MASK)
#define CRYPTO_TESTBUS_SEL_TESTBUS_SEL_RESET                         0
#define CRYPTO_TESTBUS_SEL_ADDRESS                                   0x00001c
#define CRYPTO_TESTBUS_SEL_HW_MASK                                   0x0000001f
#define CRYPTO_TESTBUS_SEL_SW_MASK                                   0x0000001f
#define CRYPTO_TESTBUS_SEL_RSTMASK                                   0x00000000
#define CRYPTO_TESTBUS_SEL_RESET                                     0x00000000

// 0x0020 (CRYPTO_TESTBUS)
#define CRYPTO_TESTBUS_TESTBUS_MSB                                   31
#define CRYPTO_TESTBUS_TESTBUS_LSB                                   0
#define CRYPTO_TESTBUS_TESTBUS_MASK                                  0xffffffff
#define CRYPTO_TESTBUS_TESTBUS_GET(x)                                (((x) & CRYPTO_TESTBUS_TESTBUS_MASK) >> CRYPTO_TESTBUS_TESTBUS_LSB)
#define CRYPTO_TESTBUS_TESTBUS_SET(x)                                (((0x0 | (x)) << CRYPTO_TESTBUS_TESTBUS_LSB) & CRYPTO_TESTBUS_TESTBUS_MASK)
#define CRYPTO_TESTBUS_TESTBUS_RESET                                 0
#define CRYPTO_TESTBUS_ADDRESS                                       0x000020
#define CRYPTO_TESTBUS_HW_MASK                                       0xffffffff
#define CRYPTO_TESTBUS_SW_MASK                                       0xffffffff
#define CRYPTO_TESTBUS_RSTMASK                                       0x00000000
#define CRYPTO_TESTBUS_RESET                                         0x00000000

// 0x0024 (DEBUG_INTERRUPT_STATUS)
#define DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_MSB                  15
#define DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_LSB                  0
#define DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_MASK                 0x0000ffff
#define DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_GET(x)               (((x) & DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_MASK) >> DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_LSB)
#define DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_SET(x)               (((0x0 | (x)) << DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_LSB) & DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_MASK)
#define DEBUG_INTERRUPT_STATUS_INTERRUPT_STATUS_RESET                0
#define DEBUG_INTERRUPT_STATUS_ADDRESS                               0x000024
#define DEBUG_INTERRUPT_STATUS_HW_MASK                               0x0000ffff
#define DEBUG_INTERRUPT_STATUS_SW_MASK                               0x0000ffff
#define DEBUG_INTERRUPT_STATUS_RSTMASK                               0x00000000
#define DEBUG_INTERRUPT_STATUS_RESET                                 0x00000000

// 0x002c (CRYPTO_CLOCK_GATE_ENABLE)
#define CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_MSB            6
#define CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_LSB            6
#define CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_MASK           0x00000040
#define CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_GET(x)         (((x) & CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_MASK) >> CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_LSB)
#define CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_SET(x)         (((0x0 | (x)) << CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_LSB) & CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_MASK)
#define CRYPTO_CLOCK_GATE_ENABLE_TKIP_CLK_GATE_ENABLE_RESET          0
#define CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_MSB             5
#define CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_LSB             5
#define CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_MASK            0x00000020
#define CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_GET(x)          (((x) & CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_MASK) >> CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_LSB)
#define CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_SET(x)          (((0x0 | (x)) << CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_LSB) & CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_MASK)
#define CRYPTO_CLOCK_GATE_ENABLE_RC4_CLK_GATE_ENABLE_RESET           0
#define CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_MSB            4
#define CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_LSB            4
#define CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_MASK           0x00000010
#define CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_GET(x)         (((x) & CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_MASK) >> CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_LSB)
#define CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_SET(x)         (((0x0 | (x)) << CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_LSB) & CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_MASK)
#define CRYPTO_CLOCK_GATE_ENABLE_WAPI_CLK_GATE_ENABLE_RESET          0
#define CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_MSB            3
#define CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_LSB            3
#define CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_MASK           0x00000008
#define CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_GET(x)         (((x) & CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_MASK) >> CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_LSB)
#define CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_SET(x)         (((0x0 | (x)) << CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_LSB) & CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_MASK)
#define CRYPTO_CLOCK_GATE_ENABLE_AES2_CLK_GATE_ENABLE_RESET          0
#define CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_MSB            2
#define CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_LSB            2
#define CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_MASK           0x00000004
#define CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_GET(x)         (((x) & CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_MASK) >> CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_LSB)
#define CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_SET(x)         (((0x0 | (x)) << CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_LSB) & CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_MASK)
#define CRYPTO_CLOCK_GATE_ENABLE_AES1_CLK_GATE_ENABLE_RESET          0
#define CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_MSB            1
#define CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_LSB            1
#define CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_MASK           0x00000002
#define CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_GET(x)         (((x) & CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_MASK) >> CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_LSB)
#define CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_SET(x)         (((0x0 | (x)) << CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_LSB) & CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_MASK)
#define CRYPTO_CLOCK_GATE_ENABLE_AES0_CLK_GATE_ENABLE_RESET          0
#define CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_MSB             0
#define CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_LSB             0
#define CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_MASK            0x00000001
#define CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_GET(x)          (((x) & CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_MASK) >> CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_LSB)
#define CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_SET(x)          (((0x0 | (x)) << CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_LSB) & CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_MASK)
#define CRYPTO_CLOCK_GATE_ENABLE_AES_CLK_GATE_ENABLE_RESET           0
#define CRYPTO_CLOCK_GATE_ENABLE_ADDRESS                             0x00002c
#define CRYPTO_CLOCK_GATE_ENABLE_HW_MASK                             0x0000007f
#define CRYPTO_CLOCK_GATE_ENABLE_SW_MASK                             0x0000007f
#define CRYPTO_CLOCK_GATE_ENABLE_RSTMASK                             0x00000000
#define CRYPTO_CLOCK_GATE_ENABLE_RESET                               0x00000000

// 0x0030 (WATCHDOG_TIMER)
#define WATCHDOG_TIMER_WATCHDOG_THRESHOLD_MSB                        31
#define WATCHDOG_TIMER_WATCHDOG_THRESHOLD_LSB                        16
#define WATCHDOG_TIMER_WATCHDOG_THRESHOLD_MASK                       0xffff0000
#define WATCHDOG_TIMER_WATCHDOG_THRESHOLD_GET(x)                     (((x) & WATCHDOG_TIMER_WATCHDOG_THRESHOLD_MASK) >> WATCHDOG_TIMER_WATCHDOG_THRESHOLD_LSB)
#define WATCHDOG_TIMER_WATCHDOG_THRESHOLD_SET(x)                     (((0x0 | (x)) << WATCHDOG_TIMER_WATCHDOG_THRESHOLD_LSB) & WATCHDOG_TIMER_WATCHDOG_THRESHOLD_MASK)
#define WATCHDOG_TIMER_WATCHDOG_THRESHOLD_RESET                      65535
#define WATCHDOG_TIMER_WATCHDOG_VALUE_MSB                            15
#define WATCHDOG_TIMER_WATCHDOG_VALUE_LSB                            0
#define WATCHDOG_TIMER_WATCHDOG_VALUE_MASK                           0x0000ffff
#define WATCHDOG_TIMER_WATCHDOG_VALUE_GET(x)                         (((x) & WATCHDOG_TIMER_WATCHDOG_VALUE_MASK) >> WATCHDOG_TIMER_WATCHDOG_VALUE_LSB)
#define WATCHDOG_TIMER_WATCHDOG_VALUE_SET(x)                         (((0x0 | (x)) << WATCHDOG_TIMER_WATCHDOG_VALUE_LSB) & WATCHDOG_TIMER_WATCHDOG_VALUE_MASK)
#define WATCHDOG_TIMER_WATCHDOG_VALUE_RESET                          0
#define WATCHDOG_TIMER_ADDRESS                                       0x000030
#define WATCHDOG_TIMER_HW_MASK                                       0xffffffff
#define WATCHDOG_TIMER_SW_MASK                                       0xffffffff
#define WATCHDOG_TIMER_RSTMASK                                       0xffff0000
#define WATCHDOG_TIMER_RESET                                         0xffff0000


#endif /* _MAC_CRYPTO_REG_H_ */
