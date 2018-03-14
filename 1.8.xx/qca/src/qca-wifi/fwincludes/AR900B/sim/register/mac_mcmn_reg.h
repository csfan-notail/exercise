//                                                                             
// File:       ./headers/mac_mcmn_reg.h                                        
// Creator:    afang                                                           
// Time:       Friday Oct 5, 2012 [2:06:27 pm]                                 
//                                                                             
// Path:       /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl
// Arguments:  /cad/denali/blueprint/3.7.4//Linux-64bit/blueprint -odir        
//             ./headers -codegen ath_ch.codegen -ath_ch -Wdesc                
//             mac_mcmn_reg.rdl                                                
//                                                                             
// Sources:    /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl/mac_mcmn_reg.rdl
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


#ifndef _MAC_MCMN_REG_H_
#define _MAC_MCMN_REG_H_

// 0x0000 (MCMN_ISR_P)
#define MCMN_ISR_P_MXI_MSB                                           10
#define MCMN_ISR_P_MXI_LSB                                           10
#define MCMN_ISR_P_MXI_MASK                                          0x00000400
#define MCMN_ISR_P_MXI_GET(x)                                        (((x) & MCMN_ISR_P_MXI_MASK) >> MCMN_ISR_P_MXI_LSB)
#define MCMN_ISR_P_MXI_SET(x)                                        (((0x0 | (x)) << MCMN_ISR_P_MXI_LSB) & MCMN_ISR_P_MXI_MASK)
#define MCMN_ISR_P_MXI_RESET                                         0
#define MCMN_ISR_P_HWSCH2_MSB                                        9
#define MCMN_ISR_P_HWSCH2_LSB                                        9
#define MCMN_ISR_P_HWSCH2_MASK                                       0x00000200
#define MCMN_ISR_P_HWSCH2_GET(x)                                     (((x) & MCMN_ISR_P_HWSCH2_MASK) >> MCMN_ISR_P_HWSCH2_LSB)
#define MCMN_ISR_P_HWSCH2_SET(x)                                     (((0x0 | (x)) << MCMN_ISR_P_HWSCH2_LSB) & MCMN_ISR_P_HWSCH2_MASK)
#define MCMN_ISR_P_HWSCH2_RESET                                      0
#define MCMN_ISR_P_HWSCH1_MSB                                        8
#define MCMN_ISR_P_HWSCH1_LSB                                        8
#define MCMN_ISR_P_HWSCH1_MASK                                       0x00000100
#define MCMN_ISR_P_HWSCH1_GET(x)                                     (((x) & MCMN_ISR_P_HWSCH1_MASK) >> MCMN_ISR_P_HWSCH1_LSB)
#define MCMN_ISR_P_HWSCH1_SET(x)                                     (((0x0 | (x)) << MCMN_ISR_P_HWSCH1_LSB) & MCMN_ISR_P_HWSCH1_MASK)
#define MCMN_ISR_P_HWSCH1_RESET                                      0
#define MCMN_ISR_P_HWSCH0_MSB                                        7
#define MCMN_ISR_P_HWSCH0_LSB                                        7
#define MCMN_ISR_P_HWSCH0_MASK                                       0x00000080
#define MCMN_ISR_P_HWSCH0_GET(x)                                     (((x) & MCMN_ISR_P_HWSCH0_MASK) >> MCMN_ISR_P_HWSCH0_LSB)
#define MCMN_ISR_P_HWSCH0_SET(x)                                     (((0x0 | (x)) << MCMN_ISR_P_HWSCH0_LSB) & MCMN_ISR_P_HWSCH0_MASK)
#define MCMN_ISR_P_HWSCH0_RESET                                      0
#define MCMN_ISR_P_PDG_MSB                                           6
#define MCMN_ISR_P_PDG_LSB                                           6
#define MCMN_ISR_P_PDG_MASK                                          0x00000040
#define MCMN_ISR_P_PDG_GET(x)                                        (((x) & MCMN_ISR_P_PDG_MASK) >> MCMN_ISR_P_PDG_LSB)
#define MCMN_ISR_P_PDG_SET(x)                                        (((0x0 | (x)) << MCMN_ISR_P_PDG_LSB) & MCMN_ISR_P_PDG_MASK)
#define MCMN_ISR_P_PDG_RESET                                         0
#define MCMN_ISR_P_RXPCU_MSB                                         5
#define MCMN_ISR_P_RXPCU_LSB                                         5
#define MCMN_ISR_P_RXPCU_MASK                                        0x00000020
#define MCMN_ISR_P_RXPCU_GET(x)                                      (((x) & MCMN_ISR_P_RXPCU_MASK) >> MCMN_ISR_P_RXPCU_LSB)
#define MCMN_ISR_P_RXPCU_SET(x)                                      (((0x0 | (x)) << MCMN_ISR_P_RXPCU_LSB) & MCMN_ISR_P_RXPCU_MASK)
#define MCMN_ISR_P_RXPCU_RESET                                       0
#define MCMN_ISR_P_TXPCU_MSB                                         4
#define MCMN_ISR_P_TXPCU_LSB                                         4
#define MCMN_ISR_P_TXPCU_MASK                                        0x00000010
#define MCMN_ISR_P_TXPCU_GET(x)                                      (((x) & MCMN_ISR_P_TXPCU_MASK) >> MCMN_ISR_P_TXPCU_LSB)
#define MCMN_ISR_P_TXPCU_SET(x)                                      (((0x0 | (x)) << MCMN_ISR_P_TXPCU_LSB) & MCMN_ISR_P_TXPCU_MASK)
#define MCMN_ISR_P_TXPCU_RESET                                       0
#define MCMN_ISR_P_RXDMA_MSB                                         3
#define MCMN_ISR_P_RXDMA_LSB                                         3
#define MCMN_ISR_P_RXDMA_MASK                                        0x00000008
#define MCMN_ISR_P_RXDMA_GET(x)                                      (((x) & MCMN_ISR_P_RXDMA_MASK) >> MCMN_ISR_P_RXDMA_LSB)
#define MCMN_ISR_P_RXDMA_SET(x)                                      (((0x0 | (x)) << MCMN_ISR_P_RXDMA_LSB) & MCMN_ISR_P_RXDMA_MASK)
#define MCMN_ISR_P_RXDMA_RESET                                       0
#define MCMN_ISR_P_TXDMA_MSB                                         2
#define MCMN_ISR_P_TXDMA_LSB                                         2
#define MCMN_ISR_P_TXDMA_MASK                                        0x00000004
#define MCMN_ISR_P_TXDMA_GET(x)                                      (((x) & MCMN_ISR_P_TXDMA_MASK) >> MCMN_ISR_P_TXDMA_LSB)
#define MCMN_ISR_P_TXDMA_SET(x)                                      (((0x0 | (x)) << MCMN_ISR_P_TXDMA_LSB) & MCMN_ISR_P_TXDMA_MASK)
#define MCMN_ISR_P_TXDMA_RESET                                       0
#define MCMN_ISR_P_CRYPTO_MSB                                        1
#define MCMN_ISR_P_CRYPTO_LSB                                        1
#define MCMN_ISR_P_CRYPTO_MASK                                       0x00000002
#define MCMN_ISR_P_CRYPTO_GET(x)                                     (((x) & MCMN_ISR_P_CRYPTO_MASK) >> MCMN_ISR_P_CRYPTO_LSB)
#define MCMN_ISR_P_CRYPTO_SET(x)                                     (((0x0 | (x)) << MCMN_ISR_P_CRYPTO_LSB) & MCMN_ISR_P_CRYPTO_MASK)
#define MCMN_ISR_P_CRYPTO_RESET                                      0
#define MCMN_ISR_P_OLE_MSB                                           0
#define MCMN_ISR_P_OLE_LSB                                           0
#define MCMN_ISR_P_OLE_MASK                                          0x00000001
#define MCMN_ISR_P_OLE_GET(x)                                        (((x) & MCMN_ISR_P_OLE_MASK) >> MCMN_ISR_P_OLE_LSB)
#define MCMN_ISR_P_OLE_SET(x)                                        (((0x0 | (x)) << MCMN_ISR_P_OLE_LSB) & MCMN_ISR_P_OLE_MASK)
#define MCMN_ISR_P_OLE_RESET                                         0
#define MCMN_ISR_P_ADDRESS                                           0x000000
#define MCMN_ISR_P_HW_MASK                                           0x000007ff
#define MCMN_ISR_P_SW_MASK                                           0x000007ff
#define MCMN_ISR_P_RSTMASK                                           0x00000000
#define MCMN_ISR_P_RESET                                             0x00000000

// 0x0004 (MCMN_ISR_S0)
#define MCMN_ISR_S0_RX_WMAC_ERR_MSB                                  13
#define MCMN_ISR_S0_RX_WMAC_ERR_LSB                                  13
#define MCMN_ISR_S0_RX_WMAC_ERR_MASK                                 0x00002000
#define MCMN_ISR_S0_RX_WMAC_ERR_GET(x)                               (((x) & MCMN_ISR_S0_RX_WMAC_ERR_MASK) >> MCMN_ISR_S0_RX_WMAC_ERR_LSB)
#define MCMN_ISR_S0_RX_WMAC_ERR_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S0_RX_WMAC_ERR_LSB) & MCMN_ISR_S0_RX_WMAC_ERR_MASK)
#define MCMN_ISR_S0_RX_WMAC_ERR_RESET                                0
#define MCMN_ISR_S0_RX_ETH_ERR_MSB                                   12
#define MCMN_ISR_S0_RX_ETH_ERR_LSB                                   12
#define MCMN_ISR_S0_RX_ETH_ERR_MASK                                  0x00001000
#define MCMN_ISR_S0_RX_ETH_ERR_GET(x)                                (((x) & MCMN_ISR_S0_RX_ETH_ERR_MASK) >> MCMN_ISR_S0_RX_ETH_ERR_LSB)
#define MCMN_ISR_S0_RX_ETH_ERR_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S0_RX_ETH_ERR_LSB) & MCMN_ISR_S0_RX_ETH_ERR_MASK)
#define MCMN_ISR_S0_RX_ETH_ERR_RESET                                 0
#define MCMN_ISR_S0_RX_MSDU_LEN_MSB                                  11
#define MCMN_ISR_S0_RX_MSDU_LEN_LSB                                  11
#define MCMN_ISR_S0_RX_MSDU_LEN_MASK                                 0x00000800
#define MCMN_ISR_S0_RX_MSDU_LEN_GET(x)                               (((x) & MCMN_ISR_S0_RX_MSDU_LEN_MASK) >> MCMN_ISR_S0_RX_MSDU_LEN_LSB)
#define MCMN_ISR_S0_RX_MSDU_LEN_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S0_RX_MSDU_LEN_LSB) & MCMN_ISR_S0_RX_MSDU_LEN_MASK)
#define MCMN_ISR_S0_RX_MSDU_LEN_RESET                                0
#define MCMN_ISR_S0_TX_ILLEGAL_MSB                                   10
#define MCMN_ISR_S0_TX_ILLEGAL_LSB                                   10
#define MCMN_ISR_S0_TX_ILLEGAL_MASK                                  0x00000400
#define MCMN_ISR_S0_TX_ILLEGAL_GET(x)                                (((x) & MCMN_ISR_S0_TX_ILLEGAL_MASK) >> MCMN_ISR_S0_TX_ILLEGAL_LSB)
#define MCMN_ISR_S0_TX_ILLEGAL_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S0_TX_ILLEGAL_LSB) & MCMN_ISR_S0_TX_ILLEGAL_MASK)
#define MCMN_ISR_S0_TX_ILLEGAL_RESET                                 0
#define MCMN_ISR_S0_TX_UNKN_LLC_MSB                                  9
#define MCMN_ISR_S0_TX_UNKN_LLC_LSB                                  9
#define MCMN_ISR_S0_TX_UNKN_LLC_MASK                                 0x00000200
#define MCMN_ISR_S0_TX_UNKN_LLC_GET(x)                               (((x) & MCMN_ISR_S0_TX_UNKN_LLC_MASK) >> MCMN_ISR_S0_TX_UNKN_LLC_LSB)
#define MCMN_ISR_S0_TX_UNKN_LLC_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S0_TX_UNKN_LLC_LSB) & MCMN_ISR_S0_TX_UNKN_LLC_MASK)
#define MCMN_ISR_S0_TX_UNKN_LLC_RESET                                0
#define MCMN_ISR_S0_TX_VLAN_LLC_MSB                                  8
#define MCMN_ISR_S0_TX_VLAN_LLC_LSB                                  8
#define MCMN_ISR_S0_TX_VLAN_LLC_MASK                                 0x00000100
#define MCMN_ISR_S0_TX_VLAN_LLC_GET(x)                               (((x) & MCMN_ISR_S0_TX_VLAN_LLC_MASK) >> MCMN_ISR_S0_TX_VLAN_LLC_LSB)
#define MCMN_ISR_S0_TX_VLAN_LLC_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S0_TX_VLAN_LLC_LSB) & MCMN_ISR_S0_TX_VLAN_LLC_MASK)
#define MCMN_ISR_S0_TX_VLAN_LLC_RESET                                0
#define MCMN_ISR_S0_WEP_KEY_ERR_MSB                                  7
#define MCMN_ISR_S0_WEP_KEY_ERR_LSB                                  7
#define MCMN_ISR_S0_WEP_KEY_ERR_MASK                                 0x00000080
#define MCMN_ISR_S0_WEP_KEY_ERR_GET(x)                               (((x) & MCMN_ISR_S0_WEP_KEY_ERR_MASK) >> MCMN_ISR_S0_WEP_KEY_ERR_LSB)
#define MCMN_ISR_S0_WEP_KEY_ERR_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S0_WEP_KEY_ERR_LSB) & MCMN_ISR_S0_WEP_KEY_ERR_MASK)
#define MCMN_ISR_S0_WEP_KEY_ERR_RESET                                0
#define MCMN_ISR_S0_TX_SEQ_BM_ERR_MSB                                6
#define MCMN_ISR_S0_TX_SEQ_BM_ERR_LSB                                6
#define MCMN_ISR_S0_TX_SEQ_BM_ERR_MASK                               0x00000040
#define MCMN_ISR_S0_TX_SEQ_BM_ERR_GET(x)                             (((x) & MCMN_ISR_S0_TX_SEQ_BM_ERR_MASK) >> MCMN_ISR_S0_TX_SEQ_BM_ERR_LSB)
#define MCMN_ISR_S0_TX_SEQ_BM_ERR_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S0_TX_SEQ_BM_ERR_LSB) & MCMN_ISR_S0_TX_SEQ_BM_ERR_MASK)
#define MCMN_ISR_S0_TX_SEQ_BM_ERR_RESET                              0
#define MCMN_ISR_S0_TX_PN_BM_ERR_MSB                                 5
#define MCMN_ISR_S0_TX_PN_BM_ERR_LSB                                 5
#define MCMN_ISR_S0_TX_PN_BM_ERR_MASK                                0x00000020
#define MCMN_ISR_S0_TX_PN_BM_ERR_GET(x)                              (((x) & MCMN_ISR_S0_TX_PN_BM_ERR_MASK) >> MCMN_ISR_S0_TX_PN_BM_ERR_LSB)
#define MCMN_ISR_S0_TX_PN_BM_ERR_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S0_TX_PN_BM_ERR_LSB) & MCMN_ISR_S0_TX_PN_BM_ERR_MASK)
#define MCMN_ISR_S0_TX_PN_BM_ERR_RESET                               0
#define MCMN_ISR_S0_RETRY_BM_ERR_MSB                                 4
#define MCMN_ISR_S0_RETRY_BM_ERR_LSB                                 4
#define MCMN_ISR_S0_RETRY_BM_ERR_MASK                                0x00000010
#define MCMN_ISR_S0_RETRY_BM_ERR_GET(x)                              (((x) & MCMN_ISR_S0_RETRY_BM_ERR_MASK) >> MCMN_ISR_S0_RETRY_BM_ERR_LSB)
#define MCMN_ISR_S0_RETRY_BM_ERR_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S0_RETRY_BM_ERR_LSB) & MCMN_ISR_S0_RETRY_BM_ERR_MASK)
#define MCMN_ISR_S0_RETRY_BM_ERR_RESET                               0
#define MCMN_ISR_S0_AMSDU_FRM_ERR_MSB                                3
#define MCMN_ISR_S0_AMSDU_FRM_ERR_LSB                                3
#define MCMN_ISR_S0_AMSDU_FRM_ERR_MASK                               0x00000008
#define MCMN_ISR_S0_AMSDU_FRM_ERR_GET(x)                             (((x) & MCMN_ISR_S0_AMSDU_FRM_ERR_MASK) >> MCMN_ISR_S0_AMSDU_FRM_ERR_LSB)
#define MCMN_ISR_S0_AMSDU_FRM_ERR_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S0_AMSDU_FRM_ERR_LSB) & MCMN_ISR_S0_AMSDU_FRM_ERR_MASK)
#define MCMN_ISR_S0_AMSDU_FRM_ERR_RESET                              0
#define MCMN_ISR_S0_TX_TLV_ERR_MSB                                   2
#define MCMN_ISR_S0_TX_TLV_ERR_LSB                                   2
#define MCMN_ISR_S0_TX_TLV_ERR_MASK                                  0x00000004
#define MCMN_ISR_S0_TX_TLV_ERR_GET(x)                                (((x) & MCMN_ISR_S0_TX_TLV_ERR_MASK) >> MCMN_ISR_S0_TX_TLV_ERR_LSB)
#define MCMN_ISR_S0_TX_TLV_ERR_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S0_TX_TLV_ERR_LSB) & MCMN_ISR_S0_TX_TLV_ERR_MASK)
#define MCMN_ISR_S0_TX_TLV_ERR_RESET                                 0
#define MCMN_ISR_S0_RX_TLV_ERR_MSB                                   1
#define MCMN_ISR_S0_RX_TLV_ERR_LSB                                   1
#define MCMN_ISR_S0_RX_TLV_ERR_MASK                                  0x00000002
#define MCMN_ISR_S0_RX_TLV_ERR_GET(x)                                (((x) & MCMN_ISR_S0_RX_TLV_ERR_MASK) >> MCMN_ISR_S0_RX_TLV_ERR_LSB)
#define MCMN_ISR_S0_RX_TLV_ERR_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S0_RX_TLV_ERR_LSB) & MCMN_ISR_S0_RX_TLV_ERR_MASK)
#define MCMN_ISR_S0_RX_TLV_ERR_RESET                                 0
#define MCMN_ISR_S0_WDOG_TOUT_MSB                                    0
#define MCMN_ISR_S0_WDOG_TOUT_LSB                                    0
#define MCMN_ISR_S0_WDOG_TOUT_MASK                                   0x00000001
#define MCMN_ISR_S0_WDOG_TOUT_GET(x)                                 (((x) & MCMN_ISR_S0_WDOG_TOUT_MASK) >> MCMN_ISR_S0_WDOG_TOUT_LSB)
#define MCMN_ISR_S0_WDOG_TOUT_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S0_WDOG_TOUT_LSB) & MCMN_ISR_S0_WDOG_TOUT_MASK)
#define MCMN_ISR_S0_WDOG_TOUT_RESET                                  0
#define MCMN_ISR_S0_ADDRESS                                          0x000004
#define MCMN_ISR_S0_HW_MASK                                          0x00003fff
#define MCMN_ISR_S0_SW_MASK                                          0x00003fff
#define MCMN_ISR_S0_RSTMASK                                          0x00000000
#define MCMN_ISR_S0_RESET                                            0x00000000

// 0x0008 (MCMN_ISR_S1)
#define MCMN_ISR_S1_RSVD_GT_MPDU_MSB                                 5
#define MCMN_ISR_S1_RSVD_GT_MPDU_LSB                                 5
#define MCMN_ISR_S1_RSVD_GT_MPDU_MASK                                0x00000020
#define MCMN_ISR_S1_RSVD_GT_MPDU_GET(x)                              (((x) & MCMN_ISR_S1_RSVD_GT_MPDU_MASK) >> MCMN_ISR_S1_RSVD_GT_MPDU_LSB)
#define MCMN_ISR_S1_RSVD_GT_MPDU_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S1_RSVD_GT_MPDU_LSB) & MCMN_ISR_S1_RSVD_GT_MPDU_MASK)
#define MCMN_ISR_S1_RSVD_GT_MPDU_RESET                               0
#define MCMN_ISR_S1_RSVD_LT_MPDU_MSB                                 4
#define MCMN_ISR_S1_RSVD_LT_MPDU_LSB                                 4
#define MCMN_ISR_S1_RSVD_LT_MPDU_MASK                                0x00000010
#define MCMN_ISR_S1_RSVD_LT_MPDU_GET(x)                              (((x) & MCMN_ISR_S1_RSVD_LT_MPDU_MASK) >> MCMN_ISR_S1_RSVD_LT_MPDU_LSB)
#define MCMN_ISR_S1_RSVD_LT_MPDU_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S1_RSVD_LT_MPDU_LSB) & MCMN_ISR_S1_RSVD_LT_MPDU_MASK)
#define MCMN_ISR_S1_RSVD_LT_MPDU_RESET                               0
#define MCMN_ISR_S1_DECRPT_ERR_MSB                                   3
#define MCMN_ISR_S1_DECRPT_ERR_LSB                                   3
#define MCMN_ISR_S1_DECRPT_ERR_MASK                                  0x00000008
#define MCMN_ISR_S1_DECRPT_ERR_GET(x)                                (((x) & MCMN_ISR_S1_DECRPT_ERR_MASK) >> MCMN_ISR_S1_DECRPT_ERR_LSB)
#define MCMN_ISR_S1_DECRPT_ERR_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S1_DECRPT_ERR_LSB) & MCMN_ISR_S1_DECRPT_ERR_MASK)
#define MCMN_ISR_S1_DECRPT_ERR_RESET                                 0
#define MCMN_ISR_S1_RX_TLV_OUT_SEQ_MSB                               2
#define MCMN_ISR_S1_RX_TLV_OUT_SEQ_LSB                               2
#define MCMN_ISR_S1_RX_TLV_OUT_SEQ_MASK                              0x00000004
#define MCMN_ISR_S1_RX_TLV_OUT_SEQ_GET(x)                            (((x) & MCMN_ISR_S1_RX_TLV_OUT_SEQ_MASK) >> MCMN_ISR_S1_RX_TLV_OUT_SEQ_LSB)
#define MCMN_ISR_S1_RX_TLV_OUT_SEQ_SET(x)                            (((0x0 | (x)) << MCMN_ISR_S1_RX_TLV_OUT_SEQ_LSB) & MCMN_ISR_S1_RX_TLV_OUT_SEQ_MASK)
#define MCMN_ISR_S1_RX_TLV_OUT_SEQ_RESET                             0
#define MCMN_ISR_S1_TX_TLV_OUT_SEQ_MSB                               1
#define MCMN_ISR_S1_TX_TLV_OUT_SEQ_LSB                               1
#define MCMN_ISR_S1_TX_TLV_OUT_SEQ_MASK                              0x00000002
#define MCMN_ISR_S1_TX_TLV_OUT_SEQ_GET(x)                            (((x) & MCMN_ISR_S1_TX_TLV_OUT_SEQ_MASK) >> MCMN_ISR_S1_TX_TLV_OUT_SEQ_LSB)
#define MCMN_ISR_S1_TX_TLV_OUT_SEQ_SET(x)                            (((0x0 | (x)) << MCMN_ISR_S1_TX_TLV_OUT_SEQ_LSB) & MCMN_ISR_S1_TX_TLV_OUT_SEQ_MASK)
#define MCMN_ISR_S1_TX_TLV_OUT_SEQ_RESET                             0
#define MCMN_ISR_S1_CRYP_WD_TMOUT_MSB                                0
#define MCMN_ISR_S1_CRYP_WD_TMOUT_LSB                                0
#define MCMN_ISR_S1_CRYP_WD_TMOUT_MASK                               0x00000001
#define MCMN_ISR_S1_CRYP_WD_TMOUT_GET(x)                             (((x) & MCMN_ISR_S1_CRYP_WD_TMOUT_MASK) >> MCMN_ISR_S1_CRYP_WD_TMOUT_LSB)
#define MCMN_ISR_S1_CRYP_WD_TMOUT_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S1_CRYP_WD_TMOUT_LSB) & MCMN_ISR_S1_CRYP_WD_TMOUT_MASK)
#define MCMN_ISR_S1_CRYP_WD_TMOUT_RESET                              0
#define MCMN_ISR_S1_ADDRESS                                          0x000008
#define MCMN_ISR_S1_HW_MASK                                          0x0000003f
#define MCMN_ISR_S1_SW_MASK                                          0x0000003f
#define MCMN_ISR_S1_RSTMASK                                          0x00000000
#define MCMN_ISR_S1_RESET                                            0x00000000

// 0x000c (MCMN_ISR_S2)
#define MCMN_ISR_S2_AXI_TMOUT_MSB                                    9
#define MCMN_ISR_S2_AXI_TMOUT_LSB                                    9
#define MCMN_ISR_S2_AXI_TMOUT_MASK                                   0x00000200
#define MCMN_ISR_S2_AXI_TMOUT_GET(x)                                 (((x) & MCMN_ISR_S2_AXI_TMOUT_MASK) >> MCMN_ISR_S2_AXI_TMOUT_LSB)
#define MCMN_ISR_S2_AXI_TMOUT_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S2_AXI_TMOUT_LSB) & MCMN_ISR_S2_AXI_TMOUT_MASK)
#define MCMN_ISR_S2_AXI_TMOUT_RESET                                  0
#define MCMN_ISR_S2_OLE_RDY_TMOUT_MSB                                8
#define MCMN_ISR_S2_OLE_RDY_TMOUT_LSB                                8
#define MCMN_ISR_S2_OLE_RDY_TMOUT_MASK                               0x00000100
#define MCMN_ISR_S2_OLE_RDY_TMOUT_GET(x)                             (((x) & MCMN_ISR_S2_OLE_RDY_TMOUT_MASK) >> MCMN_ISR_S2_OLE_RDY_TMOUT_LSB)
#define MCMN_ISR_S2_OLE_RDY_TMOUT_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S2_OLE_RDY_TMOUT_LSB) & MCMN_ISR_S2_OLE_RDY_TMOUT_MASK)
#define MCMN_ISR_S2_OLE_RDY_TMOUT_RESET                              0
#define MCMN_ISR_S2_UNEX_NUL_PTR_MSB                                 7
#define MCMN_ISR_S2_UNEX_NUL_PTR_LSB                                 7
#define MCMN_ISR_S2_UNEX_NUL_PTR_MASK                                0x00000080
#define MCMN_ISR_S2_UNEX_NUL_PTR_GET(x)                              (((x) & MCMN_ISR_S2_UNEX_NUL_PTR_MASK) >> MCMN_ISR_S2_UNEX_NUL_PTR_LSB)
#define MCMN_ISR_S2_UNEX_NUL_PTR_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S2_UNEX_NUL_PTR_LSB) & MCMN_ISR_S2_UNEX_NUL_PTR_MASK)
#define MCMN_ISR_S2_UNEX_NUL_PTR_RESET                               0
#define MCMN_ISR_S2_WDG_TMOUT_MSB                                    6
#define MCMN_ISR_S2_WDG_TMOUT_LSB                                    6
#define MCMN_ISR_S2_WDG_TMOUT_MASK                                   0x00000040
#define MCMN_ISR_S2_WDG_TMOUT_GET(x)                                 (((x) & MCMN_ISR_S2_WDG_TMOUT_MASK) >> MCMN_ISR_S2_WDG_TMOUT_LSB)
#define MCMN_ISR_S2_WDG_TMOUT_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S2_WDG_TMOUT_LSB) & MCMN_ISR_S2_WDG_TMOUT_MASK)
#define MCMN_ISR_S2_WDG_TMOUT_RESET                                  0
#define MCMN_ISR_S2_INVA_IDX_MSB                                     5
#define MCMN_ISR_S2_INVA_IDX_LSB                                     5
#define MCMN_ISR_S2_INVA_IDX_MASK                                    0x00000020
#define MCMN_ISR_S2_INVA_IDX_GET(x)                                  (((x) & MCMN_ISR_S2_INVA_IDX_MASK) >> MCMN_ISR_S2_INVA_IDX_LSB)
#define MCMN_ISR_S2_INVA_IDX_SET(x)                                  (((0x0 | (x)) << MCMN_ISR_S2_INVA_IDX_LSB) & MCMN_ISR_S2_INVA_IDX_MASK)
#define MCMN_ISR_S2_INVA_IDX_RESET                                   0
#define MCMN_ISR_S2_BAD_TLV_ORDR_MSB                                 4
#define MCMN_ISR_S2_BAD_TLV_ORDR_LSB                                 4
#define MCMN_ISR_S2_BAD_TLV_ORDR_MASK                                0x00000010
#define MCMN_ISR_S2_BAD_TLV_ORDR_GET(x)                              (((x) & MCMN_ISR_S2_BAD_TLV_ORDR_MASK) >> MCMN_ISR_S2_BAD_TLV_ORDR_LSB)
#define MCMN_ISR_S2_BAD_TLV_ORDR_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S2_BAD_TLV_ORDR_LSB) & MCMN_ISR_S2_BAD_TLV_ORDR_MASK)
#define MCMN_ISR_S2_BAD_TLV_ORDR_RESET                               0
#define MCMN_ISR_S2_MISSIN_TLV_MSB                                   3
#define MCMN_ISR_S2_MISSIN_TLV_LSB                                   3
#define MCMN_ISR_S2_MISSIN_TLV_MASK                                  0x00000008
#define MCMN_ISR_S2_MISSIN_TLV_GET(x)                                (((x) & MCMN_ISR_S2_MISSIN_TLV_MASK) >> MCMN_ISR_S2_MISSIN_TLV_LSB)
#define MCMN_ISR_S2_MISSIN_TLV_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S2_MISSIN_TLV_LSB) & MCMN_ISR_S2_MISSIN_TLV_MASK)
#define MCMN_ISR_S2_MISSIN_TLV_RESET                                 0
#define MCMN_ISR_S2_UNEX_FES_MSB                                     2
#define MCMN_ISR_S2_UNEX_FES_LSB                                     2
#define MCMN_ISR_S2_UNEX_FES_MASK                                    0x00000004
#define MCMN_ISR_S2_UNEX_FES_GET(x)                                  (((x) & MCMN_ISR_S2_UNEX_FES_MASK) >> MCMN_ISR_S2_UNEX_FES_LSB)
#define MCMN_ISR_S2_UNEX_FES_SET(x)                                  (((0x0 | (x)) << MCMN_ISR_S2_UNEX_FES_LSB) & MCMN_ISR_S2_UNEX_FES_MASK)
#define MCMN_ISR_S2_UNEX_FES_RESET                                   0
#define MCMN_ISR_S2_INCOM_TLV_MSB                                    1
#define MCMN_ISR_S2_INCOM_TLV_LSB                                    1
#define MCMN_ISR_S2_INCOM_TLV_MASK                                   0x00000002
#define MCMN_ISR_S2_INCOM_TLV_GET(x)                                 (((x) & MCMN_ISR_S2_INCOM_TLV_MASK) >> MCMN_ISR_S2_INCOM_TLV_LSB)
#define MCMN_ISR_S2_INCOM_TLV_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S2_INCOM_TLV_LSB) & MCMN_ISR_S2_INCOM_TLV_MASK)
#define MCMN_ISR_S2_INCOM_TLV_RESET                                  0
#define MCMN_ISR_S2_TX_DMA_IDLE_MSB                                  0
#define MCMN_ISR_S2_TX_DMA_IDLE_LSB                                  0
#define MCMN_ISR_S2_TX_DMA_IDLE_MASK                                 0x00000001
#define MCMN_ISR_S2_TX_DMA_IDLE_GET(x)                               (((x) & MCMN_ISR_S2_TX_DMA_IDLE_MASK) >> MCMN_ISR_S2_TX_DMA_IDLE_LSB)
#define MCMN_ISR_S2_TX_DMA_IDLE_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S2_TX_DMA_IDLE_LSB) & MCMN_ISR_S2_TX_DMA_IDLE_MASK)
#define MCMN_ISR_S2_TX_DMA_IDLE_RESET                                0
#define MCMN_ISR_S2_ADDRESS                                          0x00000c
#define MCMN_ISR_S2_HW_MASK                                          0x000003ff
#define MCMN_ISR_S2_SW_MASK                                          0x000003ff
#define MCMN_ISR_S2_RSTMASK                                          0x00000000
#define MCMN_ISR_S2_RESET                                            0x00000000

