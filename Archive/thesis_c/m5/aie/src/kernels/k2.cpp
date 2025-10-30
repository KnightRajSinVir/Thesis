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

static const uint32_t p_prime_limbs[13] = {
    0x03B9582B,  // limb 0  = bits [0..30]
    0x17991598,  // limb 1  = bits [31..61]
    0x45D5F0EC,  // limb 2  = bits [62..92]
    0x36B80A29,  // limb 3  = bits [93..123]
    0x1E44CC4D,  // limb 4  = bits [124..154]
    0x5E4D75B4,  // limb 5  = bits [155..185]
    0x0300F497,  // limb 6  = bits [186..216]
    0x503836AC,  // limb 7  = bits [217..247]
    0x5113A058,  // limb 8  = bits [248..278]
    0x206CF5C6,  // limb 9  = bits [279..309]
    0x02186F7A,  // limb 10 = bits [310..340]
    0x253E18AF,  // limb 11 = bits [341..371]
    0x000005A1   // limb 12 = bits [372..402]
};

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

void k_2(
    adf::input_buffer<int32>& __restrict t_in,
    adf::output_buffer<int32>& __restrict m_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankA) *__restrict t_ptr = (v8int32 chess_storage(DM_bankA) *__restrict)t_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict m_ptr = (v8int32 chess_storage(DM_bankC) *__restrict)m_out.data();

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

  v8int32 p_prime_limbs_vector[13];
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 8; j++) {
      p_prime_limbs_vector[i] = upd_elem(p_prime_limbs_vector[i], j, p_prime_limbs[i]);
    }
  }

  // m = ( (t mod R) x p' ) mod R

  // k = 0 : t[0]*p_prime_limbs[0] is the first product
  acc = mul(t_ptr[0], p_prime_limbs_vector[0]);
  GET_LIMB_FROM_ACC();
  m_ptr[0] = limb;
  EXTRACT_CARRY_FROM_ACC();

  // k = 1 .. (N_LIMBS - 1): accumulate t[i]*p_prime_limbs[k-i] for i=0..k
  for (int k = 1; k < N_LIMBS; k++) {
    for (int i = 0; i <= k; i++) {
      acc = mac(acc, t_ptr[i], p_prime_limbs_vector[k - i]);
    }
    GET_LIMB_FROM_ACC();
    m_ptr[k] = limb;
    EXTRACT_CARRY_FROM_ACC();
  }
}
