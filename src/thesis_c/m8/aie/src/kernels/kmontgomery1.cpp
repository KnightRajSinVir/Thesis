#include "kernels.hpp"

#define LIMB_SIZE 31
#define N_LIMBS 13

// Macro to extract carry from the accumulator after MAC operations by extracting
// the lower and upper parts of the accumulator, shifting them, and updating
// the accumulator with the shifted values.
#define EXTRACT_CARRY_FROM_ACC()                                                \
  do {                                                                          \
    acc_lo = ext_lo(acc);                                                       \
    acc_hi = ext_hi(acc);                                                       \
    carry_lo = lsrs(acc_lo, LIMB_SIZE);                                         \
    carry_hi = lsrs(acc_hi, LIMB_SIZE);                                         \
    acc_lo = lups(carry_lo, 0);                                                 \
    acc_hi = lups(carry_hi, 0);                                                 \
    acc = upd_lo(acc, acc_lo);                                                  \
    acc = upd_hi(acc, acc_hi);                                                  \
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
  v4acc80 acc_lo, acc_hi;
  v4int64 carry_lo, carry_hi;

  // Accumulator and vecroe for limb extraction 
  v8acc48 acc_limb_extraction;
  v8int32 limb, limb_top_bit;

  // k = 0 : a[0]b[0] is the first product
  acc = mul(a_ptr[0], b_ptr[0]);
  GET_LIMB_FROM_ACC();
  t_ptr[0] = limb;
  EXTRACT_CARRY_FROM_ACC();

  // k = 1 .. (N_LIMBS - 1): accumulate a[i]*b[k-i] for i=0..k
  for (int k = 1; k < N_LIMBS; k++) {
    for (int i = 0; i <= k; i++) {
      acc = mac(acc, a_ptr[i], b_ptr[k - i]);
    }
    GET_LIMB_FROM_ACC();
    t_ptr[k] = limb;
    EXTRACT_CARRY_FROM_ACC();
  }

  // k = N_LIMBS .. (2*N_LIMBS - 2): accumulate a[i]*b[k-i] for i=(k-(N_LIMBS-1))..(N_LIMBS-1)
  for (int k = N_LIMBS; k < 2 * N_LIMBS - 1; k++) {
    // i runs from (k - (N_LIMBS - 1)) .. (N_LIMBS - 1)
    int start = k - (N_LIMBS - 1);
    for (int i = start; i < N_LIMBS; i++) {
      acc = mac(acc, a_ptr[i], b_ptr[k - i]);
    }
    GET_LIMB_FROM_ACC();
    t_ptr[k] = limb;
    EXTRACT_CARRY_FROM_ACC();
  }

  // k = 2*N_LIMBS - 1: accumulator has the last limb
  t_ptr[2 * N_LIMBS - 1] = srs(acc, 0);
}