// 0x0010 (MCMN_ISR_S3)
#define MCMN_ISR_S3_RXDMA_MSDU_MSB                                   16
#define MCMN_ISR_S3_RXDMA_MSDU_LSB                                   16
#define MCMN_ISR_S3_RXDMA_MSDU_MASK                                  0x00010000
#define MCMN_ISR_S3_RXDMA_MSDU_GET(x)                                (((x) & MCMN_ISR_S3_RXDMA_MSDU_MASK) >> MCMN_ISR_S3_RXDMA_MSDU_LSB)
#define MCMN_ISR_S3_RXDMA_MSDU_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S3_RXDMA_MSDU_LSB) & MCMN_ISR_S3_RXDMA_MSDU_MASK)
#define MCMN_ISR_S3_RXDMA_MSDU_RESET                                 0
#define MCMN_ISR_S3_RXDMA_MPDU_MSB                                   15
#define MCMN_ISR_S3_RXDMA_MPDU_LSB                                   15
#define MCMN_ISR_S3_RXDMA_MPDU_MASK                                  0x00008000
#define MCMN_ISR_S3_RXDMA_MPDU_GET(x)                                (((x) & MCMN_ISR_S3_RXDMA_MPDU_MASK) >> MCMN_ISR_S3_RXDMA_MPDU_LSB)
#define MCMN_ISR_S3_RXDMA_MPDU_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S3_RXDMA_MPDU_LSB) & MCMN_ISR_S3_RXDMA_MPDU_MASK)
#define MCMN_ISR_S3_RXDMA_MPDU_RESET                                 0
#define MCMN_ISR_S3_RXDMA_PPDU_MSB                                   14
#define MCMN_ISR_S3_RXDMA_PPDU_LSB                                   14
#define MCMN_ISR_S3_RXDMA_PPDU_MASK                                  0x00004000
#define MCMN_ISR_S3_RXDMA_PPDU_GET(x)                                (((x) & MCMN_ISR_S3_RXDMA_PPDU_MASK) >> MCMN_ISR_S3_RXDMA_PPDU_LSB)
#define MCMN_ISR_S3_RXDMA_PPDU_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S3_RXDMA_PPDU_LSB) & MCMN_ISR_S3_RXDMA_PPDU_MASK)
#define MCMN_ISR_S3_RXDMA_PPDU_RESET                                 0
#define MCMN_ISR_S3_RXDMA_UAPSD_MSB                                  13
#define MCMN_ISR_S3_RXDMA_UAPSD_LSB                                  13
#define MCMN_ISR_S3_RXDMA_UAPSD_MASK                                 0x00002000
#define MCMN_ISR_S3_RXDMA_UAPSD_GET(x)                               (((x) & MCMN_ISR_S3_RXDMA_UAPSD_MASK) >> MCMN_ISR_S3_RXDMA_UAPSD_LSB)
#define MCMN_ISR_S3_RXDMA_UAPSD_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S3_RXDMA_UAPSD_LSB) & MCMN_ISR_S3_RXDMA_UAPSD_MASK)
#define MCMN_ISR_S3_RXDMA_UAPSD_RESET                                0
#define MCMN_ISR_S3_RXDMA_RNG_LWM_MSB                                12
#define MCMN_ISR_S3_RXDMA_RNG_LWM_LSB                                9
#define MCMN_ISR_S3_RXDMA_RNG_LWM_MASK                               0x00001e00
#define MCMN_ISR_S3_RXDMA_RNG_LWM_GET(x)                             (((x) & MCMN_ISR_S3_RXDMA_RNG_LWM_MASK) >> MCMN_ISR_S3_RXDMA_RNG_LWM_LSB)
#define MCMN_ISR_S3_RXDMA_RNG_LWM_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S3_RXDMA_RNG_LWM_LSB) & MCMN_ISR_S3_RXDMA_RNG_LWM_MASK)
#define MCMN_ISR_S3_RXDMA_RNG_LWM_RESET                              0
#define MCMN_ISR_S3_TLV_FRME_ERR_MSB                                 8
#define MCMN_ISR_S3_TLV_FRME_ERR_LSB                                 8
#define MCMN_ISR_S3_TLV_FRME_ERR_MASK                                0x00000100
#define MCMN_ISR_S3_TLV_FRME_ERR_GET(x)                              (((x) & MCMN_ISR_S3_TLV_FRME_ERR_MASK) >> MCMN_ISR_S3_TLV_FRME_ERR_LSB)
#define MCMN_ISR_S3_TLV_FRME_ERR_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S3_TLV_FRME_ERR_LSB) & MCMN_ISR_S3_TLV_FRME_ERR_MASK)
#define MCMN_ISR_S3_TLV_FRME_ERR_RESET                               0
#define MCMN_ISR_S3_TLV_SHORT_MSB                                    7
#define MCMN_ISR_S3_TLV_SHORT_LSB                                    7
#define MCMN_ISR_S3_TLV_SHORT_MASK                                   0x00000080
#define MCMN_ISR_S3_TLV_SHORT_GET(x)                                 (((x) & MCMN_ISR_S3_TLV_SHORT_MASK) >> MCMN_ISR_S3_TLV_SHORT_LSB)
#define MCMN_ISR_S3_TLV_SHORT_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S3_TLV_SHORT_LSB) & MCMN_ISR_S3_TLV_SHORT_MASK)
#define MCMN_ISR_S3_TLV_SHORT_RESET                                  0
#define MCMN_ISR_S3_INVAL_TAG_SEG_MSB                                6
#define MCMN_ISR_S3_INVAL_TAG_SEG_LSB                                6
#define MCMN_ISR_S3_INVAL_TAG_SEG_MASK                               0x00000040
#define MCMN_ISR_S3_INVAL_TAG_SEG_GET(x)                             (((x) & MCMN_ISR_S3_INVAL_TAG_SEG_MASK) >> MCMN_ISR_S3_INVAL_TAG_SEG_LSB)
#define MCMN_ISR_S3_INVAL_TAG_SEG_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S3_INVAL_TAG_SEG_LSB) & MCMN_ISR_S3_INVAL_TAG_SEG_MASK)
#define MCMN_ISR_S3_INVAL_TAG_SEG_RESET                              0
#define MCMN_ISR_S3_INVAL_TAG_ID_MSB                                 5
#define MCMN_ISR_S3_INVAL_TAG_ID_LSB                                 5
#define MCMN_ISR_S3_INVAL_TAG_ID_MASK                                0x00000020
#define MCMN_ISR_S3_INVAL_TAG_ID_GET(x)                              (((x) & MCMN_ISR_S3_INVAL_TAG_ID_MASK) >> MCMN_ISR_S3_INVAL_TAG_ID_LSB)
#define MCMN_ISR_S3_INVAL_TAG_ID_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S3_INVAL_TAG_ID_LSB) & MCMN_ISR_S3_INVAL_TAG_ID_MASK)
#define MCMN_ISR_S3_INVAL_TAG_ID_RESET                               0
#define MCMN_ISR_S3_NULL_RNG_MSK_MSB                                 4
#define MCMN_ISR_S3_NULL_RNG_MSK_LSB                                 4
#define MCMN_ISR_S3_NULL_RNG_MSK_MASK                                0x00000010
#define MCMN_ISR_S3_NULL_RNG_MSK_GET(x)                              (((x) & MCMN_ISR_S3_NULL_RNG_MSK_MASK) >> MCMN_ISR_S3_NULL_RNG_MSK_LSB)
#define MCMN_ISR_S3_NULL_RNG_MSK_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S3_NULL_RNG_MSK_LSB) & MCMN_ISR_S3_NULL_RNG_MSK_MASK)
#define MCMN_ISR_S3_NULL_RNG_MSK_RESET                               0
#define MCMN_ISR_S3_MDAT_TLV_MSB                                     3
#define MCMN_ISR_S3_MDAT_TLV_LSB                                     3
#define MCMN_ISR_S3_MDAT_TLV_MASK                                    0x00000008
#define MCMN_ISR_S3_MDAT_TLV_GET(x)                                  (((x) & MCMN_ISR_S3_MDAT_TLV_MASK) >> MCMN_ISR_S3_MDAT_TLV_LSB)
#define MCMN_ISR_S3_MDAT_TLV_SET(x)                                  (((0x0 | (x)) << MCMN_ISR_S3_MDAT_TLV_LSB) & MCMN_ISR_S3_MDAT_TLV_MASK)
#define MCMN_ISR_S3_MDAT_TLV_RESET                                   0
#define MCMN_ISR_S3_NON_PKT_BUF_MSB                                  2
#define MCMN_ISR_S3_NON_PKT_BUF_LSB                                  2
#define MCMN_ISR_S3_NON_PKT_BUF_MASK                                 0x00000004
#define MCMN_ISR_S3_NON_PKT_BUF_GET(x)                               (((x) & MCMN_ISR_S3_NON_PKT_BUF_MASK) >> MCMN_ISR_S3_NON_PKT_BUF_LSB)
#define MCMN_ISR_S3_NON_PKT_BUF_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S3_NON_PKT_BUF_LSB) & MCMN_ISR_S3_NON_PKT_BUF_MASK)
#define MCMN_ISR_S3_NON_PKT_BUF_RESET                                0
#define MCMN_ISR_S3_RXDMA_WD_TOUT_MSB                                1
#define MCMN_ISR_S3_RXDMA_WD_TOUT_LSB                                1
#define MCMN_ISR_S3_RXDMA_WD_TOUT_MASK                               0x00000002
#define MCMN_ISR_S3_RXDMA_WD_TOUT_GET(x)                             (((x) & MCMN_ISR_S3_RXDMA_WD_TOUT_MASK) >> MCMN_ISR_S3_RXDMA_WD_TOUT_LSB)
#define MCMN_ISR_S3_RXDMA_WD_TOUT_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S3_RXDMA_WD_TOUT_LSB) & MCMN_ISR_S3_RXDMA_WD_TOUT_MASK)
#define MCMN_ISR_S3_RXDMA_WD_TOUT_RESET                              0
#define MCMN_ISR_S3_RXDMA_IDLE_MSB                                   0
#define MCMN_ISR_S3_RXDMA_IDLE_LSB                                   0
#define MCMN_ISR_S3_RXDMA_IDLE_MASK                                  0x00000001
#define MCMN_ISR_S3_RXDMA_IDLE_GET(x)                                (((x) & MCMN_ISR_S3_RXDMA_IDLE_MASK) >> MCMN_ISR_S3_RXDMA_IDLE_LSB)
#define MCMN_ISR_S3_RXDMA_IDLE_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S3_RXDMA_IDLE_LSB) & MCMN_ISR_S3_RXDMA_IDLE_MASK)
#define MCMN_ISR_S3_RXDMA_IDLE_RESET                                 0
#define MCMN_ISR_S3_ADDRESS                                          0x000010
#define MCMN_ISR_S3_HW_MASK                                          0x0001ffff
#define MCMN_ISR_S3_SW_MASK                                          0x0001ffff
#define MCMN_ISR_S3_RSTMASK                                          0x00000000
#define MCMN_ISR_S3_RESET                                            0x00000000

// 0x0014 (MCMN_ISR_S4)
#define MCMN_ISR_S4_TXPCU_WD_TOUT_MSB                                4
#define MCMN_ISR_S4_TXPCU_WD_TOUT_LSB                                4
#define MCMN_ISR_S4_TXPCU_WD_TOUT_MASK                               0x00000010
#define MCMN_ISR_S4_TXPCU_WD_TOUT_GET(x)                             (((x) & MCMN_ISR_S4_TXPCU_WD_TOUT_MASK) >> MCMN_ISR_S4_TXPCU_WD_TOUT_LSB)
#define MCMN_ISR_S4_TXPCU_WD_TOUT_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S4_TXPCU_WD_TOUT_LSB) & MCMN_ISR_S4_TXPCU_WD_TOUT_MASK)
#define MCMN_ISR_S4_TXPCU_WD_TOUT_RESET                              0
#define MCMN_ISR_S4_BFEE_CV_TOUT_MSB                                 3
#define MCMN_ISR_S4_BFEE_CV_TOUT_LSB                                 3
#define MCMN_ISR_S4_BFEE_CV_TOUT_MASK                                0x00000008
#define MCMN_ISR_S4_BFEE_CV_TOUT_GET(x)                              (((x) & MCMN_ISR_S4_BFEE_CV_TOUT_MASK) >> MCMN_ISR_S4_BFEE_CV_TOUT_LSB)
#define MCMN_ISR_S4_BFEE_CV_TOUT_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S4_BFEE_CV_TOUT_LSB) & MCMN_ISR_S4_BFEE_CV_TOUT_MASK)
#define MCMN_ISR_S4_BFEE_CV_TOUT_RESET                               0
#define MCMN_ISR_S4_DATA_URUN_MSB                                    2
#define MCMN_ISR_S4_DATA_URUN_LSB                                    2
#define MCMN_ISR_S4_DATA_URUN_MASK                                   0x00000004
#define MCMN_ISR_S4_DATA_URUN_GET(x)                                 (((x) & MCMN_ISR_S4_DATA_URUN_MASK) >> MCMN_ISR_S4_DATA_URUN_LSB)
#define MCMN_ISR_S4_DATA_URUN_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S4_DATA_URUN_LSB) & MCMN_ISR_S4_DATA_URUN_MASK)
#define MCMN_ISR_S4_DATA_URUN_RESET                                  0
#define MCMN_ISR_S4_RECD_GT_MPDU_MSB                                 1
#define MCMN_ISR_S4_RECD_GT_MPDU_LSB                                 1
#define MCMN_ISR_S4_RECD_GT_MPDU_MASK                                0x00000002
#define MCMN_ISR_S4_RECD_GT_MPDU_GET(x)                              (((x) & MCMN_ISR_S4_RECD_GT_MPDU_MASK) >> MCMN_ISR_S4_RECD_GT_MPDU_LSB)
#define MCMN_ISR_S4_RECD_GT_MPDU_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S4_RECD_GT_MPDU_LSB) & MCMN_ISR_S4_RECD_GT_MPDU_MASK)
#define MCMN_ISR_S4_RECD_GT_MPDU_RESET                               0
#define MCMN_ISR_S4_RECD_LT_MPDU_MSB                                 0
#define MCMN_ISR_S4_RECD_LT_MPDU_LSB                                 0
#define MCMN_ISR_S4_RECD_LT_MPDU_MASK                                0x00000001
#define MCMN_ISR_S4_RECD_LT_MPDU_GET(x)                              (((x) & MCMN_ISR_S4_RECD_LT_MPDU_MASK) >> MCMN_ISR_S4_RECD_LT_MPDU_LSB)
#define MCMN_ISR_S4_RECD_LT_MPDU_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S4_RECD_LT_MPDU_LSB) & MCMN_ISR_S4_RECD_LT_MPDU_MASK)
#define MCMN_ISR_S4_RECD_LT_MPDU_RESET                               0
#define MCMN_ISR_S4_ADDRESS                                          0x000014
#define MCMN_ISR_S4_HW_MASK                                          0x0000001f
#define MCMN_ISR_S4_SW_MASK                                          0x0000001f
#define MCMN_ISR_S4_RSTMASK                                          0x00000000
#define MCMN_ISR_S4_RESET                                            0x00000000

// 0x0018 (MCMN_ISR_S5)
#define MCMN_ISR_S5_RX_OVFL_MSB                                      13
#define MCMN_ISR_S5_RX_OVFL_LSB                                      13
#define MCMN_ISR_S5_RX_OVFL_MASK                                     0x00002000
#define MCMN_ISR_S5_RX_OVFL_GET(x)                                   (((x) & MCMN_ISR_S5_RX_OVFL_MASK) >> MCMN_ISR_S5_RX_OVFL_LSB)
#define MCMN_ISR_S5_RX_OVFL_SET(x)                                   (((0x0 | (x)) << MCMN_ISR_S5_RX_OVFL_LSB) & MCMN_ISR_S5_RX_OVFL_MASK)
#define MCMN_ISR_S5_RX_OVFL_RESET                                    0
#define MCMN_ISR_S5_CONFIG_PHY_ERR_MSB                               12
#define MCMN_ISR_S5_CONFIG_PHY_ERR_LSB                               12
#define MCMN_ISR_S5_CONFIG_PHY_ERR_MASK                              0x00001000
#define MCMN_ISR_S5_CONFIG_PHY_ERR_GET(x)                            (((x) & MCMN_ISR_S5_CONFIG_PHY_ERR_MASK) >> MCMN_ISR_S5_CONFIG_PHY_ERR_LSB)
#define MCMN_ISR_S5_CONFIG_PHY_ERR_SET(x)                            (((0x0 | (x)) << MCMN_ISR_S5_CONFIG_PHY_ERR_LSB) & MCMN_ISR_S5_CONFIG_PHY_ERR_MASK)
#define MCMN_ISR_S5_CONFIG_PHY_ERR_RESET                             0
#define MCMN_ISR_S5_RX_DTIM_MSB                                      11
#define MCMN_ISR_S5_RX_DTIM_LSB                                      10
#define MCMN_ISR_S5_RX_DTIM_MASK                                     0x00000c00
#define MCMN_ISR_S5_RX_DTIM_GET(x)                                   (((x) & MCMN_ISR_S5_RX_DTIM_MASK) >> MCMN_ISR_S5_RX_DTIM_LSB)
#define MCMN_ISR_S5_RX_DTIM_SET(x)                                   (((0x0 | (x)) << MCMN_ISR_S5_RX_DTIM_LSB) & MCMN_ISR_S5_RX_DTIM_MASK)
#define MCMN_ISR_S5_RX_DTIM_RESET                                    0
#define MCMN_ISR_S5_RX_TIM_MSB                                       9
#define MCMN_ISR_S5_RX_TIM_LSB                                       8
#define MCMN_ISR_S5_RX_TIM_MASK                                      0x00000300
#define MCMN_ISR_S5_RX_TIM_GET(x)                                    (((x) & MCMN_ISR_S5_RX_TIM_MASK) >> MCMN_ISR_S5_RX_TIM_LSB)
#define MCMN_ISR_S5_RX_TIM_SET(x)                                    (((0x0 | (x)) << MCMN_ISR_S5_RX_TIM_LSB) & MCMN_ISR_S5_RX_TIM_MASK)
#define MCMN_ISR_S5_RX_TIM_RESET                                     0
#define MCMN_ISR_S5_RX_NOTIM_MSB                                     7
#define MCMN_ISR_S5_RX_NOTIM_LSB                                     6
#define MCMN_ISR_S5_RX_NOTIM_MASK                                    0x000000c0
#define MCMN_ISR_S5_RX_NOTIM_GET(x)                                  (((x) & MCMN_ISR_S5_RX_NOTIM_MASK) >> MCMN_ISR_S5_RX_NOTIM_LSB)
#define MCMN_ISR_S5_RX_NOTIM_SET(x)                                  (((0x0 | (x)) << MCMN_ISR_S5_RX_NOTIM_LSB) & MCMN_ISR_S5_RX_NOTIM_MASK)
#define MCMN_ISR_S5_RX_NOTIM_RESET                                   0
#define MCMN_ISR_S5_RSSI_LOW_MSB                                     5
#define MCMN_ISR_S5_RSSI_LOW_LSB                                     4
#define MCMN_ISR_S5_RSSI_LOW_MASK                                    0x00000030
#define MCMN_ISR_S5_RSSI_LOW_GET(x)                                  (((x) & MCMN_ISR_S5_RSSI_LOW_MASK) >> MCMN_ISR_S5_RSSI_LOW_LSB)
#define MCMN_ISR_S5_RSSI_LOW_SET(x)                                  (((0x0 | (x)) << MCMN_ISR_S5_RSSI_LOW_LSB) & MCMN_ISR_S5_RSSI_LOW_MASK)
#define MCMN_ISR_S5_RSSI_LOW_RESET                                   0
#define MCMN_ISR_S5_RSSI_HIGH_MSB                                    3
#define MCMN_ISR_S5_RSSI_HIGH_LSB                                    2
#define MCMN_ISR_S5_RSSI_HIGH_MASK                                   0x0000000c
#define MCMN_ISR_S5_RSSI_HIGH_GET(x)                                 (((x) & MCMN_ISR_S5_RSSI_HIGH_MASK) >> MCMN_ISR_S5_RSSI_HIGH_LSB)
#define MCMN_ISR_S5_RSSI_HIGH_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S5_RSSI_HIGH_LSB) & MCMN_ISR_S5_RSSI_HIGH_MASK)
#define MCMN_ISR_S5_RSSI_HIGH_RESET                                  0
#define MCMN_ISR_S5_BCON_MISS_MSB                                    1
#define MCMN_ISR_S5_BCON_MISS_LSB                                    0
#define MCMN_ISR_S5_BCON_MISS_MASK                                   0x00000003
#define MCMN_ISR_S5_BCON_MISS_GET(x)                                 (((x) & MCMN_ISR_S5_BCON_MISS_MASK) >> MCMN_ISR_S5_BCON_MISS_LSB)
#define MCMN_ISR_S5_BCON_MISS_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S5_BCON_MISS_LSB) & MCMN_ISR_S5_BCON_MISS_MASK)
#define MCMN_ISR_S5_BCON_MISS_RESET                                  0
#define MCMN_ISR_S5_ADDRESS                                          0x000018
#define MCMN_ISR_S5_HW_MASK                                          0x00003fff
#define MCMN_ISR_S5_SW_MASK                                          0x00003fff
#define MCMN_ISR_S5_RSTMASK                                          0x00000000
#define MCMN_ISR_S5_RESET                                            0x00000000

// 0x001c (MCMN_ISR_S6)
#define MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_MSB                            4
#define MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_LSB                            4
#define MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_MASK                           0x00000010
#define MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_GET(x)                         (((x) & MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_MASK) >> MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_LSB)
#define MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_SET(x)                         (((0x0 | (x)) << MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_LSB) & MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_MASK)
#define MCMN_ISR_S6_TXDMA_PDG_LEN_ERR_RESET                          0
#define MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_MSB                         3
#define MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_LSB                         3
#define MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_MASK                        0x00000008
#define MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_GET(x)                      (((x) & MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_MASK) >> MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_LSB)
#define MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_SET(x)                      (((0x0 | (x)) << MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_LSB) & MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_MASK)
#define MCMN_ISR_S6_QMGR_PDG_TLV_LEN_ERR_RESET                       0
#define MCMN_ISR_S6_WRNG_TLV_ORD_MSB                                 2
#define MCMN_ISR_S6_WRNG_TLV_ORD_LSB                                 2
#define MCMN_ISR_S6_WRNG_TLV_ORD_MASK                                0x00000004
#define MCMN_ISR_S6_WRNG_TLV_ORD_GET(x)                              (((x) & MCMN_ISR_S6_WRNG_TLV_ORD_MASK) >> MCMN_ISR_S6_WRNG_TLV_ORD_LSB)
#define MCMN_ISR_S6_WRNG_TLV_ORD_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S6_WRNG_TLV_ORD_LSB) & MCMN_ISR_S6_WRNG_TLV_ORD_MASK)
#define MCMN_ISR_S6_WRNG_TLV_ORD_RESET                               0
#define MCMN_ISR_S6_RCV_FCS_BSY_MSB                                  1
#define MCMN_ISR_S6_RCV_FCS_BSY_LSB                                  1
#define MCMN_ISR_S6_RCV_FCS_BSY_MASK                                 0x00000002
#define MCMN_ISR_S6_RCV_FCS_BSY_GET(x)                               (((x) & MCMN_ISR_S6_RCV_FCS_BSY_MASK) >> MCMN_ISR_S6_RCV_FCS_BSY_LSB)
#define MCMN_ISR_S6_RCV_FCS_BSY_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S6_RCV_FCS_BSY_LSB) & MCMN_ISR_S6_RCV_FCS_BSY_MASK)
#define MCMN_ISR_S6_RCV_FCS_BSY_RESET                                0
#define MCMN_ISR_S6_PDG_WD_TOUT_MSB                                  0
#define MCMN_ISR_S6_PDG_WD_TOUT_LSB                                  0
#define MCMN_ISR_S6_PDG_WD_TOUT_MASK                                 0x00000001
#define MCMN_ISR_S6_PDG_WD_TOUT_GET(x)                               (((x) & MCMN_ISR_S6_PDG_WD_TOUT_MASK) >> MCMN_ISR_S6_PDG_WD_TOUT_LSB)
#define MCMN_ISR_S6_PDG_WD_TOUT_SET(x)                               (((0x0 | (x)) << MCMN_ISR_S6_PDG_WD_TOUT_LSB) & MCMN_ISR_S6_PDG_WD_TOUT_MASK)
#define MCMN_ISR_S6_PDG_WD_TOUT_RESET                                0
#define MCMN_ISR_S6_ADDRESS                                          0x00001c
#define MCMN_ISR_S6_HW_MASK                                          0x0000001f
#define MCMN_ISR_S6_SW_MASK                                          0x0000001f
#define MCMN_ISR_S6_RSTMASK                                          0x00000000
#define MCMN_ISR_S6_RESET                                            0x00000000

