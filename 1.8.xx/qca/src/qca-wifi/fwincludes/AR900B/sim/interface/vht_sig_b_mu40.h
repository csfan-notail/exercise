// Copyright (c) 2012 Qualcomm Atheros, Inc.  All rights reserved.
// $ATH_LICENSE_HW_HDR_C$
//
// DO NOT EDIT!  This file is automatically generated
//               These definitions are tied to a particular hardware layout


#ifndef _VHT_SIG_B_MU40_H_
#define _VHT_SIG_B_MU40_H_
#if !defined(__ASSEMBLER__)
#endif

// ################ START SUMMARY #################
//
//	Dword	Fields
//	0	length[16:0], mcs[20:17], tail[26:21], reserved_0[28:27], mu_user_number[31:29]
//	1	length_copy[16:0], mcs_copy[20:17], tail_copy[26:21], reserved_1[28:27], user_number[31:29]
//
// ################ END SUMMARY #################

#define NUM_OF_DWORDS_VHT_SIG_B_MU40 2

struct vht_sig_b_mu40 {
    volatile uint32_t length                          : 17, //[16:0]
                      mcs                             :  4, //[20:17]
                      tail                            :  6, //[26:21]
                      reserved_0                      :  2, //[28:27]
                      mu_user_number                  :  3; //[31:29]
    volatile uint32_t length_copy                     : 17, //[16:0]
                      mcs_copy                        :  4, //[20:17]
                      tail_copy                       :  6, //[26:21]
                      reserved_1                      :  2, //[28:27]
                      user_number                     :  3; //[31:29]
};

/*

length
			
			VHT-SIG-B Length (in units of 4 octets) = ceiling
			(LENGTH/4) <legal all>

mcs
			
			Modulation as described in vht_sig_a mcs field <legal
			0-9>

tail
			
			Used to terminate the trellis of the convolutional
			decoder.
			
			Set to 0.  <legal 0>

reserved_0
			
			Not part of VHT-SIG-B.
			
			Reserved: Set to 0 and ignored on receive  <legal 0>

mu_user_number
			
			Not part of VHT-SIG-B.
			
			Mapping from user number to mu_user_number  <legal 0-3>

length_copy
			
			Same as length. This field is not valid for RX packets.
			<legal all>

mcs_copy
			
			Same as mcs. This field is not valid for RX packets.
			<legal 0-9>

tail_copy
			
			Same as tail. This field is not valid for RX packets. 
			<legal 0>

reserved_1
			
			Not part of VHT-SIG-B.
			
			Reserved: Set to 0 and ignored on receive  <legal 0>

user_number
			
			Not part of VHT-SIG-B.
			
			Defines which user this SIG-B word corresponds to during
			MU transmission. For MU packet reception, this field shall
			indicate the user number decoded by the PHY (for diagnostic
			purposes). <legal 0-2>
*/


/* Description		VHT_SIG_B_MU40_0_LENGTH
			
			VHT-SIG-B Length (in units of 4 octets) = ceiling
			(LENGTH/4) <legal all>
*/
#define VHT_SIG_B_MU40_0_LENGTH_OFFSET                               0x00000000
#define VHT_SIG_B_MU40_0_LENGTH_LSB                                  0
#define VHT_SIG_B_MU40_0_LENGTH_MASK                                 0x0001ffff

/* Description		VHT_SIG_B_MU40_0_MCS
			
			Modulation as described in vht_sig_a mcs field <legal
			0-9>
*/
#define VHT_SIG_B_MU40_0_MCS_OFFSET                                  0x00000000
#define VHT_SIG_B_MU40_0_MCS_LSB                                     17
#define VHT_SIG_B_MU40_0_MCS_MASK                                    0x001e0000

/* Description		VHT_SIG_B_MU40_0_TAIL
			
			Used to terminate the trellis of the convolutional
			decoder.
			
			Set to 0.  <legal 0>
*/
#define VHT_SIG_B_MU40_0_TAIL_OFFSET                                 0x00000000
#define VHT_SIG_B_MU40_0_TAIL_LSB                                    21
#define VHT_SIG_B_MU40_0_TAIL_MASK                                   0x07e00000

/* Description		VHT_SIG_B_MU40_0_RESERVED_0
			
			Not part of VHT-SIG-B.
			
			Reserved: Set to 0 and ignored on receive  <legal 0>
*/
#define VHT_SIG_B_MU40_0_RESERVED_0_OFFSET                           0x00000000
#define VHT_SIG_B_MU40_0_RESERVED_0_LSB                              27
#define VHT_SIG_B_MU40_0_RESERVED_0_MASK                             0x18000000

/* Description		VHT_SIG_B_MU40_0_MU_USER_NUMBER
			
			Not part of VHT-SIG-B.
			
			Mapping from user number to mu_user_number  <legal 0-3>
*/
#define VHT_SIG_B_MU40_0_MU_USER_NUMBER_OFFSET                       0x00000000
#define VHT_SIG_B_MU40_0_MU_USER_NUMBER_LSB                          29
#define VHT_SIG_B_MU40_0_MU_USER_NUMBER_MASK                         0xe0000000

/* Description		VHT_SIG_B_MU40_1_LENGTH_COPY
			
			Same as length. This field is not valid for RX packets.
			<legal all>
*/
#define VHT_SIG_B_MU40_1_LENGTH_COPY_OFFSET                          0x00000004
#define VHT_SIG_B_MU40_1_LENGTH_COPY_LSB                             0
#define VHT_SIG_B_MU40_1_LENGTH_COPY_MASK                            0x0001ffff

/* Description		VHT_SIG_B_MU40_1_MCS_COPY
			
			Same as mcs. This field is not valid for RX packets.
			<legal 0-9>
*/
#define VHT_SIG_B_MU40_1_MCS_COPY_OFFSET                             0x00000004
#define VHT_SIG_B_MU40_1_MCS_COPY_LSB                                17
#define VHT_SIG_B_MU40_1_MCS_COPY_MASK                               0x001e0000

/* Description		VHT_SIG_B_MU40_1_TAIL_COPY
			
			Same as tail. This field is not valid for RX packets. 
			<legal 0>
*/
#define VHT_SIG_B_MU40_1_TAIL_COPY_OFFSET                            0x00000004
#define VHT_SIG_B_MU40_1_TAIL_COPY_LSB                               21
#define VHT_SIG_B_MU40_1_TAIL_COPY_MASK                              0x07e00000

/* Description		VHT_SIG_B_MU40_1_RESERVED_1
			
			Not part of VHT-SIG-B.
			
			Reserved: Set to 0 and ignored on receive  <legal 0>
*/
#define VHT_SIG_B_MU40_1_RESERVED_1_OFFSET                           0x00000004
#define VHT_SIG_B_MU40_1_RESERVED_1_LSB                              27
#define VHT_SIG_B_MU40_1_RESERVED_1_MASK                             0x18000000

/* Description		VHT_SIG_B_MU40_1_USER_NUMBER
			
			Not part of VHT-SIG-B.
			
			Defines which user this SIG-B word corresponds to during
			MU transmission. For MU packet reception, this field shall
			indicate the user number decoded by the PHY (for diagnostic
			purposes). <legal 0-2>
*/
#define VHT_SIG_B_MU40_1_USER_NUMBER_OFFSET                          0x00000004
#define VHT_SIG_B_MU40_1_USER_NUMBER_LSB                             29
#define VHT_SIG_B_MU40_1_USER_NUMBER_MASK                            0xe0000000


#endif // _VHT_SIG_B_MU40_H_
