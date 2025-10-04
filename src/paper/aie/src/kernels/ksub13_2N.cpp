#include "kernels.hpp"

// This kernel is used to compute c = a +  (2 * p - b)
// a, b, c are 13-limb numbers

#define LIMB_SIZE 31
#define N_LIMBS 13

static const uint32_t two_p_limbs[13] = {
  0x7FFF5556, // limb 0  = bits [0..30]
  0x67FBFFFF, // limb 1  = bits [31..61]
  0x0A9FFFFD, // limb 2  = bits [62..92]
  0x6ABFFFEB, // limb 3  = bits [93..123]
  0x561EC483, // limb 4  = bits [124..154]
  0x4C34A83D, // limb 5  = bits [155..185]
  0x42895FB3, // limb 6  = bits [186..216]
  0x774B84F3, // limb 7  = bits [217..247]
  0x1759AEC8, // limb 8  = bits [248..278]
  0x6E9ED90D, // limb 9  = bits [279..309]
  0x7F34D258, // limb 10 = bits [310..340]
  0x111EA397, // limb 11 = bits [341..371]
  0x00000340, // limb 12 = bits [372..402]
};

// Macro to get the limb from the accumulator after MAC operations by extracting
// the lower part of the accumulator, shifting it, and storing it in the output
// buffer.
#define GET_LIMB_FROM_ACC()                                                 \
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

void ksub13_2N(
    adf::input_buffer<int32>& __restrict a_in,
    adf::input_buffer<int32>& __restrict b_in,
    adf::output_buffer<int32>& __restrict c_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankA) *__restrict a_ptr = (v8int32 chess_storage(DM_bankA) *__restrict)a_in.data();
  v8int32 chess_storage(DM_bankB) *__restrict b_ptr = (v8int32 chess_storage(DM_bankB) *__restrict)b_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict c_ptr = (v8int32 chess_storage(DM_bankC) *__restrict)c_out.data();

  set_rnd(0); // floor rounding
  clr_sat();  // clear saturation

  // Accumulator used for MAC operations
  v8acc80 acc;

  // Accumulator and vector for carry extraction  
  v4acc80 chess_storage(aml1) acc_lo;
  v4acc80 chess_storage(amh1) acc_hi;
  v4int64 chess_storage(wr0) carry_lo;
  v4int64 chess_storage(wr1) carry_hi;
  v8int32 chess_storage(wc0) carry_add;

  // Accumulator and vecroe for limb extraction 
  v8acc48 chess_storage(aml2) acc_limb_extraction;
  v8int32 chess_storage(wr2) limb;
  v8int32 chess_storage(wr3) limb_top_bit;

  // Compute a + (2 * p - b)

  v8int32 two_p_limbs_vector[13];
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 8; j++) {
      two_p_limbs_vector[i] = upd_elem(two_p_limbs_vector[i], j, two_p_limbs[i]);
    }
  }
  v8int32 all_1;
  for (int i = 0; i < 13; i++) {
    all_1 = upd_elem(all_1, i, 0x1);
  }

  v8int32 all_2;
  for (int i = 0; i < 13; i++) {
    all_2 = upd_elem(all_2, i, 0x2);
  }

  v8int32 all_31;
  for (int i = 0; i < 13; i++) {
    all_31 = upd_elem(all_31, i, 0x40000000);
  }

  // Algorithm for 2 * p - b
  // We know 2 * p > b
  // Always take a carry from the next limb 

  // 2 * p
  acc = mul(all_2, all_31);
  acc = mac(acc, two_p_limbs_vector[0], all_1);
  acc = mac(acc, - b_ptr[0], all_1);

  GET_LIMB_FROM_ACC();
  c_ptr[0] = limb;
  EXTRACT_CARRY_FROM_ACC_ADD();

  for (int i = 1; i < 12; i++) {
    acc = mac(acc, all_2, all_31);
    acc = mac(acc, two_p_limbs_vector[i], all_1);
    acc = mac(acc, - all_1, all_1);
    acc = mac(acc, - b_ptr[i], all_1);
    GET_LIMB_FROM_ACC();
    c_ptr[i] = limb;
    EXTRACT_CARRY_FROM_ACC_ADD();
  }

  acc = mac(acc, two_p_limbs_vector[12], all_1);
  acc = mac(acc, - all_1, all_1);
  acc = mac(acc, - b_ptr[12], all_1);

  GET_LIMB_FROM_ACC();
  c_ptr[12] = limb;
  EXTRACT_CARRY_FROM_ACC_ADD();

  // Allgorithm for a + (2 * p - b)
  v8int32 all_zeros;
  for (unsigned int i = 0; i < 8; i++) {
    all_zeros = upd_elem(all_zeros, i, 0x0);
  }

  // a + (2 * p - b)
  acc = lups(all_zeros, 0);
  for (int i = 0; i < 13; i++) {
    acc = mac(acc, a_ptr[i], all_1);
    acc = mac(acc, c_ptr[i], all_1);
    GET_LIMB_FROM_ACC();
    c_ptr[i] = limb;
    EXTRACT_CARRY_FROM_ACC_ADD();
  }
}