// 0x0020 (MCMN_ISR_S7)
#define MCMN_ISR_S7_HW_RNG_THR_MSB                                   31
#define MCMN_ISR_S7_HW_RNG_THR_LSB                                   16
#define MCMN_ISR_S7_HW_RNG_THR_MASK                                  0xffff0000
#define MCMN_ISR_S7_HW_RNG_THR_GET(x)                                (((x) & MCMN_ISR_S7_HW_RNG_THR_MASK) >> MCMN_ISR_S7_HW_RNG_THR_LSB)
#define MCMN_ISR_S7_HW_RNG_THR_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S7_HW_RNG_THR_LSB) & MCMN_ISR_S7_HW_RNG_THR_MASK)
#define MCMN_ISR_S7_HW_RNG_THR_RESET                                 0
#define MCMN_ISR_S7_HW_RNG_PSD_MSB                                   15
#define MCMN_ISR_S7_HW_RNG_PSD_LSB                                   0
#define MCMN_ISR_S7_HW_RNG_PSD_MASK                                  0x0000ffff
#define MCMN_ISR_S7_HW_RNG_PSD_GET(x)                                (((x) & MCMN_ISR_S7_HW_RNG_PSD_MASK) >> MCMN_ISR_S7_HW_RNG_PSD_LSB)
#define MCMN_ISR_S7_HW_RNG_PSD_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S7_HW_RNG_PSD_LSB) & MCMN_ISR_S7_HW_RNG_PSD_MASK)
#define MCMN_ISR_S7_HW_RNG_PSD_RESET                                 0
#define MCMN_ISR_S7_ADDRESS                                          0x000020
#define MCMN_ISR_S7_HW_MASK                                          0xffffffff
#define MCMN_ISR_S7_SW_MASK                                          0xffffffff
#define MCMN_ISR_S7_RSTMASK                                          0x00000000
#define MCMN_ISR_S7_RESET                                            0x00000000

// 0x0024 (MCMN_ISR_S8)
#define MCMN_ISR_S8_MTU_TIMER_MSB                                    30
#define MCMN_ISR_S8_MTU_TIMER_LSB                                    19
#define MCMN_ISR_S8_MTU_TIMER_MASK                                   0x7ff80000
#define MCMN_ISR_S8_MTU_TIMER_GET(x)                                 (((x) & MCMN_ISR_S8_MTU_TIMER_MASK) >> MCMN_ISR_S8_MTU_TIMER_LSB)
#define MCMN_ISR_S8_MTU_TIMER_SET(x)                                 (((0x0 | (x)) << MCMN_ISR_S8_MTU_TIMER_LSB) & MCMN_ISR_S8_MTU_TIMER_MASK)
#define MCMN_ISR_S8_MTU_TIMER_RESET                                  0
#define MCMN_ISR_S8_MTU_TSF_MSB                                      18
#define MCMN_ISR_S8_MTU_TSF_LSB                                      16
#define MCMN_ISR_S8_MTU_TSF_MASK                                     0x00070000
#define MCMN_ISR_S8_MTU_TSF_GET(x)                                   (((x) & MCMN_ISR_S8_MTU_TSF_MASK) >> MCMN_ISR_S8_MTU_TSF_LSB)
#define MCMN_ISR_S8_MTU_TSF_SET(x)                                   (((0x0 | (x)) << MCMN_ISR_S8_MTU_TSF_LSB) & MCMN_ISR_S8_MTU_TSF_MASK)
#define MCMN_ISR_S8_MTU_TSF_RESET                                    0
#define MCMN_ISR_S8_MTU_BKOF_MSB                                     15
#define MCMN_ISR_S8_MTU_BKOF_LSB                                     0
#define MCMN_ISR_S8_MTU_BKOF_MASK                                    0x0000ffff
#define MCMN_ISR_S8_MTU_BKOF_GET(x)                                  (((x) & MCMN_ISR_S8_MTU_BKOF_MASK) >> MCMN_ISR_S8_MTU_BKOF_LSB)
#define MCMN_ISR_S8_MTU_BKOF_SET(x)                                  (((0x0 | (x)) << MCMN_ISR_S8_MTU_BKOF_LSB) & MCMN_ISR_S8_MTU_BKOF_MASK)
#define MCMN_ISR_S8_MTU_BKOF_RESET                                   0
#define MCMN_ISR_S8_ADDRESS                                          0x000024
#define MCMN_ISR_S8_HW_MASK                                          0x7fffffff
#define MCMN_ISR_S8_SW_MASK                                          0x7fffffff
#define MCMN_ISR_S8_RSTMASK                                          0x00000000
#define MCMN_ISR_S8_RESET                                            0x00000000

// 0x0028 (MCMN_ISR_S9)
#define MCMN_ISR_S9_MTU_ERR_MSB                                      7
#define MCMN_ISR_S9_MTU_ERR_LSB                                      7
#define MCMN_ISR_S9_MTU_ERR_MASK                                     0x00000080
#define MCMN_ISR_S9_MTU_ERR_GET(x)                                   (((x) & MCMN_ISR_S9_MTU_ERR_MASK) >> MCMN_ISR_S9_MTU_ERR_LSB)
#define MCMN_ISR_S9_MTU_ERR_SET(x)                                   (((0x0 | (x)) << MCMN_ISR_S9_MTU_ERR_LSB) & MCMN_ISR_S9_MTU_ERR_MASK)
#define MCMN_ISR_S9_MTU_ERR_RESET                                    0
#define MCMN_ISR_S9_MTU_TX_BOUND_MSB                                 6
#define MCMN_ISR_S9_MTU_TX_BOUND_LSB                                 6
#define MCMN_ISR_S9_MTU_TX_BOUND_MASK                                0x00000040
#define MCMN_ISR_S9_MTU_TX_BOUND_GET(x)                              (((x) & MCMN_ISR_S9_MTU_TX_BOUND_MASK) >> MCMN_ISR_S9_MTU_TX_BOUND_LSB)
#define MCMN_ISR_S9_MTU_TX_BOUND_SET(x)                              (((0x0 | (x)) << MCMN_ISR_S9_MTU_TX_BOUND_LSB) & MCMN_ISR_S9_MTU_TX_BOUND_MASK)
#define MCMN_ISR_S9_MTU_TX_BOUND_RESET                               0
#define MCMN_ISR_S9_TLV_STS_UPDATE_MSB                               5
#define MCMN_ISR_S9_TLV_STS_UPDATE_LSB                               5
#define MCMN_ISR_S9_TLV_STS_UPDATE_MASK                              0x00000020
#define MCMN_ISR_S9_TLV_STS_UPDATE_GET(x)                            (((x) & MCMN_ISR_S9_TLV_STS_UPDATE_MASK) >> MCMN_ISR_S9_TLV_STS_UPDATE_LSB)
#define MCMN_ISR_S9_TLV_STS_UPDATE_SET(x)                            (((0x0 | (x)) << MCMN_ISR_S9_TLV_STS_UPDATE_LSB) & MCMN_ISR_S9_TLV_STS_UPDATE_MASK)
#define MCMN_ISR_S9_TLV_STS_UPDATE_RESET                             0
#define MCMN_ISR_S9_TLV_SETUP_ERR_MSB                                4
#define MCMN_ISR_S9_TLV_SETUP_ERR_LSB                                4
#define MCMN_ISR_S9_TLV_SETUP_ERR_MASK                               0x00000010
#define MCMN_ISR_S9_TLV_SETUP_ERR_GET(x)                             (((x) & MCMN_ISR_S9_TLV_SETUP_ERR_MASK) >> MCMN_ISR_S9_TLV_SETUP_ERR_LSB)
#define MCMN_ISR_S9_TLV_SETUP_ERR_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S9_TLV_SETUP_ERR_LSB) & MCMN_ISR_S9_TLV_SETUP_ERR_MASK)
#define MCMN_ISR_S9_TLV_SETUP_ERR_RESET                              0
#define MCMN_ISR_S9_FES_TLV_FLUSH_MSB                                3
#define MCMN_ISR_S9_FES_TLV_FLUSH_LSB                                3
#define MCMN_ISR_S9_FES_TLV_FLUSH_MASK                               0x00000008
#define MCMN_ISR_S9_FES_TLV_FLUSH_GET(x)                             (((x) & MCMN_ISR_S9_FES_TLV_FLUSH_MASK) >> MCMN_ISR_S9_FES_TLV_FLUSH_LSB)
#define MCMN_ISR_S9_FES_TLV_FLUSH_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S9_FES_TLV_FLUSH_LSB) & MCMN_ISR_S9_FES_TLV_FLUSH_MASK)
#define MCMN_ISR_S9_FES_TLV_FLUSH_RESET                              0
#define MCMN_ISR_S9_AXI_RD_ERR_MSB                                   2
#define MCMN_ISR_S9_AXI_RD_ERR_LSB                                   2
#define MCMN_ISR_S9_AXI_RD_ERR_MASK                                  0x00000004
#define MCMN_ISR_S9_AXI_RD_ERR_GET(x)                                (((x) & MCMN_ISR_S9_AXI_RD_ERR_MASK) >> MCMN_ISR_S9_AXI_RD_ERR_LSB)
#define MCMN_ISR_S9_AXI_RD_ERR_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S9_AXI_RD_ERR_LSB) & MCMN_ISR_S9_AXI_RD_ERR_MASK)
#define MCMN_ISR_S9_AXI_RD_ERR_RESET                                 0
#define MCMN_ISR_S9_AXI_WR_ERR_MSB                                   1
#define MCMN_ISR_S9_AXI_WR_ERR_LSB                                   1
#define MCMN_ISR_S9_AXI_WR_ERR_MASK                                  0x00000002
#define MCMN_ISR_S9_AXI_WR_ERR_GET(x)                                (((x) & MCMN_ISR_S9_AXI_WR_ERR_MASK) >> MCMN_ISR_S9_AXI_WR_ERR_LSB)
#define MCMN_ISR_S9_AXI_WR_ERR_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S9_AXI_WR_ERR_LSB) & MCMN_ISR_S9_AXI_WR_ERR_MASK)
#define MCMN_ISR_S9_AXI_WR_ERR_RESET                                 0
#define MCMN_ISR_S9_HWSCH_WD_TOUT_MSB                                0
#define MCMN_ISR_S9_HWSCH_WD_TOUT_LSB                                0
#define MCMN_ISR_S9_HWSCH_WD_TOUT_MASK                               0x00000001
#define MCMN_ISR_S9_HWSCH_WD_TOUT_GET(x)                             (((x) & MCMN_ISR_S9_HWSCH_WD_TOUT_MASK) >> MCMN_ISR_S9_HWSCH_WD_TOUT_LSB)
#define MCMN_ISR_S9_HWSCH_WD_TOUT_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S9_HWSCH_WD_TOUT_LSB) & MCMN_ISR_S9_HWSCH_WD_TOUT_MASK)
#define MCMN_ISR_S9_HWSCH_WD_TOUT_RESET                              0
#define MCMN_ISR_S9_ADDRESS                                          0x000028
#define MCMN_ISR_S9_HW_MASK                                          0x000000ff
#define MCMN_ISR_S9_SW_MASK                                          0x000000ff
#define MCMN_ISR_S9_RSTMASK                                          0x00000000
#define MCMN_ISR_S9_RESET                                            0x00000000

// 0x002c (MCMN_ISR_S10)
#define MCMN_ISR_S10_AXI_TMOUT_MSB                                   1
#define MCMN_ISR_S10_AXI_TMOUT_LSB                                   1
#define MCMN_ISR_S10_AXI_TMOUT_MASK                                  0x00000002
#define MCMN_ISR_S10_AXI_TMOUT_GET(x)                                (((x) & MCMN_ISR_S10_AXI_TMOUT_MASK) >> MCMN_ISR_S10_AXI_TMOUT_LSB)
#define MCMN_ISR_S10_AXI_TMOUT_SET(x)                                (((0x0 | (x)) << MCMN_ISR_S10_AXI_TMOUT_LSB) & MCMN_ISR_S10_AXI_TMOUT_MASK)
#define MCMN_ISR_S10_AXI_TMOUT_RESET                                 0
#define MCMN_ISR_S10_AXI_RESP_ERR_MSB                                0
#define MCMN_ISR_S10_AXI_RESP_ERR_LSB                                0
#define MCMN_ISR_S10_AXI_RESP_ERR_MASK                               0x00000001
#define MCMN_ISR_S10_AXI_RESP_ERR_GET(x)                             (((x) & MCMN_ISR_S10_AXI_RESP_ERR_MASK) >> MCMN_ISR_S10_AXI_RESP_ERR_LSB)
#define MCMN_ISR_S10_AXI_RESP_ERR_SET(x)                             (((0x0 | (x)) << MCMN_ISR_S10_AXI_RESP_ERR_LSB) & MCMN_ISR_S10_AXI_RESP_ERR_MASK)
#define MCMN_ISR_S10_AXI_RESP_ERR_RESET                              0
#define MCMN_ISR_S10_ADDRESS                                         0x00002c
#define MCMN_ISR_S10_HW_MASK                                         0x00000003
#define MCMN_ISR_S10_SW_MASK                                         0x00000003
#define MCMN_ISR_S10_RSTMASK                                         0x00000000
#define MCMN_ISR_S10_RESET                                           0x00000000

// 0x0030 (MCMN_IMR_P)
#define MCMN_IMR_P_MXI_MSB                                           10
#define MCMN_IMR_P_MXI_LSB                                           10
#define MCMN_IMR_P_MXI_MASK                                          0x00000400
#define MCMN_IMR_P_MXI_GET(x)                                        (((x) & MCMN_IMR_P_MXI_MASK) >> MCMN_IMR_P_MXI_LSB)
#define MCMN_IMR_P_MXI_SET(x)                                        (((0x0 | (x)) << MCMN_IMR_P_MXI_LSB) & MCMN_IMR_P_MXI_MASK)
#define MCMN_IMR_P_MXI_RESET                                         0
#define MCMN_IMR_P_HWSCH2_MSB                                        9
#define MCMN_IMR_P_HWSCH2_LSB                                        9
#define MCMN_IMR_P_HWSCH2_MASK                                       0x00000200
#define MCMN_IMR_P_HWSCH2_GET(x)                                     (((x) & MCMN_IMR_P_HWSCH2_MASK) >> MCMN_IMR_P_HWSCH2_LSB)
#define MCMN_IMR_P_HWSCH2_SET(x)                                     (((0x0 | (x)) << MCMN_IMR_P_HWSCH2_LSB) & MCMN_IMR_P_HWSCH2_MASK)
#define MCMN_IMR_P_HWSCH2_RESET                                      0
#define MCMN_IMR_P_HWSCH1_MSB                                        8
#define MCMN_IMR_P_HWSCH1_LSB                                        8
#define MCMN_IMR_P_HWSCH1_MASK                                       0x00000100
#define MCMN_IMR_P_HWSCH1_GET(x)                                     (((x) & MCMN_IMR_P_HWSCH1_MASK) >> MCMN_IMR_P_HWSCH1_LSB)
#define MCMN_IMR_P_HWSCH1_SET(x)                                     (((0x0 | (x)) << MCMN_IMR_P_HWSCH1_LSB) & MCMN_IMR_P_HWSCH1_MASK)
#define MCMN_IMR_P_HWSCH1_RESET                                      0
#define MCMN_IMR_P_HWSCH0_MSB                                        7
#define MCMN_IMR_P_HWSCH0_LSB                                        7
#define MCMN_IMR_P_HWSCH0_MASK                                       0x00000080
#define MCMN_IMR_P_HWSCH0_GET(x)                                     (((x) & MCMN_IMR_P_HWSCH0_MASK) >> MCMN_IMR_P_HWSCH0_LSB)
#define MCMN_IMR_P_HWSCH0_SET(x)                                     (((0x0 | (x)) << MCMN_IMR_P_HWSCH0_LSB) & MCMN_IMR_P_HWSCH0_MASK)
#define MCMN_IMR_P_HWSCH0_RESET                                      0
#define MCMN_IMR_P_PDG_MSB                                           6
#define MCMN_IMR_P_PDG_LSB                                           6
#define MCMN_IMR_P_PDG_MASK                                          0x00000040
#define MCMN_IMR_P_PDG_GET(x)                                        (((x) & MCMN_IMR_P_PDG_MASK) >> MCMN_IMR_P_PDG_LSB)
#define MCMN_IMR_P_PDG_SET(x)                                        (((0x0 | (x)) << MCMN_IMR_P_PDG_LSB) & MCMN_IMR_P_PDG_MASK)
#define MCMN_IMR_P_PDG_RESET                                         0
#define MCMN_IMR_P_RXPCU_MSB                                         5
#define MCMN_IMR_P_RXPCU_LSB                                         5
#define MCMN_IMR_P_RXPCU_MASK                                        0x00000020
#define MCMN_IMR_P_RXPCU_GET(x)                                      (((x) & MCMN_IMR_P_RXPCU_MASK) >> MCMN_IMR_P_RXPCU_LSB)
#define MCMN_IMR_P_RXPCU_SET(x)                                      (((0x0 | (x)) << MCMN_IMR_P_RXPCU_LSB) & MCMN_IMR_P_RXPCU_MASK)
#define MCMN_IMR_P_RXPCU_RESET                                       0
#define MCMN_IMR_P_TXPCU_MSB                                         4
#define MCMN_IMR_P_TXPCU_LSB                                         4
#define MCMN_IMR_P_TXPCU_MASK                                        0x00000010
#define MCMN_IMR_P_TXPCU_GET(x)                                      (((x) & MCMN_IMR_P_TXPCU_MASK) >> MCMN_IMR_P_TXPCU_LSB)
#define MCMN_IMR_P_TXPCU_SET(x)                                      (((0x0 | (x)) << MCMN_IMR_P_TXPCU_LSB) & MCMN_IMR_P_TXPCU_MASK)
#define MCMN_IMR_P_TXPCU_RESET                                       0
#define MCMN_IMR_P_RXDMA_MSB                                         3
#define MCMN_IMR_P_RXDMA_LSB                                         3
#define MCMN_IMR_P_RXDMA_MASK                                        0x00000008
#define MCMN_IMR_P_RXDMA_GET(x)                                      (((x) & MCMN_IMR_P_RXDMA_MASK) >> MCMN_IMR_P_RXDMA_LSB)
#define MCMN_IMR_P_RXDMA_SET(x)                                      (((0x0 | (x)) << MCMN_IMR_P_RXDMA_LSB) & MCMN_IMR_P_RXDMA_MASK)
#define MCMN_IMR_P_RXDMA_RESET                                       0
#define MCMN_IMR_P_TXDMA_MSB                                         2
#define MCMN_IMR_P_TXDMA_LSB                                         2
#define MCMN_IMR_P_TXDMA_MASK                                        0x00000004
#define MCMN_IMR_P_TXDMA_GET(x)                                      (((x) & MCMN_IMR_P_TXDMA_MASK) >> MCMN_IMR_P_TXDMA_LSB)
#define MCMN_IMR_P_TXDMA_SET(x)                                      (((0x0 | (x)) << MCMN_IMR_P_TXDMA_LSB) & MCMN_IMR_P_TXDMA_MASK)
#define MCMN_IMR_P_TXDMA_RESET                                       0
#define MCMN_IMR_P_CRYPTO_MSB                                        1
#define MCMN_IMR_P_CRYPTO_LSB                                        1
#define MCMN_IMR_P_CRYPTO_MASK                                       0x00000002
#define MCMN_IMR_P_CRYPTO_GET(x)                                     (((x) & MCMN_IMR_P_CRYPTO_MASK) >> MCMN_IMR_P_CRYPTO_LSB)
#define MCMN_IMR_P_CRYPTO_SET(x)                                     (((0x0 | (x)) << MCMN_IMR_P_CRYPTO_LSB) & MCMN_IMR_P_CRYPTO_MASK)
#define MCMN_IMR_P_CRYPTO_RESET                                      0
#define MCMN_IMR_P_OLE_MSB                                           0
#define MCMN_IMR_P_OLE_LSB                                           0
#define MCMN_IMR_P_OLE_MASK                                          0x00000001
#define MCMN_IMR_P_OLE_GET(x)                                        (((x) & MCMN_IMR_P_OLE_MASK) >> MCMN_IMR_P_OLE_LSB)
#define MCMN_IMR_P_OLE_SET(x)                                        (((0x0 | (x)) << MCMN_IMR_P_OLE_LSB) & MCMN_IMR_P_OLE_MASK)
#define MCMN_IMR_P_OLE_RESET                                         0
#define MCMN_IMR_P_ADDRESS                                           0x000030
#define MCMN_IMR_P_HW_MASK                                           0x000007ff
#define MCMN_IMR_P_SW_MASK                                           0x000007ff
#define MCMN_IMR_P_RSTMASK                                           0x00000000
#define MCMN_IMR_P_RESET                                             0x00000000

// 0x0034 (MCMN_IMR_S0)
#define MCMN_IMR_S0_RX_WMAC_ERR_MSB                                  13
#define MCMN_IMR_S0_RX_WMAC_ERR_LSB                                  13
#define MCMN_IMR_S0_RX_WMAC_ERR_MASK                                 0x00002000
#define MCMN_IMR_S0_RX_WMAC_ERR_GET(x)                               (((x) & MCMN_IMR_S0_RX_WMAC_ERR_MASK) >> MCMN_IMR_S0_RX_WMAC_ERR_LSB)
#define MCMN_IMR_S0_RX_WMAC_ERR_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S0_RX_WMAC_ERR_LSB) & MCMN_IMR_S0_RX_WMAC_ERR_MASK)
#define MCMN_IMR_S0_RX_WMAC_ERR_RESET                                0
#define MCMN_IMR_S0_RX_ETH_ERR_MSB                                   12
#define MCMN_IMR_S0_RX_ETH_ERR_LSB                                   12
#define MCMN_IMR_S0_RX_ETH_ERR_MASK                                  0x00001000
#define MCMN_IMR_S0_RX_ETH_ERR_GET(x)                                (((x) & MCMN_IMR_S0_RX_ETH_ERR_MASK) >> MCMN_IMR_S0_RX_ETH_ERR_LSB)
#define MCMN_IMR_S0_RX_ETH_ERR_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S0_RX_ETH_ERR_LSB) & MCMN_IMR_S0_RX_ETH_ERR_MASK)
#define MCMN_IMR_S0_RX_ETH_ERR_RESET                                 0
#define MCMN_IMR_S0_RX_MSDU_LEN_MSB                                  11
#define MCMN_IMR_S0_RX_MSDU_LEN_LSB                                  11
#define MCMN_IMR_S0_RX_MSDU_LEN_MASK                                 0x00000800
#define MCMN_IMR_S0_RX_MSDU_LEN_GET(x)                               (((x) & MCMN_IMR_S0_RX_MSDU_LEN_MASK) >> MCMN_IMR_S0_RX_MSDU_LEN_LSB)
#define MCMN_IMR_S0_RX_MSDU_LEN_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S0_RX_MSDU_LEN_LSB) & MCMN_IMR_S0_RX_MSDU_LEN_MASK)
#define MCMN_IMR_S0_RX_MSDU_LEN_RESET                                0
#define MCMN_IMR_S0_TX_ILLEGAL_MSB                                   10
#define MCMN_IMR_S0_TX_ILLEGAL_LSB                                   10
#define MCMN_IMR_S0_TX_ILLEGAL_MASK                                  0x00000400
#define MCMN_IMR_S0_TX_ILLEGAL_GET(x)                                (((x) & MCMN_IMR_S0_TX_ILLEGAL_MASK) >> MCMN_IMR_S0_TX_ILLEGAL_LSB)
#define MCMN_IMR_S0_TX_ILLEGAL_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S0_TX_ILLEGAL_LSB) & MCMN_IMR_S0_TX_ILLEGAL_MASK)
#define MCMN_IMR_S0_TX_ILLEGAL_RESET                                 0
#define MCMN_IMR_S0_TX_UNKN_LLC_MSB                                  9
#define MCMN_IMR_S0_TX_UNKN_LLC_LSB                                  9
#define MCMN_IMR_S0_TX_UNKN_LLC_MASK                                 0x00000200
#define MCMN_IMR_S0_TX_UNKN_LLC_GET(x)                               (((x) & MCMN_IMR_S0_TX_UNKN_LLC_MASK) >> MCMN_IMR_S0_TX_UNKN_LLC_LSB)
#define MCMN_IMR_S0_TX_UNKN_LLC_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S0_TX_UNKN_LLC_LSB) & MCMN_IMR_S0_TX_UNKN_LLC_MASK)
#define MCMN_IMR_S0_TX_UNKN_LLC_RESET                                0
#define MCMN_IMR_S0_TX_VLAN_LLC_MSB                                  8
#define MCMN_IMR_S0_TX_VLAN_LLC_LSB                                  8
#define MCMN_IMR_S0_TX_VLAN_LLC_MASK                                 0x00000100
#define MCMN_IMR_S0_TX_VLAN_LLC_GET(x)                               (((x) & MCMN_IMR_S0_TX_VLAN_LLC_MASK) >> MCMN_IMR_S0_TX_VLAN_LLC_LSB)
#define MCMN_IMR_S0_TX_VLAN_LLC_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S0_TX_VLAN_LLC_LSB) & MCMN_IMR_S0_TX_VLAN_LLC_MASK)
#define MCMN_IMR_S0_TX_VLAN_LLC_RESET                                0
#define MCMN_IMR_S0_WEP_KEY_ERR_MSB                                  7
#define MCMN_IMR_S0_WEP_KEY_ERR_LSB                                  7
#define MCMN_IMR_S0_WEP_KEY_ERR_MASK                                 0x00000080
#define MCMN_IMR_S0_WEP_KEY_ERR_GET(x)                               (((x) & MCMN_IMR_S0_WEP_KEY_ERR_MASK) >> MCMN_IMR_S0_WEP_KEY_ERR_LSB)
#define MCMN_IMR_S0_WEP_KEY_ERR_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S0_WEP_KEY_ERR_LSB) & MCMN_IMR_S0_WEP_KEY_ERR_MASK)
#define MCMN_IMR_S0_WEP_KEY_ERR_RESET                                0
#define MCMN_IMR_S0_TX_SEQ_BM_ERR_MSB                                6
#define MCMN_IMR_S0_TX_SEQ_BM_ERR_LSB                                6
#define MCMN_IMR_S0_TX_SEQ_BM_ERR_MASK                               0x00000040
#define MCMN_IMR_S0_TX_SEQ_BM_ERR_GET(x)                             (((x) & MCMN_IMR_S0_TX_SEQ_BM_ERR_MASK) >> MCMN_IMR_S0_TX_SEQ_BM_ERR_LSB)
#define MCMN_IMR_S0_TX_SEQ_BM_ERR_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S0_TX_SEQ_BM_ERR_LSB) & MCMN_IMR_S0_TX_SEQ_BM_ERR_MASK)
#define MCMN_IMR_S0_TX_SEQ_BM_ERR_RESET                              0
#define MCMN_IMR_S0_AMSDU_FRM_ERR_MSB                                3
#define MCMN_IMR_S0_AMSDU_FRM_ERR_LSB                                3
#define MCMN_IMR_S0_AMSDU_FRM_ERR_MASK                               0x00000008
#define MCMN_IMR_S0_AMSDU_FRM_ERR_GET(x)                             (((x) & MCMN_IMR_S0_AMSDU_FRM_ERR_MASK) >> MCMN_IMR_S0_AMSDU_FRM_ERR_LSB)
#define MCMN_IMR_S0_AMSDU_FRM_ERR_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S0_AMSDU_FRM_ERR_LSB) & MCMN_IMR_S0_AMSDU_FRM_ERR_MASK)
#define MCMN_IMR_S0_AMSDU_FRM_ERR_RESET                              0
#define MCMN_IMR_S0_TX_TLV_ERR_MSB                                   2
#define MCMN_IMR_S0_TX_TLV_ERR_LSB                                   2
#define MCMN_IMR_S0_TX_TLV_ERR_MASK                                  0x00000004
#define MCMN_IMR_S0_TX_TLV_ERR_GET(x)                                (((x) & MCMN_IMR_S0_TX_TLV_ERR_MASK) >> MCMN_IMR_S0_TX_TLV_ERR_LSB)
#define MCMN_IMR_S0_TX_TLV_ERR_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S0_TX_TLV_ERR_LSB) & MCMN_IMR_S0_TX_TLV_ERR_MASK)
#define MCMN_IMR_S0_TX_TLV_ERR_RESET                                 0
#define MCMN_IMR_S0_RX_TLV_ERR_MSB                                   1
#define MCMN_IMR_S0_RX_TLV_ERR_LSB                                   1
#define MCMN_IMR_S0_RX_TLV_ERR_MASK                                  0x00000002
#define MCMN_IMR_S0_RX_TLV_ERR_GET(x)                                (((x) & MCMN_IMR_S0_RX_TLV_ERR_MASK) >> MCMN_IMR_S0_RX_TLV_ERR_LSB)
#define MCMN_IMR_S0_RX_TLV_ERR_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S0_RX_TLV_ERR_LSB) & MCMN_IMR_S0_RX_TLV_ERR_MASK)
#define MCMN_IMR_S0_RX_TLV_ERR_RESET                                 0
#define MCMN_IMR_S0_WDOG_TOUT_MSB                                    0
#define MCMN_IMR_S0_WDOG_TOUT_LSB                                    0
#define MCMN_IMR_S0_WDOG_TOUT_MASK                                   0x00000001
#define MCMN_IMR_S0_WDOG_TOUT_GET(x)                                 (((x) & MCMN_IMR_S0_WDOG_TOUT_MASK) >> MCMN_IMR_S0_WDOG_TOUT_LSB)
#define MCMN_IMR_S0_WDOG_TOUT_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S0_WDOG_TOUT_LSB) & MCMN_IMR_S0_WDOG_TOUT_MASK)
#define MCMN_IMR_S0_WDOG_TOUT_RESET                                  0
#define MCMN_IMR_S0_ADDRESS                                          0x000034
#define MCMN_IMR_S0_HW_MASK                                          0x00003fcf
#define MCMN_IMR_S0_SW_MASK                                          0x00003fcf
#define MCMN_IMR_S0_RSTMASK                                          0x00000000
#define MCMN_IMR_S0_RESET                                            0x00000000

