#include "kernels.hpp"

#define rnd_ceil 1
#define LIMB_SIZE 31
#define N_LIMBS 13

// Macro to extract carry from acc after the k=0 multiplication
#define EXTRACT_CARRY()                                                         \
  do {                                                                          \
    v4acc80 acc_lo = ext_lo(acc);                                               \
    v4acc80 acc_hi = ext_hi(acc);                                               \
    v4int64 carry_lo = lsrs(acc_lo, LIMB_SIZE);                                 \
    v4int64 carry_hi = lsrs(acc_hi, LIMB_SIZE);                                 \
    acc_lo = lups(carry_lo, 0);                                                 \
    acc_hi = lups(carry_hi, 0);                                                 \
    acc = upd_lo(acc, acc_lo);                                                  \
    acc = upd_hi(acc, acc_hi);                                                  \
  } while (0)

// Macro to extract the lower limb and load the next carry for general k ≥ 1
#define EXTRACT_LIMB_AND_CARRY(k)                                               \
  do {                                                                          \
    c_ptr[k] = srs(acc, 0);                                                     \
    v8int32 buffer = srs(acc, LIMB_SIZE);                                       \
    acc = lups(buffer, 0);                                                      \
  } while (0)

void k_dummy(
    adf::input_buffer<int32>& __restrict a_in,
    adf::input_buffer<int32>& __restrict b_in,
    adf::output_buffer<int32>& __restrict c_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankA) *__restrict a_ptr =
      (v8int32 chess_storage(DM_bankA) *__restrict)a_in.data();
  v8int32 chess_storage(DM_bankB) *__restrict b_ptr =
      (v8int32 chess_storage(DM_bankB) *__restrict)b_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict c_ptr =
      (v8int32 chess_storage(DM_bankC) *__restrict)c_out.data();

  set_rnd(0); // floor rounding
  clr_sat();  // clear saturation

  // === k = 0 case: single multiply, extract limb, then compute carry into acc ===
  v8acc80 acc = mul(a_ptr[0], b_ptr[0]);
  // get lower 31 bits for limb 0
  v8int32 res = srs(acc, 0);
  // mask off high bits to get exactly 31-bit limb
  v8acc48 res_clear = ups(res, /*zero-extend into top bits*/ 17);
  c_ptr[0] = lsrs(res_clear, 17);
  // now extract carry into acc for the next stage
  EXTRACT_CARRY();

  // === k = 1 .. (N_LIMBS - 1): accumulate a[i]*b[k-i] for i=0..k ===
  for (int k = 1; k < N_LIMBS; k++) {
    for (int i = 0; i <= k; i++) {
      acc = mac(acc, a_ptr[i], b_ptr[k - i]);
    }
    EXTRACT_LIMB_AND_CARRY(k);
  }

  // === k = N_LIMBS .. (2*N_LIMBS - 2): accumulate for the "upper half" ===
  // for k from 13 .. 24 (when N_LIMBS = 13)
  for (int k = N_LIMBS; k < 2 * N_LIMBS - 1; k++) {
    // i runs from (k - (N_LIMBS - 1)) .. (N_LIMBS - 1)
    int start = k - (N_LIMBS - 1);
    for (int i = start; i < N_LIMBS; i++) {
      acc = mac(acc, a_ptr[i], b_ptr[k - i]);
    }
    EXTRACT_LIMB_AND_CARRY(k);
  }

  // === Final carry after k = 2*N_LIMBS - 2 ===
  // At this point, acc holds the last carry. Output it as limb index (2*N_LIMBS - 1).
  c_ptr[2 * N_LIMBS - 1] = srs(acc, 0);
}
