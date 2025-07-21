#include "kernels.hpp"

#define LIMB_SIZE 31
#define N_LIMBS 13

// Macros to extract carries in the case of addition 
// This macro is different as the result of the addition can be stored in just 64 bits 
// and not 128 bits. 

#define EXTRACT_CARRY_FROM_ACC_ADD()                                            \
  do {                                                                          \
    carry_add = srs(acc, LIMB_SIZE);                                           \
    acc = lups(carry_add, 0);                                                   \
  } while (0)


// Macro to get the limb from the accumulator after MAC operations by extracting
// the lower part of the accumulator, shifting it, and storing it in the output
// buffer.
#define GET_LIMB_FROM_ACC()                                                     \
  do {                                                                          \
    limb = srs(acc, 0);                                                         \
    acc_limb_extraction = ups(limb, 0);                                         \
    limb_top_bit = lsrs(acc_limb_extraction, 31);                               \
    acc_limb_extraction = ups(limb_top_bit, 31);                                \
    limb_top_bit = lsrs(acc_limb_extraction, 0);                                \
    limb = limb + limb_top_bit;                                                 \
  } while (0)

void kadd(
    adf::input_buffer<int32>& __restrict a_in,
    adf::input_buffer<int32>& __restrict b_in,
    adf::output_buffer<int32>& __restrict a_add_b_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankA) *__restrict a_ptr = (v8int32 chess_storage(DM_bankA) *__restrict)a_in.data();
  v8int32 chess_storage(DM_bankB) *__restrict b_ptr = (v8int32 chess_storage(DM_bankB) *__restrict)b_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict a_add_b_ptr = (v8int32 chess_storage(DM_bankC) *__restrict)a_add_b_out.data();

  set_rnd(0); // floor rounding
  clr_sat();  // clear saturation

  // Accumulator used for MAC operations
  v8acc80 acc;

  // Accumulator and vector for carry extraction  
  v4acc80 acc_lo, acc_hi;
  v4int64 carry_lo, carry_hi;
  v8int32 carry_add;

  // Accumulator and vecroe for limb extraction 
  v8acc48 acc_limb_extraction;
  v8int32 limb, limb_top_bit;

  // Modular addition
  // Calculate a + b

  // all ones
  v8int32 all_ones;
  for (unsigned int i = 0; i < 8; i++) {
    all_ones = upd_elem(all_ones, i, 0x1);
  }

  // Calculate a + b
  acc = mul(a_ptr[0], all_ones);
  acc = mac(acc, b_ptr[0], all_ones);
  GET_LIMB_FROM_ACC();
  a_add_b_ptr[0] = limb;
  EXTRACT_CARRY_FROM_ACC_ADD();

  // Calculate a + b
  for (int i = 1; i < N_LIMBS; i++) {
    acc = mac(acc, a_ptr[i], all_ones);
    acc = mac(acc, b_ptr[i], all_ones);
    GET_LIMB_FROM_ACC();
    a_add_b_ptr[i] = limb;
    EXTRACT_CARRY_FROM_ACC_ADD();
  }

}
