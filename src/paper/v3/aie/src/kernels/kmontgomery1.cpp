#include "kernels.hpp"

#define LIMB_SIZE 31
#define N_LIMBS 13
#define TOP_BIT_POINTER_OFFSET 26

// Macro to extract carry from the accumulator after MAC operations by extracting
// the lower and upper parts of the accumulator, shifting them, and updating
// the accumulator with the shifted values.
#define EXTRACT_CARRY_FROM_ACC()                                                \
  do {                                                                          \
    acc_carry = ext_lo(acc);                                                    \
    carry = lsrs(acc_carry, LIMB_SIZE);                                         \
    acc_carry = lups(carry, 0);                                                 \
    acc = upd_lo(acc, acc_carry);                                               \
    acc_carry = ext_hi(acc);                                                    \
    carry = lsrs(acc_carry, LIMB_SIZE);                                         \
    acc_carry = lups(carry, 0);                                                 \
    acc = upd_hi(acc, acc_carry);                                               \
  } while (0)

// Macro to get the limb from the accumulator after MAC operations by extracting
// the lower part of the accumulator, shifting it, and storing it in the output
// buffer.
#define GET_LIMB_FROM_ACC(i)                                                    \
  do {                                                                          \
    acc_limb_extraction = ups(t_ptr[i], 0);                                     \
    t_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 31);              \
    acc_limb_extraction = ups(t_ptr[TOP_BIT_POINTER_OFFSET], 31);               \
    t_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 0);               \
    t_ptr[i] = t_ptr[i] + t_ptr[TOP_BIT_POINTER_OFFSET];                        \
  } while (0)

