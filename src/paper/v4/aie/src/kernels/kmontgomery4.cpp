#include "kernels.hpp"

#define LIMB_SIZE 31
#define N_LIMBS 13
#define TOP_BIT_POINTER_OFFSET 26
#define CARRY_POINTER_OFFSET 27

alignas(16) static const int32 all_ones[8] = {
  0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
};
alignas(16) static const int32 all_zeros[8] = {
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};


// Macro to get the limb from the accumulator after MAC operations
#define GET_LIMB_FROM_ACC(i)                                                    \
  do {                                                                          \
    acc_limb_extraction = ups(u_ptr[i], 0);                                     \
    u_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 31);              \
    acc_limb_extraction = ups(u_ptr[TOP_BIT_POINTER_OFFSET], 31);               \
    u_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 0);               \
    u_ptr[i] = u_ptr[i] + u_ptr[TOP_BIT_POINTER_OFFSET];                        \
  } while (0)

// Macros to extract carries in the case of addition 
// This macro is different as the result of the addition can be stored in just 64 bits 

#define EXTRACT_CARRY_FROM_ACC_ADD()                                            \
  do {                                                                          \
    u_ptr[CARRY_POINTER_OFFSET] = srs(acc, LIMB_SIZE);                          \
    acc = lups(u_ptr[CARRY_POINTER_OFFSET], 0);                                 \
  } while (0)

void kmontgomery4(
    adf::input_buffer<int32>& __restrict t_in,
    adf::input_buffer<int32>& __restrict mxp_in,
    adf::output_buffer<int32>& __restrict u_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankA) *__restrict t_ptr = (v8int32 chess_storage(DM_bankA) *__restrict)t_in.data();
  v8int32 chess_storage(DM_bankB) *__restrict mxp_ptr = (v8int32 chess_storage(DM_bankB) *__restrict)mxp_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict u_ptr = (v8int32 chess_storage(DM_bankC) *__restrict)u_out.data();

  set_rnd(0); // floor rounding
  clr_sat();  // clear saturation

  // Accumulator used for MAC operations
  v8acc80 acc;

  // Accumulator and vecroe for limb extraction 
  v8acc48 acc_limb_extraction;

   // t + m x p

  // (t + m x p) / R
  
  // i = 0
  acc = mul(t_ptr[0], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[0], *(v8int32 *) (all_ones));
  u_ptr[0] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 1
  acc = mac(acc, t_ptr[1], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[1], *(v8int32 *) (all_ones));
  u_ptr[1] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 2
  acc = mac(acc, t_ptr[2], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[2], *(v8int32 *) (all_ones));
  u_ptr[2] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 3
  acc = mac(acc, t_ptr[3], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[3], *(v8int32 *) (all_ones));
  u_ptr[3] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 4
  acc = mac(acc, t_ptr[4], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[4], *(v8int32 *) (all_ones));
  u_ptr[4] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 5
  acc = mac(acc, t_ptr[5], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[5], *(v8int32 *) (all_ones));
  u_ptr[5] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 6
  acc = mac(acc, t_ptr[6], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[6], *(v8int32 *) (all_ones));
  u_ptr[6] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 7
  acc = mac(acc, t_ptr[7], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[7], *(v8int32 *) (all_ones));
  u_ptr[7] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 8
  acc = mac(acc, t_ptr[8], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[8], *(v8int32 *) (all_ones));
  u_ptr[8] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 9
  acc = mac(acc, t_ptr[9], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[9], *(v8int32 *) (all_ones));
  u_ptr[9] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 10
  acc = mac(acc, t_ptr[10], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[10], *(v8int32 *) (all_ones));
  u_ptr[10] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 11
  acc = mac(acc, t_ptr[11], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[11], *(v8int32 *) (all_ones));
  u_ptr[11] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 12
  acc = mac(acc, t_ptr[12], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[12], *(v8int32 *) (all_ones));
  u_ptr[12] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 13
  acc = mac(acc, t_ptr[13], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[13], *(v8int32 *) (all_ones));
  u_ptr[13] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 14
  acc = mac(acc, t_ptr[14], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[14], *(v8int32 *) (all_ones));
  u_ptr[14] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 15
  acc = mac(acc, t_ptr[15], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[15], *(v8int32 *) (all_ones));
  u_ptr[15] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 16
  acc = mac(acc, t_ptr[16], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[16], *(v8int32 *) (all_ones));
  u_ptr[16] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 17
  acc = mac(acc, t_ptr[17], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[17], *(v8int32 *) (all_ones));
  u_ptr[17] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 18
  acc = mac(acc, t_ptr[18], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[18], *(v8int32 *) (all_ones));
  u_ptr[18] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 19
  acc = mac(acc, t_ptr[19], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[19], *(v8int32 *) (all_ones));
  u_ptr[19] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 20
  acc = mac(acc, t_ptr[20], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[20], *(v8int32 *) (all_ones));
  u_ptr[20] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 21
  acc = mac(acc, t_ptr[21], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[21], *(v8int32 *) (all_ones));
  u_ptr[21] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 22
  acc = mac(acc, t_ptr[22], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[22], *(v8int32 *) (all_ones));
  u_ptr[22] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 23
  acc = mac(acc, t_ptr[23], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[23], *(v8int32 *) (all_ones));
  u_ptr[23] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 24
  acc = mac(acc, t_ptr[24], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[24], *(v8int32 *) (all_ones));
  u_ptr[24] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // i = 25
  acc = mac(acc, t_ptr[25], *(v8int32 *) (all_ones));
  acc = mac(acc, mxp_ptr[25], *(v8int32 *) (all_ones));
  u_ptr[25] = srs(acc, 0);
  
  u_ptr[0] = u_ptr[0 + N_LIMBS];
  u_ptr[1] = u_ptr[1 + N_LIMBS];
  u_ptr[2] = u_ptr[2 + N_LIMBS];
  u_ptr[3] = u_ptr[3 + N_LIMBS];
  u_ptr[4] = u_ptr[4 + N_LIMBS];
  u_ptr[5] = u_ptr[5 + N_LIMBS];
  u_ptr[6] = u_ptr[6 + N_LIMBS];
  u_ptr[7] = u_ptr[7 + N_LIMBS];
  u_ptr[8] = u_ptr[8 + N_LIMBS];
  u_ptr[9] = u_ptr[9 + N_LIMBS];
  u_ptr[10] = u_ptr[10 + N_LIMBS];
  u_ptr[11] = u_ptr[11 + N_LIMBS];
  u_ptr[12] = u_ptr[12 + N_LIMBS];


  GET_LIMB_FROM_ACC(0);
  GET_LIMB_FROM_ACC(1);
  GET_LIMB_FROM_ACC(2);
  GET_LIMB_FROM_ACC(3);
  GET_LIMB_FROM_ACC(4);
  GET_LIMB_FROM_ACC(5);
  GET_LIMB_FROM_ACC(6);
  GET_LIMB_FROM_ACC(7);
  GET_LIMB_FROM_ACC(8);
  GET_LIMB_FROM_ACC(9);
  GET_LIMB_FROM_ACC(10);
  GET_LIMB_FROM_ACC(11);
  GET_LIMB_FROM_ACC(12);

  // Clear top N_LIMBS in mxp_ptr
  u_ptr[0 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[1 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[2 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[3 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[4 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[5 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[6 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[7 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[8 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[9 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[10 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[11 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[12 + N_LIMBS] = *(v8int32 *) (all_zeros);
}
