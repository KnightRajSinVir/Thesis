#include "kernels.hpp"

#define LIMB_SIZE 31
#define N_LIMBS 13

static const uint32_t q_limbs[13] = {
    0x5F0B597D,
    0x7FCB7270, 
    0x7FFFFFFF, 
    0x7FFFFFFF, 
    0x7FFFFFFF, 
    0x7FFFFFFF,
    0x7FFFFFFF, 
    0x7FFFFFFF, 
    0x7FFFFFFF, 
    0x7FFFFFFF, 
    0x7FFFFFFF,
    0x7FFFFFFF,
    0x0000001F
};

// Macro to extract carry from the accumulator after MAC operations by extracting
// the lower and upper parts of the accumulator, shifting them, and updating
// the accumulator with the shifted values.
#define EXTRACT_CARRY_FROM_ACC_MAC()                                            \
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
#define GET_LIMB_FROM_ACC_MAC()                                                 \
  do {                                                                          \
    limb = srs(acc, 0);                                                         \
    acc_limb_extraction = ups(limb, 0);                                         \
    limb_top_bit = lsrs(acc_limb_extraction, 31);                               \
    acc_limb_extraction = ups(limb_top_bit, 31);                                \
    limb_top_bit = lsrs(acc_limb_extraction, 0);                                \
    limb = limb + limb_top_bit;                                                 \
  } while (0)

// Macros to extract carries in the case of addition 
// This macro is different as the result of the addition can be stored in just 64 bits 
// and not 128 bits. 

#define EXTRACT_CARRY_FROM_ACC_ADD()                                            \
  do {                                                                          \
    carry_add = srs(acc, LIMB_SIZE);                                           \
    acc = lups(carry_add, 0);                                                   \
  } while (0)

void k_3(
    adf::input_buffer<int32>& __restrict t_in,
    adf::input_buffer<int32>& __restrict m_in,
    adf::output_buffer<int32>& __restrict u_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankA) *__restrict t_ptr = (v8int32 chess_storage(DM_bankA) *__restrict)t_in.data();
  v8int32 chess_storage(DM_bankB) *__restrict m_ptr = (v8int32 chess_storage(DM_bankB) *__restrict)m_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict u_ptr = (v8int32 chess_storage(DM_bankC) *__restrict)u_out.data();

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

  v8int32 q_limbs_vector[13];
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 8; j++) {
      q_limbs_vector[i] = upd_elem(q_limbs_vector[i], j, q_limbs[i]);
    }
  }

  // u = (t + m x p) / R

  // m x p
  // We will use u_ptr as the accumulator for m x p

  acc = mul(m_ptr[0], q_limbs_vector[0]);
  GET_LIMB_FROM_ACC_MAC();
  u_ptr[0] = limb;
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 1 .. (N_LIMBS - 1): accumulate m[i]*q_limbs[k-i] for i=0..k
  for (int k = 1; k < N_LIMBS; k++) {
    for (int i = 0; i <= k; i++) {
      acc = mac(acc, m_ptr[i], q_limbs_vector[k - i]);
    }
    GET_LIMB_FROM_ACC_MAC();
    u_ptr[k] = limb;
    EXTRACT_CARRY_FROM_ACC_MAC();
  }

  // k = N_LIMBS .. (2*N_LIMBS - 2): accumulate m[i]*q_limbs[k-i] for i=(k-(N_LIMBS-1))..(N_LIMBS-1)
  for (int k = N_LIMBS; k < 2 * N_LIMBS - 1; k++) {
    // i runs from (k - (N_LIMBS - 1)) .. (N_LIMBS - 1)
    int start = k - (N_LIMBS - 1);
    for (int i = start; i < N_LIMBS; i++) {
      acc = mac(acc, m_ptr[i], q_limbs_vector[k - i]);
    }
    GET_LIMB_FROM_ACC_MAC();
    u_ptr[k] = limb;
    EXTRACT_CARRY_FROM_ACC_MAC();
  }

  // k = 2*N_LIMBS - 1: accumulator has the last limb
  u_ptr[2 * N_LIMBS - 1] = srs(acc, 0);

   // t + m x p
  v8int32 all_ones;
  for (unsigned int i = 0; i < 8; i++) {
    all_ones = upd_elem(all_ones, i, 0x1);
  }

  v8int32 all_zeros;
  for (unsigned int i = 0; i < 8; i++) {
    all_zeros = upd_elem(all_zeros, i, 0x0);
  }

  // (t + m x p) / R
  acc = lups(all_zeros, 0);
  for (int i = 0; i < 2 * N_LIMBS; i++) {
    acc = mac(acc, t_ptr[i], all_ones);
    acc = mac(acc, u_ptr[i], all_ones);
    GET_LIMB_FROM_ACC_MAC();
    if (i >= N_LIMBS) {
      u_ptr[i - N_LIMBS] = limb;
    }
    EXTRACT_CARRY_FROM_ACC_ADD();
  }

  // Clear top N_LIMBS in u_ptr
  // for (int i = 0; i < N_LIMBS; i++) {
  //   u_ptr[i + N_LIMBS] = all_zeros;
  // }

}
