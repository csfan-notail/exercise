//                                                                             
// File:       ./headers/mac_pdg_reg.h                                         
// Creator:    afang                                                           
// Time:       Friday Oct 5, 2012 [2:06:43 pm]                                 
//                                                                             
// Path:       /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl
// Arguments:  /cad/denali/blueprint/3.7.4//Linux-64bit/blueprint -odir        
//             ./headers -codegen ath_ch.codegen -ath_ch -Wdesc mac_pdg_reg.rdl
//                                                                             
// Sources:    /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl/mac_pdg_reg.rdl
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


#ifndef _MAC_PDG_REG_H_
#define _MAC_PDG_REG_H_

// 0x0000 (PDG_TEST_CTRL)
#define PDG_TEST_CTRL_TESTBUS_SEL_MSB                                3
#define PDG_TEST_CTRL_TESTBUS_SEL_LSB                                0
#define PDG_TEST_CTRL_TESTBUS_SEL_MASK                               0x0000000f
#define PDG_TEST_CTRL_TESTBUS_SEL_GET(x)                             (((x) & PDG_TEST_CTRL_TESTBUS_SEL_MASK) >> PDG_TEST_CTRL_TESTBUS_SEL_LSB)
#define PDG_TEST_CTRL_TESTBUS_SEL_SET(x)                             (((0x0 | (x)) << PDG_TEST_CTRL_TESTBUS_SEL_LSB) & PDG_TEST_CTRL_TESTBUS_SEL_MASK)
#define PDG_TEST_CTRL_TESTBUS_SEL_RESET                              0
#define PDG_TEST_CTRL_ADDRESS                                        0x000000
#define PDG_TEST_CTRL_HW_MASK                                        0x0000000f
#define PDG_TEST_CTRL_SW_MASK                                        0x0000000f
#define PDG_TEST_CTRL_RSTMASK                                        0x00000000
#define PDG_TEST_CTRL_RESET                                          0x00000000

// 0x0004 (PDG_FSM_STATUS)
#define PDG_FSM_STATUS_FSM_STATUS_MSB                                31
#define PDG_FSM_STATUS_FSM_STATUS_LSB                                0
#define PDG_FSM_STATUS_FSM_STATUS_MASK                               0xffffffff
#define PDG_FSM_STATUS_FSM_STATUS_GET(x)                             (((x) & PDG_FSM_STATUS_FSM_STATUS_MASK) >> PDG_FSM_STATUS_FSM_STATUS_LSB)
#define PDG_FSM_STATUS_FSM_STATUS_SET(x)                             (((0x0 | (x)) << PDG_FSM_STATUS_FSM_STATUS_LSB) & PDG_FSM_STATUS_FSM_STATUS_MASK)
#define PDG_FSM_STATUS_FSM_STATUS_RESET                              0
#define PDG_FSM_STATUS_ADDRESS                                       0x000004
#define PDG_FSM_STATUS_HW_MASK                                       0xffffffff
#define PDG_FSM_STATUS_SW_MASK                                       0xffffffff
#define PDG_FSM_STATUS_RSTMASK                                       0x00000000
#define PDG_FSM_STATUS_RESET                                         0x00000000

// 0x0008 (PDG_TESTBUS)
#define PDG_TESTBUS_TESTBUS_MSB                                      31
#define PDG_TESTBUS_TESTBUS_LSB                                      0
#define PDG_TESTBUS_TESTBUS_MASK                                     0xffffffff
#define PDG_TESTBUS_TESTBUS_GET(x)                                   (((x) & PDG_TESTBUS_TESTBUS_MASK) >> PDG_TESTBUS_TESTBUS_LSB)
#define PDG_TESTBUS_TESTBUS_SET(x)                                   (((0x0 | (x)) << PDG_TESTBUS_TESTBUS_LSB) & PDG_TESTBUS_TESTBUS_MASK)
#define PDG_TESTBUS_TESTBUS_RESET                                    0
#define PDG_TESTBUS_ADDRESS                                          0x000008
#define PDG_TESTBUS_HW_MASK                                          0xffffffff
#define PDG_TESTBUS_SW_MASK                                          0xffffffff
#define PDG_TESTBUS_RSTMASK                                          0x00000000
#define PDG_TESTBUS_RESET                                            0x00000000

