//                                                                             
// File:       ./headers/mac_txdma_reg.h                                       
// Creator:    afang                                                           
// Time:       Friday Oct 5, 2012 [2:07:26 pm]                                 
//                                                                             
// Path:       /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl
// Arguments:  /cad/denali/blueprint/3.7.4//Linux-64bit/blueprint -odir        
//             ./headers -codegen ath_ch.codegen -ath_ch -Wdesc                
//             mac_txdma_reg.rdl                                               
//                                                                             
// Sources:    /trees/afang/afang-dev/work/blueprint/project/beeliner/releases/AutogenVersion/rdl/mac_txdma_reg.rdl
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


#ifndef _MAC_TXDMA_REG_H_
#define _MAC_TXDMA_REG_H_

// 0x0000 (MSDU_INFO_FETCH_CFG)
#define MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_MSB               2
#define MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_LSB               0
#define MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_MASK              0x00000007
#define MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_GET(x)            (((x) & MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_MASK) >> MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_LSB)
#define MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_SET(x)            (((0x0 | (x)) << MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_LSB) & MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_MASK)
#define MSDU_INFO_FETCH_CFG_MAX_FRAG_TABLE_ENTRIES_RESET             4
#define MSDU_INFO_FETCH_CFG_ADDRESS                                  0x000000
#define MSDU_INFO_FETCH_CFG_HW_MASK                                  0x00000007
#define MSDU_INFO_FETCH_CFG_SW_MASK                                  0x00000007
#define MSDU_INFO_FETCH_CFG_RSTMASK                                  0x00000007
#define MSDU_INFO_FETCH_CFG_RESET                                    0x00000004

// 0x0004 (TXDATA_FETCH_CFG)
#define TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_MSB             7
#define TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_LSB             0
#define TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_MASK            0x000000ff
#define TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_GET(x)          (((x) & TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_MASK) >> TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_LSB)
#define TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_SET(x)          (((0x0 | (x)) << TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_LSB) & TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_MASK)
#define TXDATA_FETCH_CFG_MARGIN_OLE_BUF_AVAIL_DWORDS_RESET           20
#define TXDATA_FETCH_CFG_ADDRESS                                     0x000004
#define TXDATA_FETCH_CFG_HW_MASK                                     0x000000ff
#define TXDATA_FETCH_CFG_SW_MASK                                     0x000000ff
#define TXDATA_FETCH_CFG_RSTMASK                                     0x000000ff
#define TXDATA_FETCH_CFG_RESET                                       0x00000014

// 0x0008 (TXDMA_ERROR_CFG)
#define TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_MSB                        7
#define TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_LSB                        0
#define TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_MASK                       0x000000ff
#define TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_GET(x)                     (((x) & TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_MASK) >> TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_LSB)
#define TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_SET(x)                     (((0x0 | (x)) << TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_LSB) & TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_MASK)
#define TXDMA_ERROR_CFG_OLE_TIMEOUT_LIMIT_RESET                      2
#define TXDMA_ERROR_CFG_ADDRESS                                      0x000008
#define TXDMA_ERROR_CFG_HW_MASK                                      0x000000ff
#define TXDMA_ERROR_CFG_SW_MASK                                      0x000000ff
#define TXDMA_ERROR_CFG_RSTMASK                                      0x000000ff
#define TXDMA_ERROR_CFG_RESET                                        0x00000002

// 0x000c (WATCHDOG)
#define WATCHDOG_STATUS_MSB                                          31
#define WATCHDOG_STATUS_LSB                                          16
#define WATCHDOG_STATUS_MASK                                         0xffff0000
#define WATCHDOG_STATUS_GET(x)                                       (((x) & WATCHDOG_STATUS_MASK) >> WATCHDOG_STATUS_LSB)
#define WATCHDOG_STATUS_SET(x)                                       (((0x0 | (x)) << WATCHDOG_STATUS_LSB) & WATCHDOG_STATUS_MASK)
#define WATCHDOG_STATUS_RESET                                        0
#define WATCHDOG_LIMIT_MSB                                           15
#define WATCHDOG_LIMIT_LSB                                           0
#define WATCHDOG_LIMIT_MASK                                          0x0000ffff
#define WATCHDOG_LIMIT_GET(x)                                        (((x) & WATCHDOG_LIMIT_MASK) >> WATCHDOG_LIMIT_LSB)
#define WATCHDOG_LIMIT_SET(x)                                        (((0x0 | (x)) << WATCHDOG_LIMIT_LSB) & WATCHDOG_LIMIT_MASK)
#define WATCHDOG_LIMIT_RESET                                         65535
#define WATCHDOG_ADDRESS                                             0x00000c
#define WATCHDOG_HW_MASK                                             0xffffffff
#define WATCHDOG_SW_MASK                                             0xffffffff
#define WATCHDOG_RSTMASK                                             0x0000ffff
#define WATCHDOG_RESET                                               0x0000ffff

// 0x0010 (TRACEBUS)
#define TRACEBUS_SELECT_MSB                                          3
#define TRACEBUS_SELECT_LSB                                          0
#define TRACEBUS_SELECT_MASK                                         0x0000000f
#define TRACEBUS_SELECT_GET(x)                                       (((x) & TRACEBUS_SELECT_MASK) >> TRACEBUS_SELECT_LSB)
#define TRACEBUS_SELECT_SET(x)                                       (((0x0 | (x)) << TRACEBUS_SELECT_LSB) & TRACEBUS_SELECT_MASK)
#define TRACEBUS_SELECT_RESET                                        0
#define TRACEBUS_ADDRESS                                             0x000010
#define TRACEBUS_HW_MASK                                             0x0000000f
#define TRACEBUS_SW_MASK                                             0x0000000f
#define TRACEBUS_RSTMASK                                             0x00000000
#define TRACEBUS_RESET                                               0x00000000


#endif /* _MAC_TXDMA_REG_H_ */