// 0x0038 (MCMN_IMR_S1)
#define MCMN_IMR_S1_RSVD_GT_MPDU_MSB                                 5
#define MCMN_IMR_S1_RSVD_GT_MPDU_LSB                                 5
#define MCMN_IMR_S1_RSVD_GT_MPDU_MASK                                0x00000020
#define MCMN_IMR_S1_RSVD_GT_MPDU_GET(x)                              (((x) & MCMN_IMR_S1_RSVD_GT_MPDU_MASK) >> MCMN_IMR_S1_RSVD_GT_MPDU_LSB)
#define MCMN_IMR_S1_RSVD_GT_MPDU_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S1_RSVD_GT_MPDU_LSB) & MCMN_IMR_S1_RSVD_GT_MPDU_MASK)
#define MCMN_IMR_S1_RSVD_GT_MPDU_RESET                               0
#define MCMN_IMR_S1_RSVD_LT_MPDU_MSB                                 4
#define MCMN_IMR_S1_RSVD_LT_MPDU_LSB                                 4
#define MCMN_IMR_S1_RSVD_LT_MPDU_MASK                                0x00000010
#define MCMN_IMR_S1_RSVD_LT_MPDU_GET(x)                              (((x) & MCMN_IMR_S1_RSVD_LT_MPDU_MASK) >> MCMN_IMR_S1_RSVD_LT_MPDU_LSB)
#define MCMN_IMR_S1_RSVD_LT_MPDU_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S1_RSVD_LT_MPDU_LSB) & MCMN_IMR_S1_RSVD_LT_MPDU_MASK)
#define MCMN_IMR_S1_RSVD_LT_MPDU_RESET                               0
#define MCMN_IMR_S1_DECRPT_ERR_MSB                                   3
#define MCMN_IMR_S1_DECRPT_ERR_LSB                                   3
#define MCMN_IMR_S1_DECRPT_ERR_MASK                                  0x00000008
#define MCMN_IMR_S1_DECRPT_ERR_GET(x)                                (((x) & MCMN_IMR_S1_DECRPT_ERR_MASK) >> MCMN_IMR_S1_DECRPT_ERR_LSB)
#define MCMN_IMR_S1_DECRPT_ERR_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S1_DECRPT_ERR_LSB) & MCMN_IMR_S1_DECRPT_ERR_MASK)
#define MCMN_IMR_S1_DECRPT_ERR_RESET                                 0
#define MCMN_IMR_S1_RX_TLV_OUT_SEQ_MSB                               2
#define MCMN_IMR_S1_RX_TLV_OUT_SEQ_LSB                               2
#define MCMN_IMR_S1_RX_TLV_OUT_SEQ_MASK                              0x00000004
#define MCMN_IMR_S1_RX_TLV_OUT_SEQ_GET(x)                            (((x) & MCMN_IMR_S1_RX_TLV_OUT_SEQ_MASK) >> MCMN_IMR_S1_RX_TLV_OUT_SEQ_LSB)
#define MCMN_IMR_S1_RX_TLV_OUT_SEQ_SET(x)                            (((0x0 | (x)) << MCMN_IMR_S1_RX_TLV_OUT_SEQ_LSB) & MCMN_IMR_S1_RX_TLV_OUT_SEQ_MASK)
#define MCMN_IMR_S1_RX_TLV_OUT_SEQ_RESET                             0
#define MCMN_IMR_S1_TX_TLV_OUT_SEQ_MSB                               1
#define MCMN_IMR_S1_TX_TLV_OUT_SEQ_LSB                               1
#define MCMN_IMR_S1_TX_TLV_OUT_SEQ_MASK                              0x00000002
#define MCMN_IMR_S1_TX_TLV_OUT_SEQ_GET(x)                            (((x) & MCMN_IMR_S1_TX_TLV_OUT_SEQ_MASK) >> MCMN_IMR_S1_TX_TLV_OUT_SEQ_LSB)
#define MCMN_IMR_S1_TX_TLV_OUT_SEQ_SET(x)                            (((0x0 | (x)) << MCMN_IMR_S1_TX_TLV_OUT_SEQ_LSB) & MCMN_IMR_S1_TX_TLV_OUT_SEQ_MASK)
#define MCMN_IMR_S1_TX_TLV_OUT_SEQ_RESET                             0
#define MCMN_IMR_S1_CRYP_WD_TMOUT_MSB                                0
#define MCMN_IMR_S1_CRYP_WD_TMOUT_LSB                                0
#define MCMN_IMR_S1_CRYP_WD_TMOUT_MASK                               0x00000001
#define MCMN_IMR_S1_CRYP_WD_TMOUT_GET(x)                             (((x) & MCMN_IMR_S1_CRYP_WD_TMOUT_MASK) >> MCMN_IMR_S1_CRYP_WD_TMOUT_LSB)
#define MCMN_IMR_S1_CRYP_WD_TMOUT_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S1_CRYP_WD_TMOUT_LSB) & MCMN_IMR_S1_CRYP_WD_TMOUT_MASK)
#define MCMN_IMR_S1_CRYP_WD_TMOUT_RESET                              0
#define MCMN_IMR_S1_ADDRESS                                          0x000038
#define MCMN_IMR_S1_HW_MASK                                          0x0000003f
#define MCMN_IMR_S1_SW_MASK                                          0x0000003f
#define MCMN_IMR_S1_RSTMASK                                          0x00000000
#define MCMN_IMR_S1_RESET                                            0x00000000

// 0x003c (MCMN_IMR_S2)
#define MCMN_IMR_S2_AXI_TMOUT_MSB                                    9
#define MCMN_IMR_S2_AXI_TMOUT_LSB                                    9
#define MCMN_IMR_S2_AXI_TMOUT_MASK                                   0x00000200
#define MCMN_IMR_S2_AXI_TMOUT_GET(x)                                 (((x) & MCMN_IMR_S2_AXI_TMOUT_MASK) >> MCMN_IMR_S2_AXI_TMOUT_LSB)
#define MCMN_IMR_S2_AXI_TMOUT_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S2_AXI_TMOUT_LSB) & MCMN_IMR_S2_AXI_TMOUT_MASK)
#define MCMN_IMR_S2_AXI_TMOUT_RESET                                  0
#define MCMN_IMR_S2_OLE_RDY_TMOUT_MSB                                8
#define MCMN_IMR_S2_OLE_RDY_TMOUT_LSB                                8
#define MCMN_IMR_S2_OLE_RDY_TMOUT_MASK                               0x00000100
#define MCMN_IMR_S2_OLE_RDY_TMOUT_GET(x)                             (((x) & MCMN_IMR_S2_OLE_RDY_TMOUT_MASK) >> MCMN_IMR_S2_OLE_RDY_TMOUT_LSB)
#define MCMN_IMR_S2_OLE_RDY_TMOUT_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S2_OLE_RDY_TMOUT_LSB) & MCMN_IMR_S2_OLE_RDY_TMOUT_MASK)
#define MCMN_IMR_S2_OLE_RDY_TMOUT_RESET                              0
#define MCMN_IMR_S2_UNEX_NUL_PTR_MSB                                 7
#define MCMN_IMR_S2_UNEX_NUL_PTR_LSB                                 7
#define MCMN_IMR_S2_UNEX_NUL_PTR_MASK                                0x00000080
#define MCMN_IMR_S2_UNEX_NUL_PTR_GET(x)                              (((x) & MCMN_IMR_S2_UNEX_NUL_PTR_MASK) >> MCMN_IMR_S2_UNEX_NUL_PTR_LSB)
#define MCMN_IMR_S2_UNEX_NUL_PTR_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S2_UNEX_NUL_PTR_LSB) & MCMN_IMR_S2_UNEX_NUL_PTR_MASK)
#define MCMN_IMR_S2_UNEX_NUL_PTR_RESET                               0
#define MCMN_IMR_S2_WDG_TMOUT_MSB                                    6
#define MCMN_IMR_S2_WDG_TMOUT_LSB                                    6
#define MCMN_IMR_S2_WDG_TMOUT_MASK                                   0x00000040
#define MCMN_IMR_S2_WDG_TMOUT_GET(x)                                 (((x) & MCMN_IMR_S2_WDG_TMOUT_MASK) >> MCMN_IMR_S2_WDG_TMOUT_LSB)
#define MCMN_IMR_S2_WDG_TMOUT_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S2_WDG_TMOUT_LSB) & MCMN_IMR_S2_WDG_TMOUT_MASK)
#define MCMN_IMR_S2_WDG_TMOUT_RESET                                  0
#define MCMN_IMR_S2_INVA_IDX_MSB                                     5
#define MCMN_IMR_S2_INVA_IDX_LSB                                     5
#define MCMN_IMR_S2_INVA_IDX_MASK                                    0x00000020
#define MCMN_IMR_S2_INVA_IDX_GET(x)                                  (((x) & MCMN_IMR_S2_INVA_IDX_MASK) >> MCMN_IMR_S2_INVA_IDX_LSB)
#define MCMN_IMR_S2_INVA_IDX_SET(x)                                  (((0x0 | (x)) << MCMN_IMR_S2_INVA_IDX_LSB) & MCMN_IMR_S2_INVA_IDX_MASK)
#define MCMN_IMR_S2_INVA_IDX_RESET                                   0
#define MCMN_IMR_S2_BAD_TLV_ORDR_MSB                                 4
#define MCMN_IMR_S2_BAD_TLV_ORDR_LSB                                 4
#define MCMN_IMR_S2_BAD_TLV_ORDR_MASK                                0x00000010
#define MCMN_IMR_S2_BAD_TLV_ORDR_GET(x)                              (((x) & MCMN_IMR_S2_BAD_TLV_ORDR_MASK) >> MCMN_IMR_S2_BAD_TLV_ORDR_LSB)
#define MCMN_IMR_S2_BAD_TLV_ORDR_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S2_BAD_TLV_ORDR_LSB) & MCMN_IMR_S2_BAD_TLV_ORDR_MASK)
#define MCMN_IMR_S2_BAD_TLV_ORDR_RESET                               0
#define MCMN_IMR_S2_MISSIN_TLV_MSB                                   3
#define MCMN_IMR_S2_MISSIN_TLV_LSB                                   3
#define MCMN_IMR_S2_MISSIN_TLV_MASK                                  0x00000008
#define MCMN_IMR_S2_MISSIN_TLV_GET(x)                                (((x) & MCMN_IMR_S2_MISSIN_TLV_MASK) >> MCMN_IMR_S2_MISSIN_TLV_LSB)
#define MCMN_IMR_S2_MISSIN_TLV_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S2_MISSIN_TLV_LSB) & MCMN_IMR_S2_MISSIN_TLV_MASK)
#define MCMN_IMR_S2_MISSIN_TLV_RESET                                 0
#define MCMN_IMR_S2_UNEX_FES_MSB                                     2
#define MCMN_IMR_S2_UNEX_FES_LSB                                     2
#define MCMN_IMR_S2_UNEX_FES_MASK                                    0x00000004
#define MCMN_IMR_S2_UNEX_FES_GET(x)                                  (((x) & MCMN_IMR_S2_UNEX_FES_MASK) >> MCMN_IMR_S2_UNEX_FES_LSB)
#define MCMN_IMR_S2_UNEX_FES_SET(x)                                  (((0x0 | (x)) << MCMN_IMR_S2_UNEX_FES_LSB) & MCMN_IMR_S2_UNEX_FES_MASK)
#define MCMN_IMR_S2_UNEX_FES_RESET                                   0
#define MCMN_IMR_S2_INCOM_TLV_MSB                                    1
#define MCMN_IMR_S2_INCOM_TLV_LSB                                    1
#define MCMN_IMR_S2_INCOM_TLV_MASK                                   0x00000002
#define MCMN_IMR_S2_INCOM_TLV_GET(x)                                 (((x) & MCMN_IMR_S2_INCOM_TLV_MASK) >> MCMN_IMR_S2_INCOM_TLV_LSB)
#define MCMN_IMR_S2_INCOM_TLV_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S2_INCOM_TLV_LSB) & MCMN_IMR_S2_INCOM_TLV_MASK)
#define MCMN_IMR_S2_INCOM_TLV_RESET                                  0
#define MCMN_IMR_S2_TX_DMA_IDLE_MSB                                  0
#define MCMN_IMR_S2_TX_DMA_IDLE_LSB                                  0
#define MCMN_IMR_S2_TX_DMA_IDLE_MASK                                 0x00000001
#define MCMN_IMR_S2_TX_DMA_IDLE_GET(x)                               (((x) & MCMN_IMR_S2_TX_DMA_IDLE_MASK) >> MCMN_IMR_S2_TX_DMA_IDLE_LSB)
#define MCMN_IMR_S2_TX_DMA_IDLE_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S2_TX_DMA_IDLE_LSB) & MCMN_IMR_S2_TX_DMA_IDLE_MASK)
#define MCMN_IMR_S2_TX_DMA_IDLE_RESET                                0
#define MCMN_IMR_S2_ADDRESS                                          0x00003c
#define MCMN_IMR_S2_HW_MASK                                          0x000003ff
#define MCMN_IMR_S2_SW_MASK                                          0x000003ff
#define MCMN_IMR_S2_RSTMASK                                          0x00000000
#define MCMN_IMR_S2_RESET                                            0x00000000

// 0x0040 (MCMN_IMR_S3)
#define MCMN_IMR_S3_RXDMA_MSDU_MSB                                   16
#define MCMN_IMR_S3_RXDMA_MSDU_LSB                                   16
#define MCMN_IMR_S3_RXDMA_MSDU_MASK                                  0x00010000
#define MCMN_IMR_S3_RXDMA_MSDU_GET(x)                                (((x) & MCMN_IMR_S3_RXDMA_MSDU_MASK) >> MCMN_IMR_S3_RXDMA_MSDU_LSB)
#define MCMN_IMR_S3_RXDMA_MSDU_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S3_RXDMA_MSDU_LSB) & MCMN_IMR_S3_RXDMA_MSDU_MASK)
#define MCMN_IMR_S3_RXDMA_MSDU_RESET                                 0
#define MCMN_IMR_S3_RXDMA_MPDU_MSB                                   15
#define MCMN_IMR_S3_RXDMA_MPDU_LSB                                   15
#define MCMN_IMR_S3_RXDMA_MPDU_MASK                                  0x00008000
#define MCMN_IMR_S3_RXDMA_MPDU_GET(x)                                (((x) & MCMN_IMR_S3_RXDMA_MPDU_MASK) >> MCMN_IMR_S3_RXDMA_MPDU_LSB)
#define MCMN_IMR_S3_RXDMA_MPDU_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S3_RXDMA_MPDU_LSB) & MCMN_IMR_S3_RXDMA_MPDU_MASK)
#define MCMN_IMR_S3_RXDMA_MPDU_RESET                                 0
#define MCMN_IMR_S3_RXDMA_PPDU_MSB                                   14
#define MCMN_IMR_S3_RXDMA_PPDU_LSB                                   14
#define MCMN_IMR_S3_RXDMA_PPDU_MASK                                  0x00004000
#define MCMN_IMR_S3_RXDMA_PPDU_GET(x)                                (((x) & MCMN_IMR_S3_RXDMA_PPDU_MASK) >> MCMN_IMR_S3_RXDMA_PPDU_LSB)
#define MCMN_IMR_S3_RXDMA_PPDU_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S3_RXDMA_PPDU_LSB) & MCMN_IMR_S3_RXDMA_PPDU_MASK)
#define MCMN_IMR_S3_RXDMA_PPDU_RESET                                 0
#define MCMN_IMR_S3_RXDMA_UAPSD_MSB                                  13
#define MCMN_IMR_S3_RXDMA_UAPSD_LSB                                  13
#define MCMN_IMR_S3_RXDMA_UAPSD_MASK                                 0x00002000
#define MCMN_IMR_S3_RXDMA_UAPSD_GET(x)                               (((x) & MCMN_IMR_S3_RXDMA_UAPSD_MASK) >> MCMN_IMR_S3_RXDMA_UAPSD_LSB)
#define MCMN_IMR_S3_RXDMA_UAPSD_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S3_RXDMA_UAPSD_LSB) & MCMN_IMR_S3_RXDMA_UAPSD_MASK)
#define MCMN_IMR_S3_RXDMA_UAPSD_RESET                                0
#define MCMN_IMR_S3_RXDMA_RNG_LWM_MSB                                12
#define MCMN_IMR_S3_RXDMA_RNG_LWM_LSB                                9
#define MCMN_IMR_S3_RXDMA_RNG_LWM_MASK                               0x00001e00
#define MCMN_IMR_S3_RXDMA_RNG_LWM_GET(x)                             (((x) & MCMN_IMR_S3_RXDMA_RNG_LWM_MASK) >> MCMN_IMR_S3_RXDMA_RNG_LWM_LSB)
#define MCMN_IMR_S3_RXDMA_RNG_LWM_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S3_RXDMA_RNG_LWM_LSB) & MCMN_IMR_S3_RXDMA_RNG_LWM_MASK)
#define MCMN_IMR_S3_RXDMA_RNG_LWM_RESET                              0
#define MCMN_IMR_S3_TLV_FRME_ERR_MSB                                 8
#define MCMN_IMR_S3_TLV_FRME_ERR_LSB                                 8
#define MCMN_IMR_S3_TLV_FRME_ERR_MASK                                0x00000100
#define MCMN_IMR_S3_TLV_FRME_ERR_GET(x)                              (((x) & MCMN_IMR_S3_TLV_FRME_ERR_MASK) >> MCMN_IMR_S3_TLV_FRME_ERR_LSB)
#define MCMN_IMR_S3_TLV_FRME_ERR_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S3_TLV_FRME_ERR_LSB) & MCMN_IMR_S3_TLV_FRME_ERR_MASK)
#define MCMN_IMR_S3_TLV_FRME_ERR_RESET                               0
#define MCMN_IMR_S3_TLV_SHORT_MSB                                    7
#define MCMN_IMR_S3_TLV_SHORT_LSB                                    7
#define MCMN_IMR_S3_TLV_SHORT_MASK                                   0x00000080
#define MCMN_IMR_S3_TLV_SHORT_GET(x)                                 (((x) & MCMN_IMR_S3_TLV_SHORT_MASK) >> MCMN_IMR_S3_TLV_SHORT_LSB)
#define MCMN_IMR_S3_TLV_SHORT_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S3_TLV_SHORT_LSB) & MCMN_IMR_S3_TLV_SHORT_MASK)
#define MCMN_IMR_S3_TLV_SHORT_RESET                                  0
#define MCMN_IMR_S3_INVAL_TAG_SEG_MSB                                6
#define MCMN_IMR_S3_INVAL_TAG_SEG_LSB                                6
#define MCMN_IMR_S3_INVAL_TAG_SEG_MASK                               0x00000040
#define MCMN_IMR_S3_INVAL_TAG_SEG_GET(x)                             (((x) & MCMN_IMR_S3_INVAL_TAG_SEG_MASK) >> MCMN_IMR_S3_INVAL_TAG_SEG_LSB)
#define MCMN_IMR_S3_INVAL_TAG_SEG_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S3_INVAL_TAG_SEG_LSB) & MCMN_IMR_S3_INVAL_TAG_SEG_MASK)
#define MCMN_IMR_S3_INVAL_TAG_SEG_RESET                              0
#define MCMN_IMR_S3_INVAL_TAG_ID_MSB                                 5
#define MCMN_IMR_S3_INVAL_TAG_ID_LSB                                 5
#define MCMN_IMR_S3_INVAL_TAG_ID_MASK                                0x00000020
#define MCMN_IMR_S3_INVAL_TAG_ID_GET(x)                              (((x) & MCMN_IMR_S3_INVAL_TAG_ID_MASK) >> MCMN_IMR_S3_INVAL_TAG_ID_LSB)
#define MCMN_IMR_S3_INVAL_TAG_ID_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S3_INVAL_TAG_ID_LSB) & MCMN_IMR_S3_INVAL_TAG_ID_MASK)
#define MCMN_IMR_S3_INVAL_TAG_ID_RESET                               0
#define MCMN_IMR_S3_NULL_RNG_MSK_MSB                                 4
#define MCMN_IMR_S3_NULL_RNG_MSK_LSB                                 4
#define MCMN_IMR_S3_NULL_RNG_MSK_MASK                                0x00000010
#define MCMN_IMR_S3_NULL_RNG_MSK_GET(x)                              (((x) & MCMN_IMR_S3_NULL_RNG_MSK_MASK) >> MCMN_IMR_S3_NULL_RNG_MSK_LSB)
#define MCMN_IMR_S3_NULL_RNG_MSK_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S3_NULL_RNG_MSK_LSB) & MCMN_IMR_S3_NULL_RNG_MSK_MASK)
#define MCMN_IMR_S3_NULL_RNG_MSK_RESET                               0
#define MCMN_IMR_S3_MDAT_TLV_MSB                                     3
#define MCMN_IMR_S3_MDAT_TLV_LSB                                     3
#define MCMN_IMR_S3_MDAT_TLV_MASK                                    0x00000008
#define MCMN_IMR_S3_MDAT_TLV_GET(x)                                  (((x) & MCMN_IMR_S3_MDAT_TLV_MASK) >> MCMN_IMR_S3_MDAT_TLV_LSB)
#define MCMN_IMR_S3_MDAT_TLV_SET(x)                                  (((0x0 | (x)) << MCMN_IMR_S3_MDAT_TLV_LSB) & MCMN_IMR_S3_MDAT_TLV_MASK)
#define MCMN_IMR_S3_MDAT_TLV_RESET                                   0
#define MCMN_IMR_S3_NON_PKT_BUF_MSB                                  2
#define MCMN_IMR_S3_NON_PKT_BUF_LSB                                  2
#define MCMN_IMR_S3_NON_PKT_BUF_MASK                                 0x00000004
#define MCMN_IMR_S3_NON_PKT_BUF_GET(x)                               (((x) & MCMN_IMR_S3_NON_PKT_BUF_MASK) >> MCMN_IMR_S3_NON_PKT_BUF_LSB)
#define MCMN_IMR_S3_NON_PKT_BUF_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S3_NON_PKT_BUF_LSB) & MCMN_IMR_S3_NON_PKT_BUF_MASK)
#define MCMN_IMR_S3_NON_PKT_BUF_RESET                                0
#define MCMN_IMR_S3_RXDMA_WD_TOUT_MSB                                1
#define MCMN_IMR_S3_RXDMA_WD_TOUT_LSB                                1
#define MCMN_IMR_S3_RXDMA_WD_TOUT_MASK                               0x00000002
#define MCMN_IMR_S3_RXDMA_WD_TOUT_GET(x)                             (((x) & MCMN_IMR_S3_RXDMA_WD_TOUT_MASK) >> MCMN_IMR_S3_RXDMA_WD_TOUT_LSB)
#define MCMN_IMR_S3_RXDMA_WD_TOUT_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S3_RXDMA_WD_TOUT_LSB) & MCMN_IMR_S3_RXDMA_WD_TOUT_MASK)
#define MCMN_IMR_S3_RXDMA_WD_TOUT_RESET                              0
#define MCMN_IMR_S3_RXDMA_IDLE_MSB                                   0
#define MCMN_IMR_S3_RXDMA_IDLE_LSB                                   0
#define MCMN_IMR_S3_RXDMA_IDLE_MASK                                  0x00000001
#define MCMN_IMR_S3_RXDMA_IDLE_GET(x)                                (((x) & MCMN_IMR_S3_RXDMA_IDLE_MASK) >> MCMN_IMR_S3_RXDMA_IDLE_LSB)
#define MCMN_IMR_S3_RXDMA_IDLE_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S3_RXDMA_IDLE_LSB) & MCMN_IMR_S3_RXDMA_IDLE_MASK)
#define MCMN_IMR_S3_RXDMA_IDLE_RESET                                 0
#define MCMN_IMR_S3_ADDRESS                                          0x000040
#define MCMN_IMR_S3_HW_MASK                                          0x0001ffff
#define MCMN_IMR_S3_SW_MASK                                          0x0001ffff
#define MCMN_IMR_S3_RSTMASK                                          0x00000000
#define MCMN_IMR_S3_RESET                                            0x00000000

// 0x0044 (MCMN_IMR_S4)
#define MCMN_IMR_S4_TXPCU_WD_TOUT_MSB                                4
#define MCMN_IMR_S4_TXPCU_WD_TOUT_LSB                                4
#define MCMN_IMR_S4_TXPCU_WD_TOUT_MASK                               0x00000010
#define MCMN_IMR_S4_TXPCU_WD_TOUT_GET(x)                             (((x) & MCMN_IMR_S4_TXPCU_WD_TOUT_MASK) >> MCMN_IMR_S4_TXPCU_WD_TOUT_LSB)
#define MCMN_IMR_S4_TXPCU_WD_TOUT_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S4_TXPCU_WD_TOUT_LSB) & MCMN_IMR_S4_TXPCU_WD_TOUT_MASK)
#define MCMN_IMR_S4_TXPCU_WD_TOUT_RESET                              0
#define MCMN_IMR_S4_BFEE_CV_TOUT_MSB                                 3
#define MCMN_IMR_S4_BFEE_CV_TOUT_LSB                                 3
#define MCMN_IMR_S4_BFEE_CV_TOUT_MASK                                0x00000008
#define MCMN_IMR_S4_BFEE_CV_TOUT_GET(x)                              (((x) & MCMN_IMR_S4_BFEE_CV_TOUT_MASK) >> MCMN_IMR_S4_BFEE_CV_TOUT_LSB)
#define MCMN_IMR_S4_BFEE_CV_TOUT_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S4_BFEE_CV_TOUT_LSB) & MCMN_IMR_S4_BFEE_CV_TOUT_MASK)
#define MCMN_IMR_S4_BFEE_CV_TOUT_RESET                               0
#define MCMN_IMR_S4_DATA_URUN_MSB                                    2
#define MCMN_IMR_S4_DATA_URUN_LSB                                    2
#define MCMN_IMR_S4_DATA_URUN_MASK                                   0x00000004
#define MCMN_IMR_S4_DATA_URUN_GET(x)                                 (((x) & MCMN_IMR_S4_DATA_URUN_MASK) >> MCMN_IMR_S4_DATA_URUN_LSB)
#define MCMN_IMR_S4_DATA_URUN_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S4_DATA_URUN_LSB) & MCMN_IMR_S4_DATA_URUN_MASK)
#define MCMN_IMR_S4_DATA_URUN_RESET                                  0
#define MCMN_IMR_S4_RECD_GT_MPDU_MSB                                 1
#define MCMN_IMR_S4_RECD_GT_MPDU_LSB                                 1
#define MCMN_IMR_S4_RECD_GT_MPDU_MASK                                0x00000002
#define MCMN_IMR_S4_RECD_GT_MPDU_GET(x)                              (((x) & MCMN_IMR_S4_RECD_GT_MPDU_MASK) >> MCMN_IMR_S4_RECD_GT_MPDU_LSB)
#define MCMN_IMR_S4_RECD_GT_MPDU_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S4_RECD_GT_MPDU_LSB) & MCMN_IMR_S4_RECD_GT_MPDU_MASK)
#define MCMN_IMR_S4_RECD_GT_MPDU_RESET                               0
#define MCMN_IMR_S4_RECD_LT_MPDU_MSB                                 0
#define MCMN_IMR_S4_RECD_LT_MPDU_LSB                                 0
#define MCMN_IMR_S4_RECD_LT_MPDU_MASK                                0x00000001
#define MCMN_IMR_S4_RECD_LT_MPDU_GET(x)                              (((x) & MCMN_IMR_S4_RECD_LT_MPDU_MASK) >> MCMN_IMR_S4_RECD_LT_MPDU_LSB)
#define MCMN_IMR_S4_RECD_LT_MPDU_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S4_RECD_LT_MPDU_LSB) & MCMN_IMR_S4_RECD_LT_MPDU_MASK)
#define MCMN_IMR_S4_RECD_LT_MPDU_RESET                               0
#define MCMN_IMR_S4_ADDRESS                                          0x000044
#define MCMN_IMR_S4_HW_MASK                                          0x0000001f
#define MCMN_IMR_S4_SW_MASK                                          0x0000001f
#define MCMN_IMR_S4_RSTMASK                                          0x00000000
#define MCMN_IMR_S4_RESET                                            0x00000000