// 0x000c (PDG_DBG_COUNTERS)
#define PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_MSB                     31
#define PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_LSB                     24
#define PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_MASK                    0xff000000
#define PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_GET(x)                  (((x) & PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_MASK) >> PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_LSB)
#define PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_SET(x)                  (((0x0 | (x)) << PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_LSB) & PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_MASK)
#define PDG_DBG_COUNTERS_NUM_OF_PDG_RESPONSE_RESET                   0
#define PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_MSB                       23
#define PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_LSB                       16
#define PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_MASK                      0x00ff0000
#define PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_GET(x)                    (((x) & PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_MASK) >> PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_LSB)
#define PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_SET(x)                    (((0x0 | (x)) << PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_LSB) & PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_MASK)
#define PDG_DBG_COUNTERS_NUM_OF_PPDU_SETUP_RESET                     0
#define PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_MSB                 15
#define PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_LSB                 8
#define PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_MASK                0x0000ff00
#define PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_GET(x)              (((x) & PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_MASK) >> PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_LSB)
#define PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_SET(x)              (((0x0 | (x)) << PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_LSB) & PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_MASK)
#define PDG_DBG_COUNTERS_NUM_OF_MPDU_LENGTH_INFO_RESET               0
#define PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_MSB                        7
#define PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_LSB                        0
#define PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_MASK                       0x000000ff
#define PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_GET(x)                     (((x) & PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_MASK) >> PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_LSB)
#define PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_SET(x)                     (((0x0 | (x)) << PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_LSB) & PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_MASK)
#define PDG_DBG_COUNTERS_NUM_OF_FES_SETUP_RESET                      0
#define PDG_DBG_COUNTERS_ADDRESS                                     0x00000c
#define PDG_DBG_COUNTERS_HW_MASK                                     0xffffffff
#define PDG_DBG_COUNTERS_SW_MASK                                     0xffffffff
#define PDG_DBG_COUNTERS_RSTMASK                                     0x00000000
#define PDG_DBG_COUNTERS_RESET                                       0x00000000

// 0x0010 (PDG_CONFIG)
#define PDG_CONFIG_SW_11J_11AF_EN_MSB                                13
#define PDG_CONFIG_SW_11J_11AF_EN_LSB                                13
#define PDG_CONFIG_SW_11J_11AF_EN_MASK                               0x00002000
#define PDG_CONFIG_SW_11J_11AF_EN_GET(x)                             (((x) & PDG_CONFIG_SW_11J_11AF_EN_MASK) >> PDG_CONFIG_SW_11J_11AF_EN_LSB)
#define PDG_CONFIG_SW_11J_11AF_EN_SET(x)                             (((0x0 | (x)) << PDG_CONFIG_SW_11J_11AF_EN_LSB) & PDG_CONFIG_SW_11J_11AF_EN_MASK)
#define PDG_CONFIG_SW_11J_11AF_EN_RESET                              0
#define PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_MSB                      12
#define PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_LSB                      8
#define PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_MASK                     0x00001f00
#define PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_GET(x)                   (((x) & PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_MASK) >> PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_LSB)
#define PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_SET(x)                   (((0x0 | (x)) << PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_LSB) & PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_MASK)
#define PDG_CONFIG_SW_11J_11AF_SCALE_FACTOR_RESET                    0
#define PDG_CONFIG_SW_SIFS_TIME_MSB                                  7
#define PDG_CONFIG_SW_SIFS_TIME_LSB                                  0
#define PDG_CONFIG_SW_SIFS_TIME_MASK                                 0x000000ff
#define PDG_CONFIG_SW_SIFS_TIME_GET(x)                               (((x) & PDG_CONFIG_SW_SIFS_TIME_MASK) >> PDG_CONFIG_SW_SIFS_TIME_LSB)
#define PDG_CONFIG_SW_SIFS_TIME_SET(x)                               (((0x0 | (x)) << PDG_CONFIG_SW_SIFS_TIME_LSB) & PDG_CONFIG_SW_SIFS_TIME_MASK)
#define PDG_CONFIG_SW_SIFS_TIME_RESET                                0
#define PDG_CONFIG_ADDRESS                                           0x000010
#define PDG_CONFIG_HW_MASK                                           0x00003fff
#define PDG_CONFIG_SW_MASK                                           0x00003fff
#define PDG_CONFIG_RSTMASK                                           0x00000000
#define PDG_CONFIG_RESET                                             0x00000000

// 0x0014 (PDG_SCRAMBLING_OVERIDE)
#define PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_MSB           7
#define PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_LSB           7
#define PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_MASK          0x00000080
#define PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_GET(x)        (((x) & PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_MASK) >> PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_LSB)
#define PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_SET(x)        (((0x0 | (x)) << PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_LSB) & PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_MASK)
#define PDG_SCRAMBLING_OVERIDE_FORCE_SW_SHIFT_REG_WORD_RESET         0
#define PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_MSB         6
#define PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_LSB         0
#define PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_MASK        0x0000007f
#define PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_GET(x)      (((x) & PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_MASK) >> PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_LSB)
#define PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_SET(x)      (((0x0 | (x)) << PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_LSB) & PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_MASK)
#define PDG_SCRAMBLING_OVERIDE_SCRAMBLING_SHIFT_REG_WORD_RESET       0
#define PDG_SCRAMBLING_OVERIDE_ADDRESS                               0x000014
#define PDG_SCRAMBLING_OVERIDE_HW_MASK                               0x000000ff
#define PDG_SCRAMBLING_OVERIDE_SW_MASK                               0x000000ff
#define PDG_SCRAMBLING_OVERIDE_RSTMASK                               0x00000000
#define PDG_SCRAMBLING_OVERIDE_RESET                                 0x00000000


#endif /* _MAC_PDG_REG_H_ */
