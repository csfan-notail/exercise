// ------------------------------------------------------------------
// Copyright (c) 2004-2011 Qualcomm Atheros.  All rights reserved.
// $ATH_LICENSE_HOSTSDK0_C$
// ------------------------------------------------------------------
//===================================================================
// Author(s): ="Qualcomm Atheros"
//===================================================================



#ifndef _SM1_TABLE_MAP_H_
#define _SM1_TABLE_MAP_H_


/* macros for BB_sm1_tables_dummy1 */
#define BB_SM1_TABLES_DUMMY1_ADDRESS                                                      0x00000000
#define BB_SM1_TABLES_DUMMY1_OFFSET                                                       0x00000000
#define BB_SM1_TABLES_DUMMY1_DUMMY1_MSB                                                           31
#define BB_SM1_TABLES_DUMMY1_DUMMY1_LSB                                                            0
#define BB_SM1_TABLES_DUMMY1_DUMMY1_MASK                                                  0xffffffff
#define BB_SM1_TABLES_DUMMY1_DUMMY1_GET(x)                                 (((x) & 0xffffffff) >> 0)
#define BB_SM1_TABLES_DUMMY1_DUMMY1_SET(x)                                 (((x) << 0) & 0xffffffff)

/* macros for BB_dc_dac_mem_b1 */
#define BB_DC_DAC_MEM_B1_ADDRESS                                                          0x000003c0
#define BB_DC_DAC_MEM_B1_OFFSET                                                           0x000003c0
#define BB_DC_DAC_MEM_B1_DC_DAC_SETTING_MSB                                                       19
#define BB_DC_DAC_MEM_B1_DC_DAC_SETTING_LSB                                                        0
#define BB_DC_DAC_MEM_B1_DC_DAC_SETTING_MASK                                              0x000fffff
#define BB_DC_DAC_MEM_B1_DC_DAC_SETTING_GET(x)                             (((x) & 0x000fffff) >> 0)
#define BB_DC_DAC_MEM_B1_DC_DAC_SETTING_SET(x)                             (((x) << 0) & 0x000fffff)

/* macros for BB_sm1_tables_dummy2 */
#define BB_SM1_TABLES_DUMMY2_ADDRESS                                                      0x000005fc
#define BB_SM1_TABLES_DUMMY2_OFFSET                                                       0x000005fc
#define BB_SM1_TABLES_DUMMY2_DUMMY2_MSB                                                           31
#define BB_SM1_TABLES_DUMMY2_DUMMY2_LSB                                                            0
#define BB_SM1_TABLES_DUMMY2_DUMMY2_MASK                                                  0xffffffff
#define BB_SM1_TABLES_DUMMY2_DUMMY2_GET(x)                                 (((x) & 0xffffffff) >> 0)
#define BB_SM1_TABLES_DUMMY2_DUMMY2_SET(x)                                 (((x) << 0) & 0xffffffff)


#ifndef __ASSEMBLER__

typedef struct sm1_table_map_reg_s {
  volatile unsigned int BB_sm1_tables_dummy1;                          /*        0x0 - 0x4        */
  volatile char pad__0[0x3bc];                                         /*        0x4 - 0x3c0      */
  volatile unsigned int BB_dc_dac_mem_b1[4];                           /*      0x3c0 - 0x3d0      */
  volatile char pad__1[0x22c];                                         /*      0x3d0 - 0x5fc      */
  volatile unsigned int BB_sm1_tables_dummy2;                          /*      0x5fc - 0x600      */
} sm1_table_map_reg_t;

#endif /* __ASSEMBLER__ */

#endif /* _SM1_TABLE_MAP_H_ */
