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
  v8acc80 chess_storage(bm0) acc;

  // Accumulator and vector for carry extraction  
  v4acc80 chess_storage(aml1) acc_carry;
  v4int64 chess_storage(wr0) carry;

  // Accumulator for limb extraction 
  v8acc48 chess_storage(amh1) acc_limb_extraction;

  // k = 0 : a[0]b[0] is the first product
  acc = mul(a_ptr[0], b_ptr[0]);
  t_ptr[0] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 1 .. (N_LIMBS - 1): accumulate a[i]*b[k-i] for i=0..k
  for (int k = 1; k < N_LIMBS; k++) {
    for (int i = 0; i <= k; i++) {
      acc = mac(acc, a_ptr[i], b_ptr[k - i]);
    }
    t_ptr[k] = srs(acc, 0);
    EXTRACT_CARRY_FROM_ACC();
  }

  // k = N_LIMBS .. (2*N_LIMBS - 2): accumulate a[i]*b[k-i] for i=(k-(N_LIMBS-1))..(N_LIMBS-1)
  for (int k = N_LIMBS; k < 2 * N_LIMBS - 1; k++) {
    // i runs from (k - (N_LIMBS - 1)) .. (N_LIMBS - 1)
    int start = k - (N_LIMBS - 1);
    for (int i = start; i < N_LIMBS; i++) {
      acc = mac(acc, a_ptr[i], b_ptr[k - i]);
    }
    t_ptr[k] = srs(acc, 0);
    EXTRACT_CARRY_FROM_ACC();
  }

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
 