// 0x0048 (MCMN_IMR_S5)
#define MCMN_IMR_S5_RX_OVFL_MSB                                      13
#define MCMN_IMR_S5_RX_OVFL_LSB                                      13
#define MCMN_IMR_S5_RX_OVFL_MASK                                     0x00002000
#define MCMN_IMR_S5_RX_OVFL_GET(x)                                   (((x) & MCMN_IMR_S5_RX_OVFL_MASK) >> MCMN_IMR_S5_RX_OVFL_LSB)
#define MCMN_IMR_S5_RX_OVFL_SET(x)                                   (((0x0 | (x)) << MCMN_IMR_S5_RX_OVFL_LSB) & MCMN_IMR_S5_RX_OVFL_MASK)
#define MCMN_IMR_S5_RX_OVFL_RESET                                    0
#define MCMN_IMR_S5_CONFIG_PHY_ERR_MSB                               12
#define MCMN_IMR_S5_CONFIG_PHY_ERR_LSB                               12
#define MCMN_IMR_S5_CONFIG_PHY_ERR_MASK                              0x00001000
#define MCMN_IMR_S5_CONFIG_PHY_ERR_GET(x)                            (((x) & MCMN_IMR_S5_CONFIG_PHY_ERR_MASK) >> MCMN_IMR_S5_CONFIG_PHY_ERR_LSB)
#define MCMN_IMR_S5_CONFIG_PHY_ERR_SET(x)                            (((0x0 | (x)) << MCMN_IMR_S5_CONFIG_PHY_ERR_LSB) & MCMN_IMR_S5_CONFIG_PHY_ERR_MASK)
#define MCMN_IMR_S5_CONFIG_PHY_ERR_RESET                             0
#define MCMN_IMR_S5_RX_DTIM_MSB                                      11
#define MCMN_IMR_S5_RX_DTIM_LSB                                      10
#define MCMN_IMR_S5_RX_DTIM_MASK                                     0x00000c00
#define MCMN_IMR_S5_RX_DTIM_GET(x)                                   (((x) & MCMN_IMR_S5_RX_DTIM_MASK) >> MCMN_IMR_S5_RX_DTIM_LSB)
#define MCMN_IMR_S5_RX_DTIM_SET(x)                                   (((0x0 | (x)) << MCMN_IMR_S5_RX_DTIM_LSB) & MCMN_IMR_S5_RX_DTIM_MASK)
#define MCMN_IMR_S5_RX_DTIM_RESET                                    0
#define MCMN_IMR_S5_RX_TIM_MSB                                       9
#define MCMN_IMR_S5_RX_TIM_LSB                                       8
#define MCMN_IMR_S5_RX_TIM_MASK                                      0x00000300
#define MCMN_IMR_S5_RX_TIM_GET(x)                                    (((x) & MCMN_IMR_S5_RX_TIM_MASK) >> MCMN_IMR_S5_RX_TIM_LSB)
#define MCMN_IMR_S5_RX_TIM_SET(x)                                    (((0x0 | (x)) << MCMN_IMR_S5_RX_TIM_LSB) & MCMN_IMR_S5_RX_TIM_MASK)
#define MCMN_IMR_S5_RX_TIM_RESET                                     0
#define MCMN_IMR_S5_RX_NOTIM_MSB                                     7
#define MCMN_IMR_S5_RX_NOTIM_LSB                                     6
#define MCMN_IMR_S5_RX_NOTIM_MASK                                    0x000000c0
#define MCMN_IMR_S5_RX_NOTIM_GET(x)                                  (((x) & MCMN_IMR_S5_RX_NOTIM_MASK) >> MCMN_IMR_S5_RX_NOTIM_LSB)
#define MCMN_IMR_S5_RX_NOTIM_SET(x)                                  (((0x0 | (x)) << MCMN_IMR_S5_RX_NOTIM_LSB) & MCMN_IMR_S5_RX_NOTIM_MASK)
#define MCMN_IMR_S5_RX_NOTIM_RESET                                   0
#define MCMN_IMR_S5_RSSI_LOW_MSB                                     5
#define MCMN_IMR_S5_RSSI_LOW_LSB                                     4
#define MCMN_IMR_S5_RSSI_LOW_MASK                                    0x00000030
#define MCMN_IMR_S5_RSSI_LOW_GET(x)                                  (((x) & MCMN_IMR_S5_RSSI_LOW_MASK) >> MCMN_IMR_S5_RSSI_LOW_LSB)
#define MCMN_IMR_S5_RSSI_LOW_SET(x)                                  (((0x0 | (x)) << MCMN_IMR_S5_RSSI_LOW_LSB) & MCMN_IMR_S5_RSSI_LOW_MASK)
#define MCMN_IMR_S5_RSSI_LOW_RESET                                   0
#define MCMN_IMR_S5_RSSI_HIGH_MSB                                    3
#define MCMN_IMR_S5_RSSI_HIGH_LSB                                    2
#define MCMN_IMR_S5_RSSI_HIGH_MASK                                   0x0000000c
#define MCMN_IMR_S5_RSSI_HIGH_GET(x)                                 (((x) & MCMN_IMR_S5_RSSI_HIGH_MASK) >> MCMN_IMR_S5_RSSI_HIGH_LSB)
#define MCMN_IMR_S5_RSSI_HIGH_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S5_RSSI_HIGH_LSB) & MCMN_IMR_S5_RSSI_HIGH_MASK)
#define MCMN_IMR_S5_RSSI_HIGH_RESET                                  0
#define MCMN_IMR_S5_BCON_MISS_MSB                                    1
#define MCMN_IMR_S5_BCON_MISS_LSB                                    0
#define MCMN_IMR_S5_BCON_MISS_MASK                                   0x00000003
#define MCMN_IMR_S5_BCON_MISS_GET(x)                                 (((x) & MCMN_IMR_S5_BCON_MISS_MASK) >> MCMN_IMR_S5_BCON_MISS_LSB)
#define MCMN_IMR_S5_BCON_MISS_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S5_BCON_MISS_LSB) & MCMN_IMR_S5_BCON_MISS_MASK)
#define MCMN_IMR_S5_BCON_MISS_RESET                                  0
#define MCMN_IMR_S5_ADDRESS                                          0x000048
#define MCMN_IMR_S5_HW_MASK                                          0x00003fff
#define MCMN_IMR_S5_SW_MASK                                          0x00003fff
#define MCMN_IMR_S5_RSTMASK                                          0x00000000
#define MCMN_IMR_S5_RESET                                            0x00000000

// 0x004c (MCMN_IMR_S6)
#define MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_MSB                            4
#define MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_LSB                            4
#define MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_MASK                           0x00000010
#define MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_GET(x)                         (((x) & MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_MASK) >> MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_LSB)
#define MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_SET(x)                         (((0x0 | (x)) << MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_LSB) & MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_MASK)
#define MCMN_IMR_S6_TXDMA_PDG_LEN_ERR_RESET                          0
#define MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_MSB                         3
#define MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_LSB                         3
#define MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_MASK                        0x00000008
#define MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_GET(x)                      (((x) & MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_MASK) >> MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_LSB)
#define MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_SET(x)                      (((0x0 | (x)) << MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_LSB) & MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_MASK)
#define MCMN_IMR_S6_QMGR_PDG_TLV_LEN_ERR_RESET                       0
#define MCMN_IMR_S6_WRNG_TLV_ORD_MSB                                 2
#define MCMN_IMR_S6_WRNG_TLV_ORD_LSB                                 2
#define MCMN_IMR_S6_WRNG_TLV_ORD_MASK                                0x00000004
#define MCMN_IMR_S6_WRNG_TLV_ORD_GET(x)                              (((x) & MCMN_IMR_S6_WRNG_TLV_ORD_MASK) >> MCMN_IMR_S6_WRNG_TLV_ORD_LSB)
#define MCMN_IMR_S6_WRNG_TLV_ORD_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S6_WRNG_TLV_ORD_LSB) & MCMN_IMR_S6_WRNG_TLV_ORD_MASK)
#define MCMN_IMR_S6_WRNG_TLV_ORD_RESET                               0
#define MCMN_IMR_S6_RCV_FCS_BSY_MSB                                  1
#define MCMN_IMR_S6_RCV_FCS_BSY_LSB                                  1
#define MCMN_IMR_S6_RCV_FCS_BSY_MASK                                 0x00000002
#define MCMN_IMR_S6_RCV_FCS_BSY_GET(x)                               (((x) & MCMN_IMR_S6_RCV_FCS_BSY_MASK) >> MCMN_IMR_S6_RCV_FCS_BSY_LSB)
#define MCMN_IMR_S6_RCV_FCS_BSY_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S6_RCV_FCS_BSY_LSB) & MCMN_IMR_S6_RCV_FCS_BSY_MASK)
#define MCMN_IMR_S6_RCV_FCS_BSY_RESET                                0
#define MCMN_IMR_S6_PDG_WD_TOUT_MSB                                  0
#define MCMN_IMR_S6_PDG_WD_TOUT_LSB                                  0
#define MCMN_IMR_S6_PDG_WD_TOUT_MASK                                 0x00000001
#define MCMN_IMR_S6_PDG_WD_TOUT_GET(x)                               (((x) & MCMN_IMR_S6_PDG_WD_TOUT_MASK) >> MCMN_IMR_S6_PDG_WD_TOUT_LSB)
#define MCMN_IMR_S6_PDG_WD_TOUT_SET(x)                               (((0x0 | (x)) << MCMN_IMR_S6_PDG_WD_TOUT_LSB) & MCMN_IMR_S6_PDG_WD_TOUT_MASK)
#define MCMN_IMR_S6_PDG_WD_TOUT_RESET                                0
#define MCMN_IMR_S6_ADDRESS                                          0x00004c
#define MCMN_IMR_S6_HW_MASK                                          0x0000001f
#define MCMN_IMR_S6_SW_MASK                                          0x0000001f
#define MCMN_IMR_S6_RSTMASK                                          0x00000000
#define MCMN_IMR_S6_RESET                                            0x00000000

// 0x0050 (MCMN_IMR_S7)
#define MCMN_IMR_S7_HW_RNG_THR_MSB                                   31
#define MCMN_IMR_S7_HW_RNG_THR_LSB                                   16
#define MCMN_IMR_S7_HW_RNG_THR_MASK                                  0xffff0000
#define MCMN_IMR_S7_HW_RNG_THR_GET(x)                                (((x) & MCMN_IMR_S7_HW_RNG_THR_MASK) >> MCMN_IMR_S7_HW_RNG_THR_LSB)
#define MCMN_IMR_S7_HW_RNG_THR_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S7_HW_RNG_THR_LSB) & MCMN_IMR_S7_HW_RNG_THR_MASK)
#define MCMN_IMR_S7_HW_RNG_THR_RESET                                 0
#define MCMN_IMR_S7_HW_RNG_PSD_MSB                                   15
#define MCMN_IMR_S7_HW_RNG_PSD_LSB                                   0
#define MCMN_IMR_S7_HW_RNG_PSD_MASK                                  0x0000ffff
#define MCMN_IMR_S7_HW_RNG_PSD_GET(x)                                (((x) & MCMN_IMR_S7_HW_RNG_PSD_MASK) >> MCMN_IMR_S7_HW_RNG_PSD_LSB)
#define MCMN_IMR_S7_HW_RNG_PSD_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S7_HW_RNG_PSD_LSB) & MCMN_IMR_S7_HW_RNG_PSD_MASK)
#define MCMN_IMR_S7_HW_RNG_PSD_RESET                                 0
#define MCMN_IMR_S7_ADDRESS                                          0x000050
#define MCMN_IMR_S7_HW_MASK                                          0xffffffff
#define MCMN_IMR_S7_SW_MASK                                          0xffffffff
#define MCMN_IMR_S7_RSTMASK                                          0x00000000
#define MCMN_IMR_S7_RESET                                            0x00000000

// 0x0054 (MCMN_IMR_S8)
#define MCMN_IMR_S8_MTU_TIMER_MSB                                    30
#define MCMN_IMR_S8_MTU_TIMER_LSB                                    19
#define MCMN_IMR_S8_MTU_TIMER_MASK                                   0x7ff80000
#define MCMN_IMR_S8_MTU_TIMER_GET(x)                                 (((x) & MCMN_IMR_S8_MTU_TIMER_MASK) >> MCMN_IMR_S8_MTU_TIMER_LSB)
#define MCMN_IMR_S8_MTU_TIMER_SET(x)                                 (((0x0 | (x)) << MCMN_IMR_S8_MTU_TIMER_LSB) & MCMN_IMR_S8_MTU_TIMER_MASK)
#define MCMN_IMR_S8_MTU_TIMER_RESET                                  0
#define MCMN_IMR_S8_MTU_TSF_MSB                                      18
#define MCMN_IMR_S8_MTU_TSF_LSB                                      16
#define MCMN_IMR_S8_MTU_TSF_MASK                                     0x00070000
#define MCMN_IMR_S8_MTU_TSF_GET(x)                                   (((x) & MCMN_IMR_S8_MTU_TSF_MASK) >> MCMN_IMR_S8_MTU_TSF_LSB)
#define MCMN_IMR_S8_MTU_TSF_SET(x)                                   (((0x0 | (x)) << MCMN_IMR_S8_MTU_TSF_LSB) & MCMN_IMR_S8_MTU_TSF_MASK)
#define MCMN_IMR_S8_MTU_TSF_RESET                                    0
#define MCMN_IMR_S8_MTU_BKOF_MSB                                     15
#define MCMN_IMR_S8_MTU_BKOF_LSB                                     0
#define MCMN_IMR_S8_MTU_BKOF_MASK                                    0x0000ffff
#define MCMN_IMR_S8_MTU_BKOF_GET(x)                                  (((x) & MCMN_IMR_S8_MTU_BKOF_MASK) >> MCMN_IMR_S8_MTU_BKOF_LSB)
#define MCMN_IMR_S8_MTU_BKOF_SET(x)                                  (((0x0 | (x)) << MCMN_IMR_S8_MTU_BKOF_LSB) & MCMN_IMR_S8_MTU_BKOF_MASK)
#define MCMN_IMR_S8_MTU_BKOF_RESET                                   0
#define MCMN_IMR_S8_ADDRESS                                          0x000054
#define MCMN_IMR_S8_HW_MASK                                          0x7fffffff
#define MCMN_IMR_S8_SW_MASK                                          0x7fffffff
#define MCMN_IMR_S8_RSTMASK                                          0x00000000
#define MCMN_IMR_S8_RESET                                            0x00000000

// 0x0058 (MCMN_IMR_S9)
#define MCMN_IMR_S9_MTU_ERR_MSB                                      7
#define MCMN_IMR_S9_MTU_ERR_LSB                                      7
#define MCMN_IMR_S9_MTU_ERR_MASK                                     0x00000080
#define MCMN_IMR_S9_MTU_ERR_GET(x)                                   (((x) & MCMN_IMR_S9_MTU_ERR_MASK) >> MCMN_IMR_S9_MTU_ERR_LSB)
#define MCMN_IMR_S9_MTU_ERR_SET(x)                                   (((0x0 | (x)) << MCMN_IMR_S9_MTU_ERR_LSB) & MCMN_IMR_S9_MTU_ERR_MASK)
#define MCMN_IMR_S9_MTU_ERR_RESET                                    0
#define MCMN_IMR_S9_MTU_TX_BOUND_MSB                                 6
#define MCMN_IMR_S9_MTU_TX_BOUND_LSB                                 6
#define MCMN_IMR_S9_MTU_TX_BOUND_MASK                                0x00000040
#define MCMN_IMR_S9_MTU_TX_BOUND_GET(x)                              (((x) & MCMN_IMR_S9_MTU_TX_BOUND_MASK) >> MCMN_IMR_S9_MTU_TX_BOUND_LSB)
#define MCMN_IMR_S9_MTU_TX_BOUND_SET(x)                              (((0x0 | (x)) << MCMN_IMR_S9_MTU_TX_BOUND_LSB) & MCMN_IMR_S9_MTU_TX_BOUND_MASK)
#define MCMN_IMR_S9_MTU_TX_BOUND_RESET                               0
#define MCMN_IMR_S9_TLV_STS_UPDATE_MSB                               5
#define MCMN_IMR_S9_TLV_STS_UPDATE_LSB                               5
#define MCMN_IMR_S9_TLV_STS_UPDATE_MASK                              0x00000020
#define MCMN_IMR_S9_TLV_STS_UPDATE_GET(x)                            (((x) & MCMN_IMR_S9_TLV_STS_UPDATE_MASK) >> MCMN_IMR_S9_TLV_STS_UPDATE_LSB)
#define MCMN_IMR_S9_TLV_STS_UPDATE_SET(x)                            (((0x0 | (x)) << MCMN_IMR_S9_TLV_STS_UPDATE_LSB) & MCMN_IMR_S9_TLV_STS_UPDATE_MASK)
#define MCMN_IMR_S9_TLV_STS_UPDATE_RESET                             0
#define MCMN_IMR_S9_TLV_SETUP_ERR_MSB                                4
#define MCMN_IMR_S9_TLV_SETUP_ERR_LSB                                4
#define MCMN_IMR_S9_TLV_SETUP_ERR_MASK                               0x00000010
#define MCMN_IMR_S9_TLV_SETUP_ERR_GET(x)                             (((x) & MCMN_IMR_S9_TLV_SETUP_ERR_MASK) >> MCMN_IMR_S9_TLV_SETUP_ERR_LSB)
#define MCMN_IMR_S9_TLV_SETUP_ERR_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S9_TLV_SETUP_ERR_LSB) & MCMN_IMR_S9_TLV_SETUP_ERR_MASK)
#define MCMN_IMR_S9_TLV_SETUP_ERR_RESET                              0
#define MCMN_IMR_S9_FES_TLV_FLUSH_MSB                                3
#define MCMN_IMR_S9_FES_TLV_FLUSH_LSB                                3
#define MCMN_IMR_S9_FES_TLV_FLUSH_MASK                               0x00000008
#define MCMN_IMR_S9_FES_TLV_FLUSH_GET(x)                             (((x) & MCMN_IMR_S9_FES_TLV_FLUSH_MASK) >> MCMN_IMR_S9_FES_TLV_FLUSH_LSB)
#define MCMN_IMR_S9_FES_TLV_FLUSH_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S9_FES_TLV_FLUSH_LSB) & MCMN_IMR_S9_FES_TLV_FLUSH_MASK)
#define MCMN_IMR_S9_FES_TLV_FLUSH_RESET                              0
#define MCMN_IMR_S9_AXI_RD_ERR_MSB                                   2
#define MCMN_IMR_S9_AXI_RD_ERR_LSB                                   2
#define MCMN_IMR_S9_AXI_RD_ERR_MASK                                  0x00000004
#define MCMN_IMR_S9_AXI_RD_ERR_GET(x)                                (((x) & MCMN_IMR_S9_AXI_RD_ERR_MASK) >> MCMN_IMR_S9_AXI_RD_ERR_LSB)
#define MCMN_IMR_S9_AXI_RD_ERR_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S9_AXI_RD_ERR_LSB) & MCMN_IMR_S9_AXI_RD_ERR_MASK)
#define MCMN_IMR_S9_AXI_RD_ERR_RESET                                 0
#define MCMN_IMR_S9_AXI_WR_ERR_MSB                                   1
#define MCMN_IMR_S9_AXI_WR_ERR_LSB                                   1
#define MCMN_IMR_S9_AXI_WR_ERR_MASK                                  0x00000002
#define MCMN_IMR_S9_AXI_WR_ERR_GET(x)                                (((x) & MCMN_IMR_S9_AXI_WR_ERR_MASK) >> MCMN_IMR_S9_AXI_WR_ERR_LSB)
#define MCMN_IMR_S9_AXI_WR_ERR_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S9_AXI_WR_ERR_LSB) & MCMN_IMR_S9_AXI_WR_ERR_MASK)
#define MCMN_IMR_S9_AXI_WR_ERR_RESET                                 0
#define MCMN_IMR_S9_HWSCH_WD_TOUT_MSB                                0
#define MCMN_IMR_S9_HWSCH_WD_TOUT_LSB                                0
#define MCMN_IMR_S9_HWSCH_WD_TOUT_MASK                               0x00000001
#define MCMN_IMR_S9_HWSCH_WD_TOUT_GET(x)                             (((x) & MCMN_IMR_S9_HWSCH_WD_TOUT_MASK) >> MCMN_IMR_S9_HWSCH_WD_TOUT_LSB)
#define MCMN_IMR_S9_HWSCH_WD_TOUT_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S9_HWSCH_WD_TOUT_LSB) & MCMN_IMR_S9_HWSCH_WD_TOUT_MASK)
#define MCMN_IMR_S9_HWSCH_WD_TOUT_RESET                              0
#define MCMN_IMR_S9_ADDRESS                                          0x000058
#define MCMN_IMR_S9_HW_MASK                                          0x000000ff
#define MCMN_IMR_S9_SW_MASK                                          0x000000ff
#define MCMN_IMR_S9_RSTMASK                                          0x00000000
#define MCMN_IMR_S9_RESET                                            0x00000000

// 0x005c (MCMN_IMR_S10)
#define MCMN_IMR_S10_AXI_TMOUT_MSB                                   1
#define MCMN_IMR_S10_AXI_TMOUT_LSB                                   1
#define MCMN_IMR_S10_AXI_TMOUT_MASK                                  0x00000002
#define MCMN_IMR_S10_AXI_TMOUT_GET(x)                                (((x) & MCMN_IMR_S10_AXI_TMOUT_MASK) >> MCMN_IMR_S10_AXI_TMOUT_LSB)
#define MCMN_IMR_S10_AXI_TMOUT_SET(x)                                (((0x0 | (x)) << MCMN_IMR_S10_AXI_TMOUT_LSB) & MCMN_IMR_S10_AXI_TMOUT_MASK)
#define MCMN_IMR_S10_AXI_TMOUT_RESET                                 0
#define MCMN_IMR_S10_AXI_RESP_ERR_MSB                                0
#define MCMN_IMR_S10_AXI_RESP_ERR_LSB                                0
#define MCMN_IMR_S10_AXI_RESP_ERR_MASK                               0x00000001
#define MCMN_IMR_S10_AXI_RESP_ERR_GET(x)                             (((x) & MCMN_IMR_S10_AXI_RESP_ERR_MASK) >> MCMN_IMR_S10_AXI_RESP_ERR_LSB)
#define MCMN_IMR_S10_AXI_RESP_ERR_SET(x)                             (((0x0 | (x)) << MCMN_IMR_S10_AXI_RESP_ERR_LSB) & MCMN_IMR_S10_AXI_RESP_ERR_MASK)
#define MCMN_IMR_S10_AXI_RESP_ERR_RESET                              0
#define MCMN_IMR_S10_ADDRESS                                         0x00005c
#define MCMN_IMR_S10_HW_MASK                                         0x00000003
#define MCMN_IMR_S10_SW_MASK                                         0x00000003
#define MCMN_IMR_S10_RSTMASK                                         0x00000000
#define MCMN_IMR_S10_RESET                                           0x00000000

// 0x0060 (MPDU_QUEUE_DESC_BAR)
#define MPDU_QUEUE_DESC_BAR_VAL_MSB                                  31
#define MPDU_QUEUE_DESC_BAR_VAL_LSB                                  0
#define MPDU_QUEUE_DESC_BAR_VAL_MASK                                 0xffffffff
#define MPDU_QUEUE_DESC_BAR_VAL_GET(x)                               (((x) & MPDU_QUEUE_DESC_BAR_VAL_MASK) >> MPDU_QUEUE_DESC_BAR_VAL_LSB)
#define MPDU_QUEUE_DESC_BAR_VAL_SET(x)                               (((0x0 | (x)) << MPDU_QUEUE_DESC_BAR_VAL_LSB) & MPDU_QUEUE_DESC_BAR_VAL_MASK)
#define MPDU_QUEUE_DESC_BAR_VAL_RESET                                0
#define MPDU_QUEUE_DESC_BAR_ADDRESS                                  0x000060
#define MPDU_QUEUE_DESC_BAR_HW_MASK                                  0xffffffff
#define MPDU_QUEUE_DESC_BAR_SW_MASK                                  0xffffffff
#define MPDU_QUEUE_DESC_BAR_RSTMASK                                  0x00000000
#define MPDU_QUEUE_DESC_BAR_RESET                                    0x00000000

// 0x0064 (MPDU_QUEUE_EXT_DESC_BASE_ADDR)
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_MSB                        31
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_LSB                        0
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_MASK                       0xffffffff
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_GET(x)                     (((x) & MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_MASK) >> MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_LSB)
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_SET(x)                     (((0x0 | (x)) << MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_LSB) & MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_MASK)
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_VAL_RESET                      0
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_ADDRESS                        0x000064
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_HW_MASK                        0xffffffff
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_SW_MASK                        0xffffffff
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_RSTMASK                        0x00000000
#define MPDU_QUEUE_EXT_DESC_BASE_ADDR_RESET                          0x00000000

// 0x0068 (MPDU_LINK_DESC_BASE_ADDR)
#define MPDU_LINK_DESC_BASE_ADDR_VAL_MSB                             31
#define MPDU_LINK_DESC_BASE_ADDR_VAL_LSB                             0
#define MPDU_LINK_DESC_BASE_ADDR_VAL_MASK                            0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_VAL_GET(x)                          (((x) & MPDU_LINK_DESC_BASE_ADDR_VAL_MASK) >> MPDU_LINK_DESC_BASE_ADDR_VAL_LSB)
#define MPDU_LINK_DESC_BASE_ADDR_VAL_SET(x)                          (((0x0 | (x)) << MPDU_LINK_DESC_BASE_ADDR_VAL_LSB) & MPDU_LINK_DESC_BASE_ADDR_VAL_MASK)
#define MPDU_LINK_DESC_BASE_ADDR_VAL_RESET                           0
#define MPDU_LINK_DESC_BASE_ADDR_ADDRESS                             0x000068
#define MPDU_LINK_DESC_BASE_ADDR_HW_MASK                             0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_SW_MASK                             0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_RSTMASK                             0x00000000
#define MPDU_LINK_DESC_BASE_ADDR_RESET                               0x00000000

// 0x0068 (MPDU_LINK_DESC_BASE_ADDR_0)
#define MPDU_LINK_DESC_BASE_ADDR_0_VAL_MSB                           31
#define MPDU_LINK_DESC_BASE_ADDR_0_VAL_LSB                           0
#define MPDU_LINK_DESC_BASE_ADDR_0_VAL_MASK                          0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_0_VAL_GET(x)                        (((x) & MPDU_LINK_DESC_BASE_ADDR_0_VAL_MASK) >> MPDU_LINK_DESC_BASE_ADDR_0_VAL_LSB)
#define MPDU_LINK_DESC_BASE_ADDR_0_VAL_SET(x)                        (((0x0 | (x)) << MPDU_LINK_DESC_BASE_ADDR_0_VAL_LSB) & MPDU_LINK_DESC_BASE_ADDR_0_VAL_MASK)
#define MPDU_LINK_DESC_BASE_ADDR_0_VAL_RESET                         0
#define MPDU_LINK_DESC_BASE_ADDR_0_ADDRESS                           0x0068
#define MPDU_LINK_DESC_BASE_ADDR_0_HW_MASK                           0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_0_SW_MASK                           0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_0_RSTMASK                           0x00000000
#define MPDU_LINK_DESC_BASE_ADDR_0_RESET                             0x00000000

// 0x006c (MPDU_LINK_DESC_BASE_ADDR_1)
#define MPDU_LINK_DESC_BASE_ADDR_1_VAL_MSB                           31
#define MPDU_LINK_DESC_BASE_ADDR_1_VAL_LSB                           0
#define MPDU_LINK_DESC_BASE_ADDR_1_VAL_MASK                          0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_1_VAL_GET(x)                        (((x) & MPDU_LINK_DESC_BASE_ADDR_1_VAL_MASK) >> MPDU_LINK_DESC_BASE_ADDR_1_VAL_LSB)
#define MPDU_LINK_DESC_BASE_ADDR_1_VAL_SET(x)                        (((0x0 | (x)) << MPDU_LINK_DESC_BASE_ADDR_1_VAL_LSB) & MPDU_LINK_DESC_BASE_ADDR_1_VAL_MASK)
#define MPDU_LINK_DESC_BASE_ADDR_1_VAL_RESET                         0
#define MPDU_LINK_DESC_BASE_ADDR_1_ADDRESS                           0x006c
#define MPDU_LINK_DESC_BASE_ADDR_1_HW_MASK                           0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_1_SW_MASK                           0xffffffff
#define MPDU_LINK_DESC_BASE_ADDR_1_RSTMASK                           0x00000000
#define MPDU_LINK_DESC_BASE_ADDR_1_RESET                             0x00000000