void kmontgomery1(
    adf::input_buffer<int32>& __restrict a_in,
    adf::input_buffer<int32>& __restrict b_in,
    adf::output_buffer<int32>& __restrict t_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankA) *__restrict a_ptr = (v8int32 chess_storage(DM_bankA) *__restrict)a_in.data();
  v8int32 chess_storage(DM_bankB) *__restrict b_ptr = (v8int32 chess_storage(DM_bankB) *__restrict)b_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict t_ptr = (v8int32 chess_storage(DM_bankC) *__restrict)t_out.data();

  set_rnd(0); // floor rounding
  clr_sat();  // clear saturation

  // Accumulator used for MAC operations
  v8acc80 acc;

  // Accumulator and vector for carry extraction  
  v4acc80 acc_carry;
  v4int64 carry;

  // Accumulator for limb extraction 
  v8acc48 acc_limb_extraction;

  // k = 0 : a[0]b[0] is the first product
  acc = mul(a_ptr[0], b_ptr[0]);
  t_ptr[0] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 1 .. (N_LIMBS - 1): accumulate a[i]*b[k-i] for i=0..k
  // k = 1
  acc = mac(acc, a_ptr[0], b_ptr[1]);
  acc = mac(acc, a_ptr[1], b_ptr[0]);
  t_ptr[1] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 2
  acc = mac(acc, a_ptr[0], b_ptr[2]);
  acc = mac(acc, a_ptr[1], b_ptr[1]);
  acc = mac(acc, a_ptr[2], b_ptr[0]);
  t_ptr[2] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 3
  acc = mac(acc, a_ptr[0], b_ptr[3]);
  acc = mac(acc, a_ptr[1], b_ptr[2]);
  acc = mac(acc, a_ptr[2], b_ptr[1]);
  acc = mac(acc, a_ptr[3], b_ptr[0]);
  t_ptr[3] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 4
  acc = mac(acc, a_ptr[0], b_ptr[4]);
  acc = mac(acc, a_ptr[1], b_ptr[3]);
  acc = mac(acc, a_ptr[2], b_ptr[2]);
  acc = mac(acc, a_ptr[3], b_ptr[1]);
  acc = mac(acc, a_ptr[4], b_ptr[0]);
  t_ptr[4] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 5
  acc = mac(acc, a_ptr[0], b_ptr[5]);
  acc = mac(acc, a_ptr[1], b_ptr[4]);
  acc = mac(acc, a_ptr[2], b_ptr[3]);
  acc = mac(acc, a_ptr[3], b_ptr[2]);
  acc = mac(acc, a_ptr[4], b_ptr[1]);
  acc = mac(acc, a_ptr[5], b_ptr[0]);
  t_ptr[5] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 6
  acc = mac(acc, a_ptr[0], b_ptr[6]);
  acc = mac(acc, a_ptr[1], b_ptr[5]);
  acc = mac(acc, a_ptr[2], b_ptr[4]);
  acc = mac(acc, a_ptr[3], b_ptr[3]);
  acc = mac(acc, a_ptr[4], b_ptr[2]);
  acc = mac(acc, a_ptr[5], b_ptr[1]);
  acc = mac(acc, a_ptr[6], b_ptr[0]);
  t_ptr[6] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 7
  acc = mac(acc, a_ptr[0], b_ptr[7]);
  acc = mac(acc, a_ptr[1], b_ptr[6]);
  acc = mac(acc, a_ptr[2], b_ptr[5]);
  acc = mac(acc, a_ptr[3], b_ptr[4]);
  acc = mac(acc, a_ptr[4], b_ptr[3]);
  acc = mac(acc, a_ptr[5], b_ptr[2]);
  acc = mac(acc, a_ptr[6], b_ptr[1]);
  acc = mac(acc, a_ptr[7], b_ptr[0]);
  t_ptr[7] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 8
  acc = mac(acc, a_ptr[0], b_ptr[8]);
  acc = mac(acc, a_ptr[1], b_ptr[7]);
  acc = mac(acc, a_ptr[2], b_ptr[6]);
  acc = mac(acc, a_ptr[3], b_ptr[5]);
  acc = mac(acc, a_ptr[4], b_ptr[4]);
  acc = mac(acc, a_ptr[5], b_ptr[3]);
  acc = mac(acc, a_ptr[6], b_ptr[2]);
  acc = mac(acc, a_ptr[7], b_ptr[1]);
  acc = mac(acc, a_ptr[8], b_ptr[0]);
  t_ptr[8] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 9
  acc = mac(acc, a_ptr[0], b_ptr[9]);
  acc = mac(acc, a_ptr[1], b_ptr[8]);
  acc = mac(acc, a_ptr[2], b_ptr[7]);
  acc = mac(acc, a_ptr[3], b_ptr[6]);
  acc = mac(acc, a_ptr[4], b_ptr[5]);
  acc = mac(acc, a_ptr[5], b_ptr[4]);
  acc = mac(acc, a_ptr[6], b_ptr[3]);
  acc = mac(acc, a_ptr[7], b_ptr[2]);
  acc = mac(acc, a_ptr[8], b_ptr[1]);
  acc = mac(acc, a_ptr[9], b_ptr[0]);
  t_ptr[9] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 10
  acc = mac(acc, a_ptr[0], b_ptr[10]);
  acc = mac(acc, a_ptr[1], b_ptr[9]);
  acc = mac(acc, a_ptr[2], b_ptr[8]);
  acc = mac(acc, a_ptr[3], b_ptr[7]);
  acc = mac(acc, a_ptr[4], b_ptr[6]);
  acc = mac(acc, a_ptr[5], b_ptr[5]);
  acc = mac(acc, a_ptr[6], b_ptr[4]);
  acc = mac(acc, a_ptr[7], b_ptr[3]);
  acc = mac(acc, a_ptr[8], b_ptr[2]);
  acc = mac(acc, a_ptr[9], b_ptr[1]);
  acc = mac(acc, a_ptr[10], b_ptr[0]);
  t_ptr[10] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 11
  acc = mac(acc, a_ptr[0], b_ptr[11]);
  acc = mac(acc, a_ptr[1], b_ptr[10]);
  acc = mac(acc, a_ptr[2], b_ptr[9]);
  acc = mac(acc, a_ptr[3], b_ptr[8]);
  acc = mac(acc, a_ptr[4], b_ptr[7]);
  acc = mac(acc, a_ptr[5], b_ptr[6]);
  acc = mac(acc, a_ptr[6], b_ptr[5]);
  acc = mac(acc, a_ptr[7], b_ptr[4]);
  acc = mac(acc, a_ptr[8], b_ptr[3]);
  acc = mac(acc, a_ptr[9], b_ptr[2]);
  acc = mac(acc, a_ptr[10], b_ptr[1]);
  acc = mac(acc, a_ptr[11], b_ptr[0]);
  t_ptr[11] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 12
  acc = mac(acc, a_ptr[0], b_ptr[12]);
  acc = mac(acc, a_ptr[1], b_ptr[11]);
  acc = mac(acc, a_ptr[2], b_ptr[10]);
  acc = mac(acc, a_ptr[3], b_ptr[9]);
  acc = mac(acc, a_ptr[4], b_ptr[8]);
  acc = mac(acc, a_ptr[5], b_ptr[7]);
  acc = mac(acc, a_ptr[6], b_ptr[6]);
  acc = mac(acc, a_ptr[7], b_ptr[5]);
  acc = mac(acc, a_ptr[8], b_ptr[4]);
  acc = mac(acc, a_ptr[9], b_ptr[3]);
  acc = mac(acc, a_ptr[10], b_ptr[2]);
  acc = mac(acc, a_ptr[11], b_ptr[1]);
  acc = mac(acc, a_ptr[12], b_ptr[0]);
  t_ptr[12] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = N_LIMBS .. (2*N_LIMBS - 2): accumulate a[i]*b[k-i] for i=(k-(N_LIMBS-1))..(N_LIMBS-1)
  // k = 13
  acc = mac(acc, a_ptr[1], b_ptr[12]);
  acc = mac(acc, a_ptr[2], b_ptr[11]);
  acc = mac(acc, a_ptr[3], b_ptr[10]);
  acc = mac(acc, a_ptr[4], b_ptr[9]);
  acc = mac(acc, a_ptr[5], b_ptr[8]);
  acc = mac(acc, a_ptr[6], b_ptr[7]);
  acc = mac(acc, a_ptr[7], b_ptr[6]);
  acc = mac(acc, a_ptr[8], b_ptr[5]);
  acc = mac(acc, a_ptr[9], b_ptr[4]);
  acc = mac(acc, a_ptr[10], b_ptr[3]);
  acc = mac(acc, a_ptr[11], b_ptr[2]);
  acc = mac(acc, a_ptr[12], b_ptr[1]);
  t_ptr[13] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 14
  acc = mac(acc, a_ptr[2], b_ptr[12]);
  acc = mac(acc, a_ptr[3], b_ptr[11]);
  acc = mac(acc, a_ptr[4], b_ptr[10]);
  acc = mac(acc, a_ptr[5], b_ptr[9]);
  acc = mac(acc, a_ptr[6], b_ptr[8]);
  acc = mac(acc, a_ptr[7], b_ptr[7]);
  acc = mac(acc, a_ptr[8], b_ptr[6]);
  acc = mac(acc, a_ptr[9], b_ptr[5]);
  acc = mac(acc, a_ptr[10], b_ptr[4]);
  acc = mac(acc, a_ptr[11], b_ptr[3]);
  acc = mac(acc, a_ptr[12], b_ptr[2]);
  t_ptr[14] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 15
  acc = mac(acc, a_ptr[3], b_ptr[12]);
  acc = mac(acc, a_ptr[4], b_ptr[11]);
  acc = mac(acc, a_ptr[5], b_ptr[10]);
  acc = mac(acc, a_ptr[6], b_ptr[9]);
  acc = mac(acc, a_ptr[7], b_ptr[8]);
  acc = mac(acc, a_ptr[8], b_ptr[7]);
  acc = mac(acc, a_ptr[9], b_ptr[6]);
  acc = mac(acc, a_ptr[10], b_ptr[5]);
  acc = mac(acc, a_ptr[11], b_ptr[4]);
  acc = mac(acc, a_ptr[12], b_ptr[3]);
  t_ptr[15] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 16
  acc = mac(acc, a_ptr[4], b_ptr[12]);
  acc = mac(acc, a_ptr[5], b_ptr[11]);
  acc = mac(acc, a_ptr[6], b_ptr[10]);
  acc = mac(acc, a_ptr[7], b_ptr[9]);
  acc = mac(acc, a_ptr[8], b_ptr[8]);
  acc = mac(acc, a_ptr[9], b_ptr[7]);
  acc = mac(acc, a_ptr[10], b_ptr[6]);
  acc = mac(acc, a_ptr[11], b_ptr[5]);
  acc = mac(acc, a_ptr[12], b_ptr[4]);
  t_ptr[16] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 17
  acc = mac(acc, a_ptr[5], b_ptr[12]);
  acc = mac(acc, a_ptr[6], b_ptr[11]);
  acc = mac(acc, a_ptr[7], b_ptr[10]);
  acc = mac(acc, a_ptr[8], b_ptr[9]);
  acc = mac(acc, a_ptr[9], b_ptr[8]);
  acc = mac(acc, a_ptr[10], b_ptr[7]);
  acc = mac(acc, a_ptr[11], b_ptr[6]);
  acc = mac(acc, a_ptr[12], b_ptr[5]);
  t_ptr[17] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 18
  acc = mac(acc, a_ptr[6], b_ptr[12]);
  acc = mac(acc, a_ptr[7], b_ptr[11]);
  acc = mac(acc, a_ptr[8], b_ptr[10]);
  acc = mac(acc, a_ptr[9], b_ptr[9]);
  acc = mac(acc, a_ptr[10], b_ptr[8]);
  acc = mac(acc, a_ptr[11], b_ptr[7]);
  acc = mac(acc, a_ptr[12], b_ptr[6]);
  t_ptr[18] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 19
  acc = mac(acc, a_ptr[7], b_ptr[12]);
  acc = mac(acc, a_ptr[8], b_ptr[11]);
  acc = mac(acc, a_ptr[9], b_ptr[10]);
  acc = mac(acc, a_ptr[10], b_ptr[9]);
  acc = mac(acc, a_ptr[11], b_ptr[8]);
  acc = mac(acc, a_ptr[12], b_ptr[7]);
  t_ptr[19] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 20
  acc = mac(acc, a_ptr[8], b_ptr[12]);
  acc = mac(acc, a_ptr[9], b_ptr[11]);
  acc = mac(acc, a_ptr[10], b_ptr[10]);
  acc = mac(acc, a_ptr[11], b_ptr[9]);
  acc = mac(acc, a_ptr[12], b_ptr[8]);
  t_ptr[20] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 21
  acc = mac(acc, a_ptr[9], b_ptr[12]);
  acc = mac(acc, a_ptr[10], b_ptr[11]);
  acc = mac(acc, a_ptr[11], b_ptr[10]);
  acc = mac(acc, a_ptr[12], b_ptr[9]);
  t_ptr[21] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 22
  acc = mac(acc, a_ptr[10], b_ptr[12]);
  acc = mac(acc, a_ptr[11], b_ptr[11]);
  acc = mac(acc, a_ptr[12], b_ptr[10]);
  t_ptr[22] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 23
  acc = mac(acc, a_ptr[11], b_ptr[12]);
  acc = mac(acc, a_ptr[12], b_ptr[11]);
  t_ptr[23] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 24
  acc = mac(acc, a_ptr[12], b_ptr[12]);
  t_ptr[24] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 2*N_LIMBS - 1: accumulator has the last limb
  t_ptr[2 * N_LIMBS - 1] = srs(acc, 0);

  // Get t_ptr
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
  GET_LIMB_FROM_ACC(13);
  GET_LIMB_FROM_ACC(14);
  GET_LIMB_FROM_ACC(15);
  GET_LIMB_FROM_ACC(16);
  GET_LIMB_FROM_ACC(17);
  GET_LIMB_FROM_ACC(18);
  GET_LIMB_FROM_ACC(19);
  GET_LIMB_FROM_ACC(20);
  GET_LIMB_FROM_ACC(21);
  GET_LIMB_FROM_ACC(22);
  GET_LIMB_FROM_ACC(23);
  GET_LIMB_FROM_ACC(24);
  GET_LIMB_FROM_ACC(25);
}
 