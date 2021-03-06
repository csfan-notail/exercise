// Copyright (c) 2012 Qualcomm Atheros, Inc.  All rights reserved.
// $ATH_LICENSE_HW_HDR_C$
//
// DO NOT EDIT!  This file is automatically generated
//               These definitions are tied to a particular hardware layout


#ifndef _PKT_USER2_H_
#define _PKT_USER2_H_
#if !defined(__ASSEMBLER__)
#endif

// ################ START SUMMARY #################
//
//	Dword	Fields
//	0	data_word_0[31:0]
//	1	data_word_1[31:0]
//	2	data_word_2[31:0]
//	3	data_word_3[31:0]
//
// ################ END SUMMARY #################

#define NUM_OF_DWORDS_PKT_USER2 4

struct pkt_user2 {
    volatile uint32_t data_word_0                     : 32; //[31:0]
    volatile uint32_t data_word_1                     : 32; //[31:0]
    volatile uint32_t data_word_2                     : 32; //[31:0]
    volatile uint32_t data_word_3                     : 32; //[31:0]
};

/*

data_word_0
			
			1st Dword of data block - length field in TLV indicates
			message length in octets <legal all>

data_word_1
			
			2nd Dword of data block (optional)  <legal all>

data_word_2
			
			3rd Dword of data block (optional)  <legal all>

data_word_3
			
			4th (i.e. last) Dword of data block (optional)  <legal
			all>
*/


/* Description		PKT_USER2_0_DATA_WORD_0
			
			1st Dword of data block - length field in TLV indicates
			message length in octets <legal all>
*/
#define PKT_USER2_0_DATA_WORD_0_OFFSET                               0x00000000
#define PKT_USER2_0_DATA_WORD_0_LSB                                  0
#define PKT_USER2_0_DATA_WORD_0_MASK                                 0xffffffff

/* Description		PKT_USER2_1_DATA_WORD_1
			
			2nd Dword of data block (optional)  <legal all>
*/
#define PKT_USER2_1_DATA_WORD_1_OFFSET                               0x00000004
#define PKT_USER2_1_DATA_WORD_1_LSB                                  0
#define PKT_USER2_1_DATA_WORD_1_MASK                                 0xffffffff

/* Description		PKT_USER2_2_DATA_WORD_2
			
			3rd Dword of data block (optional)  <legal all>
*/
#define PKT_USER2_2_DATA_WORD_2_OFFSET                               0x00000008
#define PKT_USER2_2_DATA_WORD_2_LSB                                  0
#define PKT_USER2_2_DATA_WORD_2_MASK                                 0xffffffff

/* Description		PKT_USER2_3_DATA_WORD_3
			
			4th (i.e. last) Dword of data block (optional)  <legal
			all>
*/
#define PKT_USER2_3_DATA_WORD_3_OFFSET                               0x0000000c
#define PKT_USER2_3_DATA_WORD_3_LSB                                  0
#define PKT_USER2_3_DATA_WORD_3_MASK                                 0xffffffff


#endif // _PKT_USER2_H_