// 0x0070 (MPDU_LINK_DESC_MIN_IDX)
#define MPDU_LINK_DESC_MIN_IDX_VAL_MSB                               31
#define MPDU_LINK_DESC_MIN_IDX_VAL_LSB                               0
#define MPDU_LINK_DESC_MIN_IDX_VAL_MASK                              0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_VAL_GET(x)                            (((x) & MPDU_LINK_DESC_MIN_IDX_VAL_MASK) >> MPDU_LINK_DESC_MIN_IDX_VAL_LSB)
#define MPDU_LINK_DESC_MIN_IDX_VAL_SET(x)                            (((0x0 | (x)) << MPDU_LINK_DESC_MIN_IDX_VAL_LSB) & MPDU_LINK_DESC_MIN_IDX_VAL_MASK)
#define MPDU_LINK_DESC_MIN_IDX_VAL_RESET                             0
#define MPDU_LINK_DESC_MIN_IDX_ADDRESS                               0x000070
#define MPDU_LINK_DESC_MIN_IDX_HW_MASK                               0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_SW_MASK                               0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_RSTMASK                               0x00000000
#define MPDU_LINK_DESC_MIN_IDX_RESET                                 0x00000000

// 0x0070 (MPDU_LINK_DESC_MIN_IDX_0)
#define MPDU_LINK_DESC_MIN_IDX_0_VAL_MSB                             31
#define MPDU_LINK_DESC_MIN_IDX_0_VAL_LSB                             0
#define MPDU_LINK_DESC_MIN_IDX_0_VAL_MASK                            0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_0_VAL_GET(x)                          (((x) & MPDU_LINK_DESC_MIN_IDX_0_VAL_MASK) >> MPDU_LINK_DESC_MIN_IDX_0_VAL_LSB)
#define MPDU_LINK_DESC_MIN_IDX_0_VAL_SET(x)                          (((0x0 | (x)) << MPDU_LINK_DESC_MIN_IDX_0_VAL_LSB) & MPDU_LINK_DESC_MIN_IDX_0_VAL_MASK)
#define MPDU_LINK_DESC_MIN_IDX_0_VAL_RESET                           0
#define MPDU_LINK_DESC_MIN_IDX_0_ADDRESS                             0x0070
#define MPDU_LINK_DESC_MIN_IDX_0_HW_MASK                             0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_0_SW_MASK                             0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_0_RSTMASK                             0x00000000
#define MPDU_LINK_DESC_MIN_IDX_0_RESET                               0x00000000

// 0x0074 (MPDU_LINK_DESC_MIN_IDX_1)
#define MPDU_LINK_DESC_MIN_IDX_1_VAL_MSB                             31
#define MPDU_LINK_DESC_MIN_IDX_1_VAL_LSB                             0
#define MPDU_LINK_DESC_MIN_IDX_1_VAL_MASK                            0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_1_VAL_GET(x)                          (((x) & MPDU_LINK_DESC_MIN_IDX_1_VAL_MASK) >> MPDU_LINK_DESC_MIN_IDX_1_VAL_LSB)
#define MPDU_LINK_DESC_MIN_IDX_1_VAL_SET(x)                          (((0x0 | (x)) << MPDU_LINK_DESC_MIN_IDX_1_VAL_LSB) & MPDU_LINK_DESC_MIN_IDX_1_VAL_MASK)
#define MPDU_LINK_DESC_MIN_IDX_1_VAL_RESET                           0
#define MPDU_LINK_DESC_MIN_IDX_1_ADDRESS                             0x0074
#define MPDU_LINK_DESC_MIN_IDX_1_HW_MASK                             0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_1_SW_MASK                             0xffffffff
#define MPDU_LINK_DESC_MIN_IDX_1_RSTMASK                             0x00000000
#define MPDU_LINK_DESC_MIN_IDX_1_RESET                               0x00000000

// 0x0078 (MPDU_LINK_DESC_MAX_IDX)
#define MPDU_LINK_DESC_MAX_IDX_VAL_MSB                               31
#define MPDU_LINK_DESC_MAX_IDX_VAL_LSB                               0
#define MPDU_LINK_DESC_MAX_IDX_VAL_MASK                              0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_VAL_GET(x)                            (((x) & MPDU_LINK_DESC_MAX_IDX_VAL_MASK) >> MPDU_LINK_DESC_MAX_IDX_VAL_LSB)
#define MPDU_LINK_DESC_MAX_IDX_VAL_SET(x)                            (((0x0 | (x)) << MPDU_LINK_DESC_MAX_IDX_VAL_LSB) & MPDU_LINK_DESC_MAX_IDX_VAL_MASK)
#define MPDU_LINK_DESC_MAX_IDX_VAL_RESET                             0
#define MPDU_LINK_DESC_MAX_IDX_ADDRESS                               0x000078
#define MPDU_LINK_DESC_MAX_IDX_HW_MASK                               0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_SW_MASK                               0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_RSTMASK                               0x00000000
#define MPDU_LINK_DESC_MAX_IDX_RESET                                 0x00000000

// 0x0078 (MPDU_LINK_DESC_MAX_IDX_0)
#define MPDU_LINK_DESC_MAX_IDX_0_VAL_MSB                             31
#define MPDU_LINK_DESC_MAX_IDX_0_VAL_LSB                             0
#define MPDU_LINK_DESC_MAX_IDX_0_VAL_MASK                            0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_0_VAL_GET(x)                          (((x) & MPDU_LINK_DESC_MAX_IDX_0_VAL_MASK) >> MPDU_LINK_DESC_MAX_IDX_0_VAL_LSB)
#define MPDU_LINK_DESC_MAX_IDX_0_VAL_SET(x)                          (((0x0 | (x)) << MPDU_LINK_DESC_MAX_IDX_0_VAL_LSB) & MPDU_LINK_DESC_MAX_IDX_0_VAL_MASK)
#define MPDU_LINK_DESC_MAX_IDX_0_VAL_RESET                           0
#define MPDU_LINK_DESC_MAX_IDX_0_ADDRESS                             0x0078
#define MPDU_LINK_DESC_MAX_IDX_0_HW_MASK                             0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_0_SW_MASK                             0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_0_RSTMASK                             0x00000000
#define MPDU_LINK_DESC_MAX_IDX_0_RESET                               0x00000000

// 0x007c (MPDU_LINK_DESC_MAX_IDX_1)
#define MPDU_LINK_DESC_MAX_IDX_1_VAL_MSB                             31
#define MPDU_LINK_DESC_MAX_IDX_1_VAL_LSB                             0
#define MPDU_LINK_DESC_MAX_IDX_1_VAL_MASK                            0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_1_VAL_GET(x)                          (((x) & MPDU_LINK_DESC_MAX_IDX_1_VAL_MASK) >> MPDU_LINK_DESC_MAX_IDX_1_VAL_LSB)
#define MPDU_LINK_DESC_MAX_IDX_1_VAL_SET(x)                          (((0x0 | (x)) << MPDU_LINK_DESC_MAX_IDX_1_VAL_LSB) & MPDU_LINK_DESC_MAX_IDX_1_VAL_MASK)
#define MPDU_LINK_DESC_MAX_IDX_1_VAL_RESET                           0
#define MPDU_LINK_DESC_MAX_IDX_1_ADDRESS                             0x007c
#define MPDU_LINK_DESC_MAX_IDX_1_HW_MASK                             0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_1_SW_MASK                             0xffffffff
#define MPDU_LINK_DESC_MAX_IDX_1_RSTMASK                             0x00000000
#define MPDU_LINK_DESC_MAX_IDX_1_RESET                               0x00000000

// 0x0080 (MPDU_LINK_EXT_DESC_SWAP)
#define MPDU_LINK_EXT_DESC_SWAP_VAL_MSB                              0
#define MPDU_LINK_EXT_DESC_SWAP_VAL_LSB                              0
#define MPDU_LINK_EXT_DESC_SWAP_VAL_MASK                             0x00000001
#define MPDU_LINK_EXT_DESC_SWAP_VAL_GET(x)                           (((x) & MPDU_LINK_EXT_DESC_SWAP_VAL_MASK) >> MPDU_LINK_EXT_DESC_SWAP_VAL_LSB)
#define MPDU_LINK_EXT_DESC_SWAP_VAL_SET(x)                           (((0x0 | (x)) << MPDU_LINK_EXT_DESC_SWAP_VAL_LSB) & MPDU_LINK_EXT_DESC_SWAP_VAL_MASK)
#define MPDU_LINK_EXT_DESC_SWAP_VAL_RESET                            0
#define MPDU_LINK_EXT_DESC_SWAP_ADDRESS                              0x000080
#define MPDU_LINK_EXT_DESC_SWAP_HW_MASK                              0x00000001
#define MPDU_LINK_EXT_DESC_SWAP_SW_MASK                              0x00000001
#define MPDU_LINK_EXT_DESC_SWAP_RSTMASK                              0x00000000
#define MPDU_LINK_EXT_DESC_SWAP_RESET                                0x00000000

// 0x0084 (MSDU_LINK_DESC_SWAP)
#define MSDU_LINK_DESC_SWAP_VAL_MSB                                  0
#define MSDU_LINK_DESC_SWAP_VAL_LSB                                  0
#define MSDU_LINK_DESC_SWAP_VAL_MASK                                 0x00000001
#define MSDU_LINK_DESC_SWAP_VAL_GET(x)                               (((x) & MSDU_LINK_DESC_SWAP_VAL_MASK) >> MSDU_LINK_DESC_SWAP_VAL_LSB)
#define MSDU_LINK_DESC_SWAP_VAL_SET(x)                               (((0x0 | (x)) << MSDU_LINK_DESC_SWAP_VAL_LSB) & MSDU_LINK_DESC_SWAP_VAL_MASK)
#define MSDU_LINK_DESC_SWAP_VAL_RESET                                0
#define MSDU_LINK_DESC_SWAP_ADDRESS                                  0x000084
#define MSDU_LINK_DESC_SWAP_HW_MASK                                  0x00000001
#define MSDU_LINK_DESC_SWAP_SW_MASK                                  0x00000001
#define MSDU_LINK_DESC_SWAP_RSTMASK                                  0x00000000
#define MSDU_LINK_DESC_SWAP_RESET                                    0x00000000

// 0x0088 (MPDU_LINK_EXT_DESC_BASE_ADDR)
#define MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_MSB                         31
#define MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_LSB                         0
#define MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_MASK                        0xffffffff
#define MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_GET(x)                      (((x) & MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_MASK) >> MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_LSB)
#define MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_SET(x)                      (((0x0 | (x)) << MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_LSB) & MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_MASK)
#define MPDU_LINK_EXT_DESC_BASE_ADDR_VAL_RESET                       0
#define MPDU_LINK_EXT_DESC_BASE_ADDR_ADDRESS                         0x000088
#define MPDU_LINK_EXT_DESC_BASE_ADDR_HW_MASK                         0xffffffff
#define MPDU_LINK_EXT_DESC_BASE_ADDR_SW_MASK                         0xffffffff
#define MPDU_LINK_EXT_DESC_BASE_ADDR_RSTMASK                         0x00000000
#define MPDU_LINK_EXT_DESC_BASE_ADDR_RESET                           0x00000000

// 0x0090 (MSDU_LINK_DESC_BASE_ADDR)
#define MSDU_LINK_DESC_BASE_ADDR_VAL_MSB                             31
#define MSDU_LINK_DESC_BASE_ADDR_VAL_LSB                             0
#define MSDU_LINK_DESC_BASE_ADDR_VAL_MASK                            0xffffffff
#define MSDU_LINK_DESC_BASE_ADDR_VAL_GET(x)                          (((x) & MSDU_LINK_DESC_BASE_ADDR_VAL_MASK) >> MSDU_LINK_DESC_BASE_ADDR_VAL_LSB)
#define MSDU_LINK_DESC_BASE_ADDR_VAL_SET(x)                          (((0x0 | (x)) << MSDU_LINK_DESC_BASE_ADDR_VAL_LSB) & MSDU_LINK_DESC_BASE_ADDR_VAL_MASK)
#define MSDU_LINK_DESC_BASE_ADDR_VAL_RESET                           0
#define MSDU_LINK_DESC_BASE_ADDR_ADDRESS                             0x000090
#define MSDU_LINK_DESC_BASE_ADDR_HW_MASK                             0xffffffff
#define MSDU_LINK_DESC_BASE_ADDR_SW_MASK                             0xffffffff
#define MSDU_LINK_DESC_BASE_ADDR_RSTMASK                             0x00000000
#define MSDU_LINK_DESC_BASE_ADDR_RESET                               0x00000000

// 0x00a0 (MSDU_LINK_EXT_DESC_BASE_ADDR)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_MSB                         31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_MASK                        0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_ADDRESS                         0x0000a0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_HW_MASK                         0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_SW_MASK                         0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_RESET                           0x00000000

// 0x00a0 (MSDU_LINK_EXT_DESC_BASE_ADDR_0)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_ADDRESS                       0x00a0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_0_RESET                         0x00000000

// 0x00a4 (MSDU_LINK_EXT_DESC_BASE_ADDR_1)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_ADDRESS                       0x00a4
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_1_RESET                         0x00000000

// 0x00a8 (MSDU_LINK_EXT_DESC_BASE_ADDR_2)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_ADDRESS                       0x00a8
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_2_RESET                         0x00000000

// 0x00ac (MSDU_LINK_EXT_DESC_BASE_ADDR_3)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_ADDRESS                       0x00ac
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_3_RESET                         0x00000000

// 0x00b0 (MSDU_LINK_EXT_DESC_BASE_ADDR_4)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_ADDRESS                       0x00b0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_4_RESET                         0x00000000

// 0x00b4 (MSDU_LINK_EXT_DESC_BASE_ADDR_5)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_ADDRESS                       0x00b4
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_5_RESET                         0x00000000

// 0x00b8 (MSDU_LINK_EXT_DESC_BASE_ADDR_6)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_ADDRESS                       0x00b8
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_6_RESET                         0x00000000

// 0x00bc (MSDU_LINK_EXT_DESC_BASE_ADDR_7)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_ADDRESS                       0x00bc
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_7_RESET                         0x00000000

// 0x00c0 (MSDU_LINK_EXT_DESC_BASE_ADDR_8)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_ADDRESS                       0x00c0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_8_RESET                         0x00000000

// 0x00c4 (MSDU_LINK_EXT_DESC_BASE_ADDR_9)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_MSB                       31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_LSB                       0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_GET(x)                    (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_SET(x)                    (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_VAL_RESET                     0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_ADDRESS                       0x00c4
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_HW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_SW_MASK                       0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_RSTMASK                       0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_9_RESET                         0x00000000

// 0x00c8 (MSDU_LINK_EXT_DESC_BASE_ADDR_10)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_MSB                      31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_LSB                      0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_MASK                     0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_GET(x)                   (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_SET(x)                   (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_VAL_RESET                    0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_ADDRESS                      0x00c8
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_HW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_SW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_RSTMASK                      0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_10_RESET                        0x00000000

// 0x00cc (MSDU_LINK_EXT_DESC_BASE_ADDR_11)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_MSB                      31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_LSB                      0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_MASK                     0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_GET(x)                   (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_SET(x)                   (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_VAL_RESET                    0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_ADDRESS                      0x00cc
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_HW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_SW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_RSTMASK                      0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_11_RESET                        0x00000000

// 0x00d0 (MSDU_LINK_EXT_DESC_BASE_ADDR_12)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_MSB                      31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_LSB                      0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_MASK                     0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_GET(x)                   (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_SET(x)                   (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_VAL_RESET                    0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_ADDRESS                      0x00d0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_HW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_SW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_RSTMASK                      0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_12_RESET                        0x00000000

// 0x00d4 (MSDU_LINK_EXT_DESC_BASE_ADDR_13)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_MSB                      31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_LSB                      0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_MASK                     0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_GET(x)                   (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_SET(x)                   (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_VAL_RESET                    0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_ADDRESS                      0x00d4
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_HW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_SW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_RSTMASK                      0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_13_RESET                        0x00000000

// 0x00d8 (MSDU_LINK_EXT_DESC_BASE_ADDR_14)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_MSB                      31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_LSB                      0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_MASK                     0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_GET(x)                   (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_SET(x)                   (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_VAL_RESET                    0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_ADDRESS                      0x00d8
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_HW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_SW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_RSTMASK                      0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_14_RESET                        0x00000000

// 0x00dc (MSDU_LINK_EXT_DESC_BASE_ADDR_15)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_MSB                      31
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_LSB                      0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_MASK                     0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_GET(x)                   (((x) & MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_MASK) >> MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_LSB)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_SET(x)                   (((0x0 | (x)) << MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_LSB) & MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_MASK)
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_VAL_RESET                    0
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_ADDRESS                      0x00dc
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_HW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_SW_MASK                      0xffffffff
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_RSTMASK                      0x00000000
#define MSDU_LINK_EXT_DESC_BASE_ADDR_15_RESET                        0x00000000

// 0x00e0 (MSDU_LINK_EXT_DESC_MIN_IDX)
#define MSDU_LINK_EXT_DESC_MIN_IDX_VAL_MSB                           15
#define MSDU_LINK_EXT_DESC_MIN_IDX_VAL_LSB                           0
#define MSDU_LINK_EXT_DESC_MIN_IDX_VAL_MASK                          0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_VAL_GET(x)                        (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_VAL_SET(x)                        (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_VAL_RESET                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_ADDRESS                           0x0000e0
#define MSDU_LINK_EXT_DESC_MIN_IDX_HW_MASK                           0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_SW_MASK                           0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_RSTMASK                           0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_RESET                             0x00000000

// 0x00e0 (MSDU_LINK_EXT_DESC_MIN_IDX_0)
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_ADDRESS                         0x00e0
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_0_RESET                           0x00000000

// 0x00e4 (MSDU_LINK_EXT_DESC_MIN_IDX_1)
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_ADDRESS                         0x00e4
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_1_RESET                           0x00000000

// 0x00e8 (MSDU_LINK_EXT_DESC_MIN_IDX_2)
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_ADDRESS                         0x00e8
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_2_RESET                           0x00000000

// 0x00ec (MSDU_LINK_EXT_DESC_MIN_IDX_3)
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_ADDRESS                         0x00ec
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_3_RESET                           0x00000000

// 0x00f0 (MSDU_LINK_EXT_DESC_MIN_IDX_4)
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_ADDRESS                         0x00f0
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_4_RESET                           0x00000000

// 0x00f4 (MSDU_LINK_EXT_DESC_MIN_IDX_5)
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_ADDRESS                         0x00f4
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_5_RESET                           0x00000000

// 0x00f8 (MSDU_LINK_EXT_DESC_MIN_IDX_6)
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_ADDRESS                         0x00f8
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_6_RESET                           0x00000000

// 0x00fc (MSDU_LINK_EXT_DESC_MIN_IDX_7)
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_ADDRESS                         0x00fc
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_7_RESET                           0x00000000

// 0x0100 (MSDU_LINK_EXT_DESC_MIN_IDX_8)
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_ADDRESS                         0x0100
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_8_RESET                           0x00000000

// 0x0104 (MSDU_LINK_EXT_DESC_MIN_IDX_9)
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_ADDRESS                         0x0104
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_9_RESET                           0x00000000

// 0x0108 (MSDU_LINK_EXT_DESC_MIN_IDX_10)
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_ADDRESS                        0x0108
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_10_RESET                          0x00000000

// 0x010c (MSDU_LINK_EXT_DESC_MIN_IDX_11)
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_ADDRESS                        0x010c
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_11_RESET                          0x00000000

// 0x0110 (MSDU_LINK_EXT_DESC_MIN_IDX_12)
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_ADDRESS                        0x0110
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_12_RESET                          0x00000000

// 0x0114 (MSDU_LINK_EXT_DESC_MIN_IDX_13)
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_ADDRESS                        0x0114
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_13_RESET                          0x00000000

// 0x0118 (MSDU_LINK_EXT_DESC_MIN_IDX_14)
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_ADDRESS                        0x0118
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_14_RESET                          0x00000000

// 0x011c (MSDU_LINK_EXT_DESC_MIN_IDX_15)
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_MASK) >> MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_LSB) & MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_ADDRESS                        0x011c
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MIN_IDX_15_RESET                          0x00000000

// 0x0120 (MSDU_LINK_EXT_DESC_MAX_IDX)
#define MSDU_LINK_EXT_DESC_MAX_IDX_VAL_MSB                           15
#define MSDU_LINK_EXT_DESC_MAX_IDX_VAL_LSB                           0
#define MSDU_LINK_EXT_DESC_MAX_IDX_VAL_MASK                          0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_VAL_GET(x)                        (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_VAL_SET(x)                        (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_VAL_RESET                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_ADDRESS                           0x000120
#define MSDU_LINK_EXT_DESC_MAX_IDX_HW_MASK                           0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_SW_MASK                           0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_RSTMASK                           0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_RESET                             0x00000000

// 0x0120 (MSDU_LINK_EXT_DESC_MAX_IDX_0)
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_ADDRESS                         0x0120
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_0_RESET                           0x00000000

// 0x0124 (MSDU_LINK_EXT_DESC_MAX_IDX_1)
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_ADDRESS                         0x0124
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_1_RESET                           0x00000000

// 0x0128 (MSDU_LINK_EXT_DESC_MAX_IDX_2)
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_ADDRESS                         0x0128
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_2_RESET                           0x00000000

// 0x012c (MSDU_LINK_EXT_DESC_MAX_IDX_3)
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_ADDRESS                         0x012c
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_3_RESET                           0x00000000

// 0x0130 (MSDU_LINK_EXT_DESC_MAX_IDX_4)
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_ADDRESS                         0x0130
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_4_RESET                           0x00000000

// 0x0134 (MSDU_LINK_EXT_DESC_MAX_IDX_5)
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_ADDRESS                         0x0134
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_5_RESET                           0x00000000

// 0x0138 (MSDU_LINK_EXT_DESC_MAX_IDX_6)
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_ADDRESS                         0x0138
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_6_RESET                           0x00000000

// 0x013c (MSDU_LINK_EXT_DESC_MAX_IDX_7)
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_ADDRESS                         0x013c
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_7_RESET                           0x00000000

// 0x0140 (MSDU_LINK_EXT_DESC_MAX_IDX_8)
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_ADDRESS                         0x0140
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_8_RESET                           0x00000000

// 0x0144 (MSDU_LINK_EXT_DESC_MAX_IDX_9)
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_MSB                         15
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_LSB                         0
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_GET(x)                      (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_SET(x)                      (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_VAL_RESET                       0
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_ADDRESS                         0x0144
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_HW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_SW_MASK                         0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_RSTMASK                         0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_9_RESET                           0x00000000

// 0x0148 (MSDU_LINK_EXT_DESC_MAX_IDX_10)
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_ADDRESS                        0x0148
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_10_RESET                          0x00000000

// 0x014c (MSDU_LINK_EXT_DESC_MAX_IDX_11)
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_ADDRESS                        0x014c
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_11_RESET                          0x00000000

// 0x0150 (MSDU_LINK_EXT_DESC_MAX_IDX_12)
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_ADDRESS                        0x0150
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_12_RESET                          0x00000000

// 0x0154 (MSDU_LINK_EXT_DESC_MAX_IDX_13)
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_ADDRESS                        0x0154
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_13_RESET                          0x00000000

// 0x0158 (MSDU_LINK_EXT_DESC_MAX_IDX_14)
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_ADDRESS                        0x0158
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_14_RESET                          0x00000000

// 0x015c (MSDU_LINK_EXT_DESC_MAX_IDX_15)
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_MSB                        15
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_LSB                        0
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_MASK                       0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_GET(x)                     (((x) & MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_MASK) >> MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_LSB)
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_SET(x)                     (((0x0 | (x)) << MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_LSB) & MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_MASK)
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_VAL_RESET                      0
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_ADDRESS                        0x015c
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_HW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_SW_MASK                        0x0000ffff
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_RSTMASK                        0x00000000
#define MSDU_LINK_EXT_DESC_MAX_IDX_15_RESET                          0x00000000

// 0x0160 (MSDU_LINK_EXT_DESC_SWAP)
#define MSDU_LINK_EXT_DESC_SWAP_VAL_MSB                              0
#define MSDU_LINK_EXT_DESC_SWAP_VAL_LSB                              0
#define MSDU_LINK_EXT_DESC_SWAP_VAL_MASK                             0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_VAL_GET(x)                           (((x) & MSDU_LINK_EXT_DESC_SWAP_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_VAL_SET(x)                           (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_VAL_RESET                            0
#define MSDU_LINK_EXT_DESC_SWAP_ADDRESS                              0x000160
#define MSDU_LINK_EXT_DESC_SWAP_HW_MASK                              0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_SW_MASK                              0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_RSTMASK                              0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_RESET                                0x00000000

// 0x0160 (MSDU_LINK_EXT_DESC_SWAP_0)
#define MSDU_LINK_EXT_DESC_SWAP_0_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_0_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_0_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_0_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_0_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_0_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_0_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_0_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_0_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_0_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_0_ADDRESS                            0x0160
#define MSDU_LINK_EXT_DESC_SWAP_0_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_0_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_0_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_0_RESET                              0x00000000

// 0x0164 (MSDU_LINK_EXT_DESC_SWAP_1)
#define MSDU_LINK_EXT_DESC_SWAP_1_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_1_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_1_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_1_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_1_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_1_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_1_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_1_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_1_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_1_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_1_ADDRESS                            0x0164
#define MSDU_LINK_EXT_DESC_SWAP_1_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_1_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_1_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_1_RESET                              0x00000000

// 0x0168 (MSDU_LINK_EXT_DESC_SWAP_2)
#define MSDU_LINK_EXT_DESC_SWAP_2_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_2_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_2_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_2_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_2_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_2_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_2_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_2_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_2_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_2_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_2_ADDRESS                            0x0168
#define MSDU_LINK_EXT_DESC_SWAP_2_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_2_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_2_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_2_RESET                              0x00000000

// 0x016c (MSDU_LINK_EXT_DESC_SWAP_3)
#define MSDU_LINK_EXT_DESC_SWAP_3_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_3_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_3_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_3_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_3_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_3_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_3_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_3_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_3_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_3_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_3_ADDRESS                            0x016c
#define MSDU_LINK_EXT_DESC_SWAP_3_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_3_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_3_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_3_RESET                              0x00000000

// 0x0170 (MSDU_LINK_EXT_DESC_SWAP_4)
#define MSDU_LINK_EXT_DESC_SWAP_4_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_4_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_4_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_4_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_4_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_4_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_4_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_4_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_4_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_4_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_4_ADDRESS                            0x0170
#define MSDU_LINK_EXT_DESC_SWAP_4_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_4_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_4_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_4_RESET                              0x00000000

// 0x0174 (MSDU_LINK_EXT_DESC_SWAP_5)
#define MSDU_LINK_EXT_DESC_SWAP_5_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_5_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_5_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_5_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_5_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_5_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_5_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_5_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_5_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_5_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_5_ADDRESS                            0x0174
#define MSDU_LINK_EXT_DESC_SWAP_5_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_5_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_5_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_5_RESET                              0x00000000

// 0x0178 (MSDU_LINK_EXT_DESC_SWAP_6)
#define MSDU_LINK_EXT_DESC_SWAP_6_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_6_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_6_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_6_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_6_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_6_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_6_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_6_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_6_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_6_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_6_ADDRESS                            0x0178
#define MSDU_LINK_EXT_DESC_SWAP_6_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_6_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_6_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_6_RESET                              0x00000000

// 0x017c (MSDU_LINK_EXT_DESC_SWAP_7)
#define MSDU_LINK_EXT_DESC_SWAP_7_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_7_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_7_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_7_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_7_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_7_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_7_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_7_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_7_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_7_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_7_ADDRESS                            0x017c
#define MSDU_LINK_EXT_DESC_SWAP_7_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_7_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_7_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_7_RESET                              0x00000000

// 0x0180 (MSDU_LINK_EXT_DESC_SWAP_8)
#define MSDU_LINK_EXT_DESC_SWAP_8_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_8_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_8_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_8_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_8_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_8_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_8_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_8_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_8_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_8_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_8_ADDRESS                            0x0180
#define MSDU_LINK_EXT_DESC_SWAP_8_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_8_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_8_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_8_RESET                              0x00000000

// 0x0184 (MSDU_LINK_EXT_DESC_SWAP_9)
#define MSDU_LINK_EXT_DESC_SWAP_9_VAL_MSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_9_VAL_LSB                            0
#define MSDU_LINK_EXT_DESC_SWAP_9_VAL_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_9_VAL_GET(x)                         (((x) & MSDU_LINK_EXT_DESC_SWAP_9_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_9_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_9_VAL_SET(x)                         (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_9_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_9_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_9_VAL_RESET                          0
#define MSDU_LINK_EXT_DESC_SWAP_9_ADDRESS                            0x0184
#define MSDU_LINK_EXT_DESC_SWAP_9_HW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_9_SW_MASK                            0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_9_RSTMASK                            0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_9_RESET                              0x00000000

// 0x0188 (MSDU_LINK_EXT_DESC_SWAP_10)
#define MSDU_LINK_EXT_DESC_SWAP_10_VAL_MSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_10_VAL_LSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_10_VAL_MASK                          0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_10_VAL_GET(x)                        (((x) & MSDU_LINK_EXT_DESC_SWAP_10_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_10_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_10_VAL_SET(x)                        (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_10_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_10_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_10_VAL_RESET                         0
#define MSDU_LINK_EXT_DESC_SWAP_10_ADDRESS                           0x0188
#define MSDU_LINK_EXT_DESC_SWAP_10_HW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_10_SW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_10_RSTMASK                           0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_10_RESET                             0x00000000

// 0x018c (MSDU_LINK_EXT_DESC_SWAP_11)
#define MSDU_LINK_EXT_DESC_SWAP_11_VAL_MSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_11_VAL_LSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_11_VAL_MASK                          0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_11_VAL_GET(x)                        (((x) & MSDU_LINK_EXT_DESC_SWAP_11_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_11_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_11_VAL_SET(x)                        (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_11_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_11_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_11_VAL_RESET                         0
#define MSDU_LINK_EXT_DESC_SWAP_11_ADDRESS                           0x018c
#define MSDU_LINK_EXT_DESC_SWAP_11_HW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_11_SW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_11_RSTMASK                           0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_11_RESET                             0x00000000

// 0x0190 (MSDU_LINK_EXT_DESC_SWAP_12)
#define MSDU_LINK_EXT_DESC_SWAP_12_VAL_MSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_12_VAL_LSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_12_VAL_MASK                          0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_12_VAL_GET(x)                        (((x) & MSDU_LINK_EXT_DESC_SWAP_12_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_12_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_12_VAL_SET(x)                        (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_12_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_12_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_12_VAL_RESET                         0
#define MSDU_LINK_EXT_DESC_SWAP_12_ADDRESS                           0x0190
#define MSDU_LINK_EXT_DESC_SWAP_12_HW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_12_SW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_12_RSTMASK                           0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_12_RESET                             0x00000000

// 0x0194 (MSDU_LINK_EXT_DESC_SWAP_13)
#define MSDU_LINK_EXT_DESC_SWAP_13_VAL_MSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_13_VAL_LSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_13_VAL_MASK                          0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_13_VAL_GET(x)                        (((x) & MSDU_LINK_EXT_DESC_SWAP_13_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_13_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_13_VAL_SET(x)                        (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_13_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_13_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_13_VAL_RESET                         0
#define MSDU_LINK_EXT_DESC_SWAP_13_ADDRESS                           0x0194
#define MSDU_LINK_EXT_DESC_SWAP_13_HW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_13_SW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_13_RSTMASK                           0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_13_RESET                             0x00000000

// 0x0198 (MSDU_LINK_EXT_DESC_SWAP_14)
#define MSDU_LINK_EXT_DESC_SWAP_14_VAL_MSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_14_VAL_LSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_14_VAL_MASK                          0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_14_VAL_GET(x)                        (((x) & MSDU_LINK_EXT_DESC_SWAP_14_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_14_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_14_VAL_SET(x)                        (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_14_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_14_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_14_VAL_RESET                         0
#define MSDU_LINK_EXT_DESC_SWAP_14_ADDRESS                           0x0198
#define MSDU_LINK_EXT_DESC_SWAP_14_HW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_14_SW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_14_RSTMASK                           0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_14_RESET                             0x00000000

// 0x019c (MSDU_LINK_EXT_DESC_SWAP_15)
#define MSDU_LINK_EXT_DESC_SWAP_15_VAL_MSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_15_VAL_LSB                           0
#define MSDU_LINK_EXT_DESC_SWAP_15_VAL_MASK                          0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_15_VAL_GET(x)                        (((x) & MSDU_LINK_EXT_DESC_SWAP_15_VAL_MASK) >> MSDU_LINK_EXT_DESC_SWAP_15_VAL_LSB)
#define MSDU_LINK_EXT_DESC_SWAP_15_VAL_SET(x)                        (((0x0 | (x)) << MSDU_LINK_EXT_DESC_SWAP_15_VAL_LSB) & MSDU_LINK_EXT_DESC_SWAP_15_VAL_MASK)
#define MSDU_LINK_EXT_DESC_SWAP_15_VAL_RESET                         0
#define MSDU_LINK_EXT_DESC_SWAP_15_ADDRESS                           0x019c
#define MSDU_LINK_EXT_DESC_SWAP_15_HW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_15_SW_MASK                           0x00000001
#define MSDU_LINK_EXT_DESC_SWAP_15_RSTMASK                           0x00000000
#define MSDU_LINK_EXT_DESC_SWAP_15_RESET                             0x00000000

// 0x01a0 (MAX_AXI_RD_BURST_SIZE)
#define MAX_AXI_RD_BURST_SIZE_VAL_MSB                                15
#define MAX_AXI_RD_BURST_SIZE_VAL_LSB                                0
#define MAX_AXI_RD_BURST_SIZE_VAL_MASK                               0x0000ffff
#define MAX_AXI_RD_BURST_SIZE_VAL_GET(x)                             (((x) & MAX_AXI_RD_BURST_SIZE_VAL_MASK) >> MAX_AXI_RD_BURST_SIZE_VAL_LSB)
#define MAX_AXI_RD_BURST_SIZE_VAL_SET(x)                             (((0x0 | (x)) << MAX_AXI_RD_BURST_SIZE_VAL_LSB) & MAX_AXI_RD_BURST_SIZE_VAL_MASK)
#define MAX_AXI_RD_BURST_SIZE_VAL_RESET                              128
#define MAX_AXI_RD_BURST_SIZE_ADDRESS                                0x0001a0
#define MAX_AXI_RD_BURST_SIZE_HW_MASK                                0x0000ffff
#define MAX_AXI_RD_BURST_SIZE_SW_MASK                                0x0000ffff
#define MAX_AXI_RD_BURST_SIZE_RSTMASK                                0x0000ffff
#define MAX_AXI_RD_BURST_SIZE_RESET                                  0x00000080

// 0x01a4 (MAC_PCU_STA_ADDR_L32)
#define MAC_PCU_STA_ADDR_L32_ADDR_31_0_MSB                           31
#define MAC_PCU_STA_ADDR_L32_ADDR_31_0_LSB                           0
#define MAC_PCU_STA_ADDR_L32_ADDR_31_0_MASK                          0xffffffff
#define MAC_PCU_STA_ADDR_L32_ADDR_31_0_GET(x)                        (((x) & MAC_PCU_STA_ADDR_L32_ADDR_31_0_MASK) >> MAC_PCU_STA_ADDR_L32_ADDR_31_0_LSB)
#define MAC_PCU_STA_ADDR_L32_ADDR_31_0_SET(x)                        (((0x0 | (x)) << MAC_PCU_STA_ADDR_L32_ADDR_31_0_LSB) & MAC_PCU_STA_ADDR_L32_ADDR_31_0_MASK)
#define MAC_PCU_STA_ADDR_L32_ADDR_31_0_RESET                         0
#define MAC_PCU_STA_ADDR_L32_ADDRESS                                 0x0001a4
#define MAC_PCU_STA_ADDR_L32_HW_MASK                                 0xffffffff
#define MAC_PCU_STA_ADDR_L32_SW_MASK                                 0xffffffff
#define MAC_PCU_STA_ADDR_L32_RSTMASK                                 0x00000000
#define MAC_PCU_STA_ADDR_L32_RESET                                   0x00000000

// 0x01a8 (MAC_PCU_STA_ADDR_U16)
#define MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_MSB                  31
#define MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_LSB                  31
#define MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_MASK                 0x80000000
#define MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_GET(x)               (((x) & MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_MASK) >> MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_LSB)
#define MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_SET(x)               (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_LSB) & MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_MASK)
#define MAC_PCU_STA_ADDR_U16_ADHOC_MCAST_SEARCH_RESET                0
#define MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_MSB                        30
#define MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_LSB                        30
#define MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_MASK                       0x40000000
#define MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_GET(x)                     (((x) & MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_MASK) >> MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_LSB)
#define MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_SET(x)                     (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_LSB) & MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_MASK)
#define MAC_PCU_STA_ADDR_U16_CBCIV_ENDIAN_RESET                      0
#define MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_MSB                     29
#define MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_LSB                     29
#define MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_MASK                    0x20000000
#define MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_GET(x)                  (((x) & MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_MASK) >> MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_LSB)
#define MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_SET(x)                  (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_LSB) & MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_MASK)
#define MAC_PCU_STA_ADDR_U16_PRESERVE_SEQNUM_RESET                   1
#define MAC_PCU_STA_ADDR_U16_KSRCH_MODE_MSB                          28
#define MAC_PCU_STA_ADDR_U16_KSRCH_MODE_LSB                          28
#define MAC_PCU_STA_ADDR_U16_KSRCH_MODE_MASK                         0x10000000
#define MAC_PCU_STA_ADDR_U16_KSRCH_MODE_GET(x)                       (((x) & MAC_PCU_STA_ADDR_U16_KSRCH_MODE_MASK) >> MAC_PCU_STA_ADDR_U16_KSRCH_MODE_LSB)
#define MAC_PCU_STA_ADDR_U16_KSRCH_MODE_SET(x)                       (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_KSRCH_MODE_LSB) & MAC_PCU_STA_ADDR_U16_KSRCH_MODE_MASK)
#define MAC_PCU_STA_ADDR_U16_KSRCH_MODE_RESET                        0
#define MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_MSB                     27
#define MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_LSB                     27
#define MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_MASK                    0x08000000
#define MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_GET(x)                  (((x) & MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_MASK) >> MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_LSB)
#define MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_SET(x)                  (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_LSB) & MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_MASK)
#define MAC_PCU_STA_ADDR_U16_CRPT_MIC_ENABLE_RESET                   0
#define MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_MSB                     26
#define MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_LSB                     26
#define MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_MASK                    0x04000000
#define MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_GET(x)                  (((x) & MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_MASK) >> MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_LSB)
#define MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_SET(x)                  (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_LSB) & MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_MASK)
#define MAC_PCU_STA_ADDR_U16_SECTOR_SELF_GEN_RESET                   0
#define MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_MSB                       25
#define MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_LSB                       25
#define MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_MASK                      0x02000000
#define MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_GET(x)                    (((x) & MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_MASK) >> MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_LSB)
#define MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_SET(x)                    (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_LSB) & MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_MASK)
#define MAC_PCU_STA_ADDR_U16_BASE_RATE_11B_RESET                     0
#define MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_MSB                          24
#define MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_LSB                          24
#define MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_MASK                         0x01000000
#define MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_GET(x)                       (((x) & MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_MASK) >> MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_LSB)
#define MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_SET(x)                       (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_LSB) & MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_MASK)
#define MAC_PCU_STA_ADDR_U16_ACKCTS_6MB_RESET                        0
#define MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_MSB                         23
#define MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_LSB                         23
#define MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_MASK                        0x00800000
#define MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_GET(x)                      (((x) & MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_MASK) >> MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_LSB)
#define MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_SET(x)                      (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_LSB) & MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_MASK)
#define MAC_PCU_STA_ADDR_U16_RTS_USE_DEF_RESET                       0
#define MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_MSB                       22
#define MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_LSB                       22
#define MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_MASK                      0x00400000
#define MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_GET(x)                    (((x) & MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_MASK) >> MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_LSB)
#define MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_SET(x)                    (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_LSB) & MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_MASK)
#define MAC_PCU_STA_ADDR_U16_DEFANT_UPDATE_RESET                     0
#define MAC_PCU_STA_ADDR_U16_USE_DEFANT_MSB                          21
#define MAC_PCU_STA_ADDR_U16_USE_DEFANT_LSB                          21
#define MAC_PCU_STA_ADDR_U16_USE_DEFANT_MASK                         0x00200000
#define MAC_PCU_STA_ADDR_U16_USE_DEFANT_GET(x)                       (((x) & MAC_PCU_STA_ADDR_U16_USE_DEFANT_MASK) >> MAC_PCU_STA_ADDR_U16_USE_DEFANT_LSB)
#define MAC_PCU_STA_ADDR_U16_USE_DEFANT_SET(x)                       (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_USE_DEFANT_LSB) & MAC_PCU_STA_ADDR_U16_USE_DEFANT_MASK)
#define MAC_PCU_STA_ADDR_U16_USE_DEFANT_RESET                        0
#define MAC_PCU_STA_ADDR_U16_PCF_MSB                                 20
#define MAC_PCU_STA_ADDR_U16_PCF_LSB                                 20
#define MAC_PCU_STA_ADDR_U16_PCF_MASK                                0x00100000
#define MAC_PCU_STA_ADDR_U16_PCF_GET(x)                              (((x) & MAC_PCU_STA_ADDR_U16_PCF_MASK) >> MAC_PCU_STA_ADDR_U16_PCF_LSB)
#define MAC_PCU_STA_ADDR_U16_PCF_SET(x)                              (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_PCF_LSB) & MAC_PCU_STA_ADDR_U16_PCF_MASK)
#define MAC_PCU_STA_ADDR_U16_PCF_RESET                               0
#define MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_MSB                         19
#define MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_LSB                         19
#define MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_MASK                        0x00080000
#define MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_GET(x)                      (((x) & MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_MASK) >> MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_LSB)
#define MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_SET(x)                      (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_LSB) & MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_MASK)
#define MAC_PCU_STA_ADDR_U16_KEYSRCH_DIS_RESET                       0
#define MAC_PCU_STA_ADDR_U16_PW_SAVE_MSB                             18
#define MAC_PCU_STA_ADDR_U16_PW_SAVE_LSB                             18
#define MAC_PCU_STA_ADDR_U16_PW_SAVE_MASK                            0x00040000
#define MAC_PCU_STA_ADDR_U16_PW_SAVE_GET(x)                          (((x) & MAC_PCU_STA_ADDR_U16_PW_SAVE_MASK) >> MAC_PCU_STA_ADDR_U16_PW_SAVE_LSB)
#define MAC_PCU_STA_ADDR_U16_PW_SAVE_SET(x)                          (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_PW_SAVE_LSB) & MAC_PCU_STA_ADDR_U16_PW_SAVE_MASK)
#define MAC_PCU_STA_ADDR_U16_PW_SAVE_RESET                           0
#define MAC_PCU_STA_ADDR_U16_ADHOC_MSB                               17
#define MAC_PCU_STA_ADDR_U16_ADHOC_LSB                               17
#define MAC_PCU_STA_ADDR_U16_ADHOC_MASK                              0x00020000
#define MAC_PCU_STA_ADDR_U16_ADHOC_GET(x)                            (((x) & MAC_PCU_STA_ADDR_U16_ADHOC_MASK) >> MAC_PCU_STA_ADDR_U16_ADHOC_LSB)
#define MAC_PCU_STA_ADDR_U16_ADHOC_SET(x)                            (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_ADHOC_LSB) & MAC_PCU_STA_ADDR_U16_ADHOC_MASK)
#define MAC_PCU_STA_ADDR_U16_ADHOC_RESET                             0
#define MAC_PCU_STA_ADDR_U16_STA_AP_MSB                              16
#define MAC_PCU_STA_ADDR_U16_STA_AP_LSB                              16
#define MAC_PCU_STA_ADDR_U16_STA_AP_MASK                             0x00010000
#define MAC_PCU_STA_ADDR_U16_STA_AP_GET(x)                           (((x) & MAC_PCU_STA_ADDR_U16_STA_AP_MASK) >> MAC_PCU_STA_ADDR_U16_STA_AP_LSB)
#define MAC_PCU_STA_ADDR_U16_STA_AP_SET(x)                           (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_STA_AP_LSB) & MAC_PCU_STA_ADDR_U16_STA_AP_MASK)
#define MAC_PCU_STA_ADDR_U16_STA_AP_RESET                            0
#define MAC_PCU_STA_ADDR_U16_ADDR_47_32_MSB                          15
#define MAC_PCU_STA_ADDR_U16_ADDR_47_32_LSB                          0
#define MAC_PCU_STA_ADDR_U16_ADDR_47_32_MASK                         0x0000ffff
#define MAC_PCU_STA_ADDR_U16_ADDR_47_32_GET(x)                       (((x) & MAC_PCU_STA_ADDR_U16_ADDR_47_32_MASK) >> MAC_PCU_STA_ADDR_U16_ADDR_47_32_LSB)
#define MAC_PCU_STA_ADDR_U16_ADDR_47_32_SET(x)                       (((0x0 | (x)) << MAC_PCU_STA_ADDR_U16_ADDR_47_32_LSB) & MAC_PCU_STA_ADDR_U16_ADDR_47_32_MASK)
#define MAC_PCU_STA_ADDR_U16_ADDR_47_32_RESET                        0
#define MAC_PCU_STA_ADDR_U16_ADDRESS                                 0x0001a8
#define MAC_PCU_STA_ADDR_U16_HW_MASK                                 0xffffffff
#define MAC_PCU_STA_ADDR_U16_SW_MASK                                 0xffffffff
#define MAC_PCU_STA_ADDR_U16_RSTMASK                                 0x20000000
#define MAC_PCU_STA_ADDR_U16_RESET                                   0x20000000

// 0x01ac (MAC_PCU_BSSID_L32)
#define MAC_PCU_BSSID_L32_ADDR_MSB                                   31
#define MAC_PCU_BSSID_L32_ADDR_LSB                                   0
#define MAC_PCU_BSSID_L32_ADDR_MASK                                  0xffffffff
#define MAC_PCU_BSSID_L32_ADDR_GET(x)                                (((x) & MAC_PCU_BSSID_L32_ADDR_MASK) >> MAC_PCU_BSSID_L32_ADDR_LSB)
#define MAC_PCU_BSSID_L32_ADDR_SET(x)                                (((0x0 | (x)) << MAC_PCU_BSSID_L32_ADDR_LSB) & MAC_PCU_BSSID_L32_ADDR_MASK)
#define MAC_PCU_BSSID_L32_ADDR_RESET                                 0
#define MAC_PCU_BSSID_L32_ADDRESS                                    0x0001ac
#define MAC_PCU_BSSID_L32_HW_MASK                                    0xffffffff
#define MAC_PCU_BSSID_L32_SW_MASK                                    0xffffffff
#define MAC_PCU_BSSID_L32_RSTMASK                                    0x00000000
#define MAC_PCU_BSSID_L32_RESET                                      0x00000000

// 0x01b0 (MAC_PCU_BSSID_U16)
#define MAC_PCU_BSSID_U16_AID_MSB                                    26
#define MAC_PCU_BSSID_U16_AID_LSB                                    16
#define MAC_PCU_BSSID_U16_AID_MASK                                   0x07ff0000
#define MAC_PCU_BSSID_U16_AID_GET(x)                                 (((x) & MAC_PCU_BSSID_U16_AID_MASK) >> MAC_PCU_BSSID_U16_AID_LSB)
#define MAC_PCU_BSSID_U16_AID_SET(x)                                 (((0x0 | (x)) << MAC_PCU_BSSID_U16_AID_LSB) & MAC_PCU_BSSID_U16_AID_MASK)
#define MAC_PCU_BSSID_U16_AID_RESET                                  0
#define MAC_PCU_BSSID_U16_ADDR_MSB                                   15
#define MAC_PCU_BSSID_U16_ADDR_LSB                                   0
#define MAC_PCU_BSSID_U16_ADDR_MASK                                  0x0000ffff
#define MAC_PCU_BSSID_U16_ADDR_GET(x)                                (((x) & MAC_PCU_BSSID_U16_ADDR_MASK) >> MAC_PCU_BSSID_U16_ADDR_LSB)
#define MAC_PCU_BSSID_U16_ADDR_SET(x)                                (((0x0 | (x)) << MAC_PCU_BSSID_U16_ADDR_LSB) & MAC_PCU_BSSID_U16_ADDR_MASK)
#define MAC_PCU_BSSID_U16_ADDR_RESET                                 0
#define MAC_PCU_BSSID_U16_ADDRESS                                    0x0001b0
#define MAC_PCU_BSSID_U16_HW_MASK                                    0x07ffffff
#define MAC_PCU_BSSID_U16_SW_MASK                                    0x07ffffff
#define MAC_PCU_BSSID_U16_RSTMASK                                    0x00000000
#define MAC_PCU_BSSID_U16_RESET                                      0x00000000

// 0x01b4 (MAC_PCU_DIAG_SW)
#define MAC_PCU_DIAG_SW_DEBUG_MODE_MSB                               31
#define MAC_PCU_DIAG_SW_DEBUG_MODE_LSB                               30
#define MAC_PCU_DIAG_SW_DEBUG_MODE_MASK                              0xc0000000
#define MAC_PCU_DIAG_SW_DEBUG_MODE_GET(x)                            (((x) & MAC_PCU_DIAG_SW_DEBUG_MODE_MASK) >> MAC_PCU_DIAG_SW_DEBUG_MODE_LSB)
#define MAC_PCU_DIAG_SW_DEBUG_MODE_SET(x)                            (((0x0 | (x)) << MAC_PCU_DIAG_SW_DEBUG_MODE_LSB) & MAC_PCU_DIAG_SW_DEBUG_MODE_MASK)
#define MAC_PCU_DIAG_SW_DEBUG_MODE_RESET                             0
#define MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_MSB                         29
#define MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_LSB                         29
#define MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_MASK                        0x20000000
#define MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_GET(x)                      (((x) & MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_MASK) >> MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_LSB)
#define MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_SET(x)                      (((0x0 | (x)) << MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_LSB) & MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_MASK)
#define MAC_PCU_DIAG_SW_RX_CLEAR_EXT_LOW_RESET                       0
#define MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_MSB                         28
#define MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_LSB                         28
#define MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_MASK                        0x10000000
#define MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_GET(x)                      (((x) & MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_MASK) >> MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_LSB)
#define MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_SET(x)                      (((0x0 | (x)) << MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_LSB) & MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_MASK)
#define MAC_PCU_DIAG_SW_RX_CLEAR_CTL_LOW_RESET                       0
#define MAC_PCU_DIAG_SW_OBS_SEL_2_MSB                                27
#define MAC_PCU_DIAG_SW_OBS_SEL_2_LSB                                27
#define MAC_PCU_DIAG_SW_OBS_SEL_2_MASK                               0x08000000
#define MAC_PCU_DIAG_SW_OBS_SEL_2_GET(x)                             (((x) & MAC_PCU_DIAG_SW_OBS_SEL_2_MASK) >> MAC_PCU_DIAG_SW_OBS_SEL_2_LSB)
#define MAC_PCU_DIAG_SW_OBS_SEL_2_SET(x)                             (((0x0 | (x)) << MAC_PCU_DIAG_SW_OBS_SEL_2_LSB) & MAC_PCU_DIAG_SW_OBS_SEL_2_MASK)
#define MAC_PCU_DIAG_SW_OBS_SEL_2_RESET                              0
#define MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_MSB                       26
#define MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_LSB                       26
#define MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_MASK                      0x04000000
#define MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_GET(x)                    (((x) & MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_MASK) >> MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_LSB)
#define MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_SET(x)                    (((0x0 | (x)) << MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_LSB) & MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_MASK)
#define MAC_PCU_DIAG_SW_SATURATE_CYCLE_CNT_RESET                     0
#define MAC_PCU_DIAG_SW_FORCE_RX_ABORT_MSB                           25
#define MAC_PCU_DIAG_SW_FORCE_RX_ABORT_LSB                           25
#define MAC_PCU_DIAG_SW_FORCE_RX_ABORT_MASK                          0x02000000
#define MAC_PCU_DIAG_SW_FORCE_RX_ABORT_GET(x)                        (((x) & MAC_PCU_DIAG_SW_FORCE_RX_ABORT_MASK) >> MAC_PCU_DIAG_SW_FORCE_RX_ABORT_LSB)
#define MAC_PCU_DIAG_SW_FORCE_RX_ABORT_SET(x)                        (((0x0 | (x)) << MAC_PCU_DIAG_SW_FORCE_RX_ABORT_LSB) & MAC_PCU_DIAG_SW_FORCE_RX_ABORT_MASK)
#define MAC_PCU_DIAG_SW_FORCE_RX_ABORT_RESET                         0
#define MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_MSB                     24
#define MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_LSB                     24
#define MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_MASK                    0x01000000
#define MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_GET(x)                  (((x) & MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_MASK) >> MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_LSB)
#define MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_SET(x)                  (((0x0 | (x)) << MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_LSB) & MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_MASK)
#define MAC_PCU_DIAG_SW_DUAL_CHAIN_CHAN_INFO_RESET                   0
#define MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_MSB                   23
#define MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_LSB                   23
#define MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_MASK                  0x00800000
#define MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_GET(x)                (((x) & MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_MASK) >> MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_LSB)
#define MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_SET(x)                (((0x0 | (x)) << MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_LSB) & MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_MASK)
#define MAC_PCU_DIAG_SW_PHYERR_ENABLE_EIFS_CTL_RESET                 0
#define MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_MSB                           22
#define MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_LSB                           22
#define MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_MASK                          0x00400000
#define MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_GET(x)                        (((x) & MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_MASK) >> MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_LSB)
#define MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_SET(x)                        (((0x0 | (x)) << MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_LSB) & MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_MASK)
#define MAC_PCU_DIAG_SW_CHAN_IDLE_HIGH_RESET                         0
#define MAC_PCU_DIAG_SW_IGNORE_NAV_MSB                               21
#define MAC_PCU_DIAG_SW_IGNORE_NAV_LSB                               21
#define MAC_PCU_DIAG_SW_IGNORE_NAV_MASK                              0x00200000
#define MAC_PCU_DIAG_SW_IGNORE_NAV_GET(x)                            (((x) & MAC_PCU_DIAG_SW_IGNORE_NAV_MASK) >> MAC_PCU_DIAG_SW_IGNORE_NAV_LSB)
#define MAC_PCU_DIAG_SW_IGNORE_NAV_SET(x)                            (((0x0 | (x)) << MAC_PCU_DIAG_SW_IGNORE_NAV_LSB) & MAC_PCU_DIAG_SW_IGNORE_NAV_MASK)
#define MAC_PCU_DIAG_SW_IGNORE_NAV_RESET                             0
#define MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_MSB                            20
#define MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_LSB                            20
#define MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_MASK                           0x00100000
#define MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_GET(x)                         (((x) & MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_MASK) >> MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_LSB)
#define MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_SET(x)                         (((0x0 | (x)) << MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_LSB) & MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_MASK)
#define MAC_PCU_DIAG_SW_RX_CLEAR_HIGH_RESET                          0
#define MAC_PCU_DIAG_SW_OBS_SEL_1_0_MSB                              19
#define MAC_PCU_DIAG_SW_OBS_SEL_1_0_LSB                              18
#define MAC_PCU_DIAG_SW_OBS_SEL_1_0_MASK                             0x000c0000
#define MAC_PCU_DIAG_SW_OBS_SEL_1_0_GET(x)                           (((x) & MAC_PCU_DIAG_SW_OBS_SEL_1_0_MASK) >> MAC_PCU_DIAG_SW_OBS_SEL_1_0_LSB)
#define MAC_PCU_DIAG_SW_OBS_SEL_1_0_SET(x)                           (((0x0 | (x)) << MAC_PCU_DIAG_SW_OBS_SEL_1_0_LSB) & MAC_PCU_DIAG_SW_OBS_SEL_1_0_MASK)
#define MAC_PCU_DIAG_SW_OBS_SEL_1_0_RESET                            0
#define MAC_PCU_DIAG_SW_ACCEPT_NON_V0_MSB                            17
#define MAC_PCU_DIAG_SW_ACCEPT_NON_V0_LSB                            17
#define MAC_PCU_DIAG_SW_ACCEPT_NON_V0_MASK                           0x00020000
#define MAC_PCU_DIAG_SW_ACCEPT_NON_V0_GET(x)                         (((x) & MAC_PCU_DIAG_SW_ACCEPT_NON_V0_MASK) >> MAC_PCU_DIAG_SW_ACCEPT_NON_V0_LSB)
#define MAC_PCU_DIAG_SW_ACCEPT_NON_V0_SET(x)                         (((0x0 | (x)) << MAC_PCU_DIAG_SW_ACCEPT_NON_V0_LSB) & MAC_PCU_DIAG_SW_ACCEPT_NON_V0_MASK)
#define MAC_PCU_DIAG_SW_ACCEPT_NON_V0_RESET                          0
#define MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_MSB                           8
#define MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_LSB                           8
#define MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_MASK                          0x00000100
#define MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_GET(x)                        (((x) & MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_MASK) >> MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_LSB)
#define MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_SET(x)                        (((0x0 | (x)) << MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_LSB) & MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_MASK)
#define MAC_PCU_DIAG_SW_DUMP_CHAN_INFO_RESET                         0
#define MAC_PCU_DIAG_SW_CORRUPT_FCS_MSB                              7
#define MAC_PCU_DIAG_SW_CORRUPT_FCS_LSB                              7
#define MAC_PCU_DIAG_SW_CORRUPT_FCS_MASK                             0x00000080
#define MAC_PCU_DIAG_SW_CORRUPT_FCS_GET(x)                           (((x) & MAC_PCU_DIAG_SW_CORRUPT_FCS_MASK) >> MAC_PCU_DIAG_SW_CORRUPT_FCS_LSB)
#define MAC_PCU_DIAG_SW_CORRUPT_FCS_SET(x)                           (((0x0 | (x)) << MAC_PCU_DIAG_SW_CORRUPT_FCS_LSB) & MAC_PCU_DIAG_SW_CORRUPT_FCS_MASK)
#define MAC_PCU_DIAG_SW_CORRUPT_FCS_RESET                            0
#define MAC_PCU_DIAG_SW_LOOP_BACK_MSB                                6
#define MAC_PCU_DIAG_SW_LOOP_BACK_LSB                                6
#define MAC_PCU_DIAG_SW_LOOP_BACK_MASK                               0x00000040
#define MAC_PCU_DIAG_SW_LOOP_BACK_GET(x)                             (((x) & MAC_PCU_DIAG_SW_LOOP_BACK_MASK) >> MAC_PCU_DIAG_SW_LOOP_BACK_LSB)
#define MAC_PCU_DIAG_SW_LOOP_BACK_SET(x)                             (((0x0 | (x)) << MAC_PCU_DIAG_SW_LOOP_BACK_LSB) & MAC_PCU_DIAG_SW_LOOP_BACK_MASK)
#define MAC_PCU_DIAG_SW_LOOP_BACK_RESET                              0
#define MAC_PCU_DIAG_SW_HALT_RX_MSB                                  5
#define MAC_PCU_DIAG_SW_HALT_RX_LSB                                  5
#define MAC_PCU_DIAG_SW_HALT_RX_MASK                                 0x00000020
#define MAC_PCU_DIAG_SW_HALT_RX_GET(x)                               (((x) & MAC_PCU_DIAG_SW_HALT_RX_MASK) >> MAC_PCU_DIAG_SW_HALT_RX_LSB)
#define MAC_PCU_DIAG_SW_HALT_RX_SET(x)                               (((0x0 | (x)) << MAC_PCU_DIAG_SW_HALT_RX_LSB) & MAC_PCU_DIAG_SW_HALT_RX_MASK)
#define MAC_PCU_DIAG_SW_HALT_RX_RESET                                0
#define MAC_PCU_DIAG_SW_NO_DECRYPT_MSB                               4
#define MAC_PCU_DIAG_SW_NO_DECRYPT_LSB                               4
#define MAC_PCU_DIAG_SW_NO_DECRYPT_MASK                              0x00000010
#define MAC_PCU_DIAG_SW_NO_DECRYPT_GET(x)                            (((x) & MAC_PCU_DIAG_SW_NO_DECRYPT_MASK) >> MAC_PCU_DIAG_SW_NO_DECRYPT_LSB)
#define MAC_PCU_DIAG_SW_NO_DECRYPT_SET(x)                            (((0x0 | (x)) << MAC_PCU_DIAG_SW_NO_DECRYPT_LSB) & MAC_PCU_DIAG_SW_NO_DECRYPT_MASK)
#define MAC_PCU_DIAG_SW_NO_DECRYPT_RESET                             0
#define MAC_PCU_DIAG_SW_NO_ENCRYPT_MSB                               3
#define MAC_PCU_DIAG_SW_NO_ENCRYPT_LSB                               3
#define MAC_PCU_DIAG_SW_NO_ENCRYPT_MASK                              0x00000008
#define MAC_PCU_DIAG_SW_NO_ENCRYPT_GET(x)                            (((x) & MAC_PCU_DIAG_SW_NO_ENCRYPT_MASK) >> MAC_PCU_DIAG_SW_NO_ENCRYPT_LSB)
#define MAC_PCU_DIAG_SW_NO_ENCRYPT_SET(x)                            (((0x0 | (x)) << MAC_PCU_DIAG_SW_NO_ENCRYPT_LSB) & MAC_PCU_DIAG_SW_NO_ENCRYPT_MASK)
#define MAC_PCU_DIAG_SW_NO_ENCRYPT_RESET                             0
#define MAC_PCU_DIAG_SW_NO_CTS_MSB                                   2
#define MAC_PCU_DIAG_SW_NO_CTS_LSB                                   2
#define MAC_PCU_DIAG_SW_NO_CTS_MASK                                  0x00000004
#define MAC_PCU_DIAG_SW_NO_CTS_GET(x)                                (((x) & MAC_PCU_DIAG_SW_NO_CTS_MASK) >> MAC_PCU_DIAG_SW_NO_CTS_LSB)
#define MAC_PCU_DIAG_SW_NO_CTS_SET(x)                                (((0x0 | (x)) << MAC_PCU_DIAG_SW_NO_CTS_LSB) & MAC_PCU_DIAG_SW_NO_CTS_MASK)
#define MAC_PCU_DIAG_SW_NO_CTS_RESET                                 0
#define MAC_PCU_DIAG_SW_NO_ACK_MSB                                   1
#define MAC_PCU_DIAG_SW_NO_ACK_LSB                                   1
#define MAC_PCU_DIAG_SW_NO_ACK_MASK                                  0x00000002
#define MAC_PCU_DIAG_SW_NO_ACK_GET(x)                                (((x) & MAC_PCU_DIAG_SW_NO_ACK_MASK) >> MAC_PCU_DIAG_SW_NO_ACK_LSB)
#define MAC_PCU_DIAG_SW_NO_ACK_SET(x)                                (((0x0 | (x)) << MAC_PCU_DIAG_SW_NO_ACK_LSB) & MAC_PCU_DIAG_SW_NO_ACK_MASK)
#define MAC_PCU_DIAG_SW_NO_ACK_RESET                                 0
#define MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_MSB                       0
#define MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_LSB                       0
#define MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_MASK                      0x00000001
#define MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_GET(x)                    (((x) & MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_MASK) >> MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_LSB)
#define MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_SET(x)                    (((0x0 | (x)) << MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_LSB) & MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_MASK)
#define MAC_PCU_DIAG_SW_INVALID_KEY_NO_ACK_RESET                     0
#define MAC_PCU_DIAG_SW_ADDRESS                                      0x0001b4
#define MAC_PCU_DIAG_SW_HW_MASK                                      0xfffe01ff
#define MAC_PCU_DIAG_SW_SW_MASK                                      0xfffe01ff
#define MAC_PCU_DIAG_SW_RSTMASK                                      0x00000000
#define MAC_PCU_DIAG_SW_RESET                                        0x00000000

// 0x01b8 (MAC_PCU_AZIMUTH_MODE)
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_MSB               11
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_LSB               11
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_MASK              0x00000800
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_GET(x)            (((x) & MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_MASK) >> MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_LSB)
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_SET(x)            (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_LSB) & MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_MASK)
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX2_ENABLE_RESET             1
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_MSB               10
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_LSB               10
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_MASK              0x00000400
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_GET(x)            (((x) & MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_MASK) >> MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_LSB)
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_SET(x)            (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_LSB) & MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_MASK)
#define MAC_PCU_AZIMUTH_MODE_PROXY_STA_FIX1_ENABLE_RESET             1
#define MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_MSB                    9
#define MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_LSB                    9
#define MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_MASK                   0x00000200
#define MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_GET(x)                 (((x) & MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_MASK) >> MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_LSB)
#define MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_SET(x)                 (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_LSB) & MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_MASK)
#define MAC_PCU_AZIMUTH_MODE_FILTER_PASS_HOLD_RESET                  1
#define MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_MSB                        8
#define MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_LSB                        8
#define MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_MASK                       0x00000100
#define MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_GET(x)                     (((x) & MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_MASK) >> MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_LSB)
#define MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_SET(x)                     (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_LSB) & MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_MASK)
#define MAC_PCU_AZIMUTH_MODE_WMAC_CLK_SEL_RESET                      1
#define MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_MSB                         7
#define MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_LSB                         7
#define MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_MASK                        0x00000080
#define MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_GET(x)                      (((x) & MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_MASK) >> MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_LSB)
#define MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_SET(x)                      (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_LSB) & MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_MASK)
#define MAC_PCU_AZIMUTH_MODE_BA_USES_AD1_RESET                       0
#define MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_MSB                6
#define MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_LSB                6
#define MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_MASK               0x00000040
#define MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_GET(x)             (((x) & MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_MASK) >> MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_LSB)
#define MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_SET(x)             (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_LSB) & MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_MASK)
#define MAC_PCU_AZIMUTH_MODE_ACK_CTS_MATCH_TX_AD2_RESET              1
#define MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_MSB                          5
#define MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_LSB                          5
#define MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_MASK                         0x00000020
#define MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_GET(x)                       (((x) & MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_MASK) >> MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_LSB)
#define MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_SET(x)                       (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_LSB) & MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_MASK)
#define MAC_PCU_AZIMUTH_MODE_TX_DESC_EN_RESET                        0
#define MAC_PCU_AZIMUTH_MODE_CLK_EN_MSB                              4
#define MAC_PCU_AZIMUTH_MODE_CLK_EN_LSB                              4
#define MAC_PCU_AZIMUTH_MODE_CLK_EN_MASK                             0x00000010
#define MAC_PCU_AZIMUTH_MODE_CLK_EN_GET(x)                           (((x) & MAC_PCU_AZIMUTH_MODE_CLK_EN_MASK) >> MAC_PCU_AZIMUTH_MODE_CLK_EN_LSB)
#define MAC_PCU_AZIMUTH_MODE_CLK_EN_SET(x)                           (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_CLK_EN_LSB) & MAC_PCU_AZIMUTH_MODE_CLK_EN_MASK)
#define MAC_PCU_AZIMUTH_MODE_CLK_EN_RESET                            0
#define MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_MSB                   3
#define MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_LSB                   3
#define MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_MASK                  0x00000008
#define MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_GET(x)                (((x) & MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_MASK) >> MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_LSB)
#define MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_SET(x)                (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_LSB) & MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_MASK)
#define MAC_PCU_AZIMUTH_MODE_RX_TSF_STATUS_SEL_RESET                 0
#define MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_MSB                   2
#define MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_LSB                   2
#define MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_MASK                  0x00000004
#define MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_GET(x)                (((x) & MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_MASK) >> MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_LSB)
#define MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_SET(x)                (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_LSB) & MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_MASK)
#define MAC_PCU_AZIMUTH_MODE_TX_TSF_STATUS_SEL_RESET                 0
#define MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_MSB                      1
#define MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_LSB                      1
#define MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_MASK                     0x00000002
#define MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_GET(x)                   (((x) & MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_MASK) >> MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_LSB)
#define MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_SET(x)                   (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_LSB) & MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_MASK)
#define MAC_PCU_AZIMUTH_MODE_KEY_SEARCH_AD1_RESET                    0
#define MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_MSB                  0
#define MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_LSB                  0
#define MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_MASK                 0x00000001
#define MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_GET(x)               (((x) & MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_MASK) >> MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_LSB)
#define MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_SET(x)               (((0x0 | (x)) << MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_LSB) & MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_MASK)
#define MAC_PCU_AZIMUTH_MODE_DISABLE_TSF_UPDATE_RESET                0
#define MAC_PCU_AZIMUTH_MODE_ADDRESS                                 0x0001b8
#define MAC_PCU_AZIMUTH_MODE_HW_MASK                                 0x00000fff
#define MAC_PCU_AZIMUTH_MODE_SW_MASK                                 0x00000fff
#define MAC_PCU_AZIMUTH_MODE_RSTMASK                                 0x00000f40
#define MAC_PCU_AZIMUTH_MODE_RESET                                   0x00000f40

// 0x01bc (MAC_PCU_AZIMUTH_TIME_STAMP)
#define MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_MSB                         31
#define MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_LSB                         0
#define MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_MASK                        0xffffffff
#define MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_GET(x)                      (((x) & MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_MASK) >> MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_LSB)
#define MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_SET(x)                      (((0x0 | (x)) << MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_LSB) & MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_MASK)
#define MAC_PCU_AZIMUTH_TIME_STAMP_VALUE_RESET                       0
#define MAC_PCU_AZIMUTH_TIME_STAMP_ADDRESS                           0x0001bc
#define MAC_PCU_AZIMUTH_TIME_STAMP_HW_MASK                           0xffffffff
#define MAC_PCU_AZIMUTH_TIME_STAMP_SW_MASK                           0xffffffff
#define MAC_PCU_AZIMUTH_TIME_STAMP_RSTMASK                           0x00000000
#define MAC_PCU_AZIMUTH_TIME_STAMP_RESET                             0x00000000

// 0x01c0 (MAC_PCU_RX_CLEAR_DIFF_CNT)
#define MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_MSB                          31
#define MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_LSB                          0
#define MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_MASK                         0xffffffff
#define MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_GET(x)                       (((x) & MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_MASK) >> MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_LSB)
#define MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_SET(x)                       (((0x0 | (x)) << MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_LSB) & MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_MASK)
#define MAC_PCU_RX_CLEAR_DIFF_CNT_VALUE_RESET                        0
#define MAC_PCU_RX_CLEAR_DIFF_CNT_ADDRESS                            0x0001c0
#define MAC_PCU_RX_CLEAR_DIFF_CNT_HW_MASK                            0xffffffff
#define MAC_PCU_RX_CLEAR_DIFF_CNT_SW_MASK                            0xffffffff
#define MAC_PCU_RX_CLEAR_DIFF_CNT_RSTMASK                            0x00000000
#define MAC_PCU_RX_CLEAR_DIFF_CNT_RESET                              0x00000000

// 0x01c4 (MAC_PCU_WARM_TX_CONTROL)
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_MSB                  31
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_LSB                  31
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_MASK                 0x80000000
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_GET(x)               (((x) & MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_MASK) >> MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_LSB)
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_SET(x)               (((0x0 | (x)) << MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_LSB) & MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_MASK)
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_SELF_GEN_RESET                0
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_MSB                      30
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_LSB                      30
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_MASK                     0x40000000
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_GET(x)                   (((x) & MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_MASK) >> MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_LSB)
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_SET(x)                   (((0x0 | (x)) << MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_LSB) & MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_MASK)
#define MAC_PCU_WARM_TX_CONTROL_ENABLE_DATA_RESET                    0
#define MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_MSB                         15
#define MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_LSB                         0
#define MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_MASK                        0x0000ffff
#define MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_GET(x)                      (((x) & MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_MASK) >> MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_LSB)
#define MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_SET(x)                      (((0x0 | (x)) << MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_LSB) & MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_MASK)
#define MAC_PCU_WARM_TX_CONTROL_SIFS_DUR_RESET                       640
#define MAC_PCU_WARM_TX_CONTROL_ADDRESS                              0x0001c4
#define MAC_PCU_WARM_TX_CONTROL_HW_MASK                              0xc000ffff
#define MAC_PCU_WARM_TX_CONTROL_SW_MASK                              0xc000ffff
#define MAC_PCU_WARM_TX_CONTROL_RSTMASK                              0x0000ffff
#define MAC_PCU_WARM_TX_CONTROL_RESET                                0x00000280

// 0x01c8 (MCMN_GO_TO_IDLE)
#define MCMN_GO_TO_IDLE_PDG_MSB                                      3
#define MCMN_GO_TO_IDLE_PDG_LSB                                      3
#define MCMN_GO_TO_IDLE_PDG_MASK                                     0x00000008
#define MCMN_GO_TO_IDLE_PDG_GET(x)                                   (((x) & MCMN_GO_TO_IDLE_PDG_MASK) >> MCMN_GO_TO_IDLE_PDG_LSB)
#define MCMN_GO_TO_IDLE_PDG_SET(x)                                   (((0x0 | (x)) << MCMN_GO_TO_IDLE_PDG_LSB) & MCMN_GO_TO_IDLE_PDG_MASK)
#define MCMN_GO_TO_IDLE_PDG_RESET                                    0
#define MCMN_GO_TO_IDLE_RXPCU_MSB                                    2
#define MCMN_GO_TO_IDLE_RXPCU_LSB                                    2
#define MCMN_GO_TO_IDLE_RXPCU_MASK                                   0x00000004
#define MCMN_GO_TO_IDLE_RXPCU_GET(x)                                 (((x) & MCMN_GO_TO_IDLE_RXPCU_MASK) >> MCMN_GO_TO_IDLE_RXPCU_LSB)
#define MCMN_GO_TO_IDLE_RXPCU_SET(x)                                 (((0x0 | (x)) << MCMN_GO_TO_IDLE_RXPCU_LSB) & MCMN_GO_TO_IDLE_RXPCU_MASK)
#define MCMN_GO_TO_IDLE_RXPCU_RESET                                  0
#define MCMN_GO_TO_IDLE_TXDMA_MSB                                    1
#define MCMN_GO_TO_IDLE_TXDMA_LSB                                    1
#define MCMN_GO_TO_IDLE_TXDMA_MASK                                   0x00000002
#define MCMN_GO_TO_IDLE_TXDMA_GET(x)                                 (((x) & MCMN_GO_TO_IDLE_TXDMA_MASK) >> MCMN_GO_TO_IDLE_TXDMA_LSB)
#define MCMN_GO_TO_IDLE_TXDMA_SET(x)                                 (((0x0 | (x)) << MCMN_GO_TO_IDLE_TXDMA_LSB) & MCMN_GO_TO_IDLE_TXDMA_MASK)
#define MCMN_GO_TO_IDLE_TXDMA_RESET                                  0
#define MCMN_GO_TO_IDLE_OLE_MSB                                      0
#define MCMN_GO_TO_IDLE_OLE_LSB                                      0
#define MCMN_GO_TO_IDLE_OLE_MASK                                     0x00000001
#define MCMN_GO_TO_IDLE_OLE_GET(x)                                   (((x) & MCMN_GO_TO_IDLE_OLE_MASK) >> MCMN_GO_TO_IDLE_OLE_LSB)
#define MCMN_GO_TO_IDLE_OLE_SET(x)                                   (((0x0 | (x)) << MCMN_GO_TO_IDLE_OLE_LSB) & MCMN_GO_TO_IDLE_OLE_MASK)
#define MCMN_GO_TO_IDLE_OLE_RESET                                    0
#define MCMN_GO_TO_IDLE_ADDRESS                                      0x0001c8
#define MCMN_GO_TO_IDLE_HW_MASK                                      0x0000000f
#define MCMN_GO_TO_IDLE_SW_MASK                                      0x0000000f
#define MCMN_GO_TO_IDLE_RSTMASK                                      0x00000000
#define MCMN_GO_TO_IDLE_RESET                                        0x00000000

// 0x01d0 (ASE_AST_BASE_ADDR)
#define ASE_AST_BASE_ADDR_VALUE_MSB                                  31
#define ASE_AST_BASE_ADDR_VALUE_LSB                                  0
#define ASE_AST_BASE_ADDR_VALUE_MASK                                 0xffffffff
#define ASE_AST_BASE_ADDR_VALUE_GET(x)                               (((x) & ASE_AST_BASE_ADDR_VALUE_MASK) >> ASE_AST_BASE_ADDR_VALUE_LSB)
#define ASE_AST_BASE_ADDR_VALUE_SET(x)                               (((0x0 | (x)) << ASE_AST_BASE_ADDR_VALUE_LSB) & ASE_AST_BASE_ADDR_VALUE_MASK)
#define ASE_AST_BASE_ADDR_VALUE_RESET                                0
#define ASE_AST_BASE_ADDR_ADDRESS                                    0x0001d0
#define ASE_AST_BASE_ADDR_HW_MASK                                    0xffffffff
#define ASE_AST_BASE_ADDR_SW_MASK                                    0xffffffff
#define ASE_AST_BASE_ADDR_RSTMASK                                    0x00000000
#define ASE_AST_BASE_ADDR_RESET                                      0x00000000

// 0x01d4 (ASE_AST_SIZE)
#define ASE_AST_SIZE_VALUE_MSB                                       12
#define ASE_AST_SIZE_VALUE_LSB                                       0
#define ASE_AST_SIZE_VALUE_MASK                                      0x00001fff
#define ASE_AST_SIZE_VALUE_GET(x)                                    (((x) & ASE_AST_SIZE_VALUE_MASK) >> ASE_AST_SIZE_VALUE_LSB)
#define ASE_AST_SIZE_VALUE_SET(x)                                    (((0x0 | (x)) << ASE_AST_SIZE_VALUE_LSB) & ASE_AST_SIZE_VALUE_MASK)
#define ASE_AST_SIZE_VALUE_RESET                                     0
#define ASE_AST_SIZE_ADDRESS                                         0x0001d4
#define ASE_AST_SIZE_HW_MASK                                         0x00001fff
#define ASE_AST_SIZE_SW_MASK                                         0x00001fff
#define ASE_AST_SIZE_RSTMASK                                         0x00000000
#define ASE_AST_SIZE_RESET                                           0x00000000

// 0x01d8 (ASE_SEARCH_CTRL)
#define ASE_SEARCH_CTRL_TIMEOUT_THRESH_MSB                           31
#define ASE_SEARCH_CTRL_TIMEOUT_THRESH_LSB                           16
#define ASE_SEARCH_CTRL_TIMEOUT_THRESH_MASK                          0xffff0000
#define ASE_SEARCH_CTRL_TIMEOUT_THRESH_GET(x)                        (((x) & ASE_SEARCH_CTRL_TIMEOUT_THRESH_MASK) >> ASE_SEARCH_CTRL_TIMEOUT_THRESH_LSB)
#define ASE_SEARCH_CTRL_TIMEOUT_THRESH_SET(x)                        (((0x0 | (x)) << ASE_SEARCH_CTRL_TIMEOUT_THRESH_LSB) & ASE_SEARCH_CTRL_TIMEOUT_THRESH_MASK)
#define ASE_SEARCH_CTRL_TIMEOUT_THRESH_RESET                         0
#define ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_MSB                         8
#define ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_LSB                         8
#define ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_MASK                        0x00000100
#define ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_GET(x)                      (((x) & ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_MASK) >> ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_LSB)
#define ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_SET(x)                      (((0x0 | (x)) << ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_LSB) & ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_MASK)
#define ASE_SEARCH_CTRL_ADDR_SEARCH_SWAP_RESET                       0
#define ASE_SEARCH_CTRL_MAX_SEARCH_MSB                               7
#define ASE_SEARCH_CTRL_MAX_SEARCH_LSB                               0
#define ASE_SEARCH_CTRL_MAX_SEARCH_MASK                              0x000000ff
#define ASE_SEARCH_CTRL_MAX_SEARCH_GET(x)                            (((x) & ASE_SEARCH_CTRL_MAX_SEARCH_MASK) >> ASE_SEARCH_CTRL_MAX_SEARCH_LSB)
#define ASE_SEARCH_CTRL_MAX_SEARCH_SET(x)                            (((0x0 | (x)) << ASE_SEARCH_CTRL_MAX_SEARCH_LSB) & ASE_SEARCH_CTRL_MAX_SEARCH_MASK)
#define ASE_SEARCH_CTRL_MAX_SEARCH_RESET                             0
#define ASE_SEARCH_CTRL_ADDRESS                                      0x0001d8
#define ASE_SEARCH_CTRL_HW_MASK                                      0xffff01ff
#define ASE_SEARCH_CTRL_SW_MASK                                      0xffff01ff
#define ASE_SEARCH_CTRL_RSTMASK                                      0x00000000
#define ASE_SEARCH_CTRL_RESET                                        0x00000000

// 0x01dc (ASE_HASH_CTRL)
#define ASE_HASH_CTRL_CACHE_INVALIDATE_MSB                           23
#define ASE_HASH_CTRL_CACHE_INVALIDATE_LSB                           23
#define ASE_HASH_CTRL_CACHE_INVALIDATE_MASK                          0x00800000
#define ASE_HASH_CTRL_CACHE_INVALIDATE_GET(x)                        (((x) & ASE_HASH_CTRL_CACHE_INVALIDATE_MASK) >> ASE_HASH_CTRL_CACHE_INVALIDATE_LSB)
#define ASE_HASH_CTRL_CACHE_INVALIDATE_SET(x)                        (((0x0 | (x)) << ASE_HASH_CTRL_CACHE_INVALIDATE_LSB) & ASE_HASH_CTRL_CACHE_INVALIDATE_MASK)
#define ASE_HASH_CTRL_CACHE_INVALIDATE_RESET                         0
#define ASE_HASH_CTRL_CACHE_DISABLE_MSB                              22
#define ASE_HASH_CTRL_CACHE_DISABLE_LSB                              22
#define ASE_HASH_CTRL_CACHE_DISABLE_MASK                             0x00400000
#define ASE_HASH_CTRL_CACHE_DISABLE_GET(x)                           (((x) & ASE_HASH_CTRL_CACHE_DISABLE_MASK) >> ASE_HASH_CTRL_CACHE_DISABLE_LSB)
#define ASE_HASH_CTRL_CACHE_DISABLE_SET(x)                           (((0x0 | (x)) << ASE_HASH_CTRL_CACHE_DISABLE_LSB) & ASE_HASH_CTRL_CACHE_DISABLE_MASK)
#define ASE_HASH_CTRL_CACHE_DISABLE_RESET                            0
#define ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_MSB                       21
#define ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_LSB                       21
#define ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_MASK                      0x00200000
#define ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_GET(x)                    (((x) & ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_MASK) >> ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_LSB)
#define ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_SET(x)                    (((0x0 | (x)) << ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_LSB) & ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_MASK)
#define ASE_HASH_CTRL_HASH_MAC_31_0_IGNORE_RESET                     0
#define ASE_HASH_CTRL_HASH_SEL_MSB                                   20
#define ASE_HASH_CTRL_HASH_SEL_LSB                                   20
#define ASE_HASH_CTRL_HASH_SEL_MASK                                  0x00100000
#define ASE_HASH_CTRL_HASH_SEL_GET(x)                                (((x) & ASE_HASH_CTRL_HASH_SEL_MASK) >> ASE_HASH_CTRL_HASH_SEL_LSB)
#define ASE_HASH_CTRL_HASH_SEL_SET(x)                                (((0x0 | (x)) << ASE_HASH_CTRL_HASH_SEL_LSB) & ASE_HASH_CTRL_HASH_SEL_MASK)
#define ASE_HASH_CTRL_HASH_SEL_RESET                                 1
#define ASE_HASH_CTRL_HASH_MCAST_SHIFT_MSB                           19
#define ASE_HASH_CTRL_HASH_MCAST_SHIFT_LSB                           16
#define ASE_HASH_CTRL_HASH_MCAST_SHIFT_MASK                          0x000f0000
#define ASE_HASH_CTRL_HASH_MCAST_SHIFT_GET(x)                        (((x) & ASE_HASH_CTRL_HASH_MCAST_SHIFT_MASK) >> ASE_HASH_CTRL_HASH_MCAST_SHIFT_LSB)
#define ASE_HASH_CTRL_HASH_MCAST_SHIFT_SET(x)                        (((0x0 | (x)) << ASE_HASH_CTRL_HASH_MCAST_SHIFT_LSB) & ASE_HASH_CTRL_HASH_MCAST_SHIFT_MASK)
#define ASE_HASH_CTRL_HASH_MCAST_SHIFT_RESET                         2
#define ASE_HASH_CTRL_HASH_XOR_MSB                                   15
#define ASE_HASH_CTRL_HASH_XOR_LSB                                   0
#define ASE_HASH_CTRL_HASH_XOR_MASK                                  0x0000ffff
#define ASE_HASH_CTRL_HASH_XOR_GET(x)                                (((x) & ASE_HASH_CTRL_HASH_XOR_MASK) >> ASE_HASH_CTRL_HASH_XOR_LSB)
#define ASE_HASH_CTRL_HASH_XOR_SET(x)                                (((0x0 | (x)) << ASE_HASH_CTRL_HASH_XOR_LSB) & ASE_HASH_CTRL_HASH_XOR_MASK)
#define ASE_HASH_CTRL_HASH_XOR_RESET                                 43981
#define ASE_HASH_CTRL_ADDRESS                                        0x0001dc
#define ASE_HASH_CTRL_HW_MASK                                        0x00ffffff
#define ASE_HASH_CTRL_SW_MASK                                        0x00ffffff
#define ASE_HASH_CTRL_RSTMASK                                        0x001fffff
#define ASE_HASH_CTRL_RESET                                          0x0012abcd

// 0x01e0 (MCMN_TRACE_SELECT)
#define MCMN_TRACE_SELECT_MUX_SEL_MSB                                3
#define MCMN_TRACE_SELECT_MUX_SEL_LSB                                0
#define MCMN_TRACE_SELECT_MUX_SEL_MASK                               0x0000000f
#define MCMN_TRACE_SELECT_MUX_SEL_GET(x)                             (((x) & MCMN_TRACE_SELECT_MUX_SEL_MASK) >> MCMN_TRACE_SELECT_MUX_SEL_LSB)
#define MCMN_TRACE_SELECT_MUX_SEL_SET(x)                             (((0x0 | (x)) << MCMN_TRACE_SELECT_MUX_SEL_LSB) & MCMN_TRACE_SELECT_MUX_SEL_MASK)
#define MCMN_TRACE_SELECT_MUX_SEL_RESET                              0
#define MCMN_TRACE_SELECT_ADDRESS                                    0x0001e0
#define MCMN_TRACE_SELECT_HW_MASK                                    0x0000000f
#define MCMN_TRACE_SELECT_SW_MASK                                    0x0000000f
#define MCMN_TRACE_SELECT_RSTMASK                                    0x00000000
#define MCMN_TRACE_SELECT_RESET                                      0x00000000


#endif /* _MAC_MCMN_REG_H_ */
