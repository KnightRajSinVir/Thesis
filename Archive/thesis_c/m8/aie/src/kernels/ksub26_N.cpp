#include "kernels.hpp"

// This kernel is used to compute c = a +  (p - b)
// a, b, c are 13-limb numbers

#define LIMB_SIZE 31
#define N_LIMBS 13

static const uint32_t p_limbs[13] = {
    0x7FFFAAAB, 
    0x73FDFFFF, 
    0x454FFFFE, 
    0x755FFFF5, 
    0x6B0F6241, 
    0x661A541E, 
    0x6144AFD9, 
    0x3BA5C279, 
    0x4BACD764, 
    0x374F6C86, 
    0x7F9A692C, 
    0x088F51CB, 
    0x000001A0, 
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

void ksub26_N(
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
  v4acc80 acc_lo, acc_hi;
  v4int64 carry_lo, carry_hi;
  v8int32 carry_add;

  // Accumulator and vecroe for limb extraction 
  v8acc48 acc_limb_extraction;
  v8int32 limb, limb_top_bit;

  // Compute a + (p - b)

  v8int32 p_limbs_vector[13];
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 8; j++) {
      p_limbs_vector[i] = upd_elem(p_limbs_vector[i], j, p_limbs[i]);
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

  // Algorithm for p - b
  // We know p > b
  // Always take a carry from the next limb 

  // p
  acc = mul(all_2, all_31);
  acc = mac(acc, p_limbs_vector[0], all_1);
  acc = mac(acc, - b_ptr[0], all_1);

  GET_LIMB_FROM_ACC();
  c_ptr[0] = limb;
  EXTRACT_CARRY_FROM_ACC_ADD();

  for (int i = 1; i < 12; i++) {
    acc = mac(acc, all_2, all_31);
    acc = mac(acc, p_limbs_vector[i], all_1);
    acc = mac(acc, - all_1, all_1);
    acc = mac(acc, - b_ptr[i], all_1);
    GET_LIMB_FROM_ACC();
    c_ptr[i] = limb;
    EXTRACT_CARRY_FROM_ACC_ADD();
  }

  acc = mac(acc, p_limbs_vector[12], all_1);
  acc = mac(acc, - all_1, all_1);
  acc = mac(acc, - b_ptr[12], all_1);

  GET_LIMB_FROM_ACC();
  c_ptr[12] = limb;
  EXTRACT_CARRY_FROM_ACC_ADD();

  // Allgorithm for a + (p - b)
  v8int32 all_zeros;
  for (unsigned int i = 0; i < 8; i++) {
    all_zeros = upd_elem(all_zeros, i, 0x0);
  }

  // a + (p - b)
  acc = lups(all_zeros, 0);
  for (int i = 0; i < 13; i++) {
    acc = mac(acc, a_ptr[i], all_1);
    acc = mac(acc, c_ptr[i], all_1);
    GET_LIMB_FROM_ACC();
    c_ptr[i] = limb;
    EXTRACT_CARRY_FROM_ACC_ADD();
  }

  // Propagating carry 
  for (int i = 13; i < 26; i++) {
    acc = mac(acc, a_ptr[i], all_1);
    GET_LIMB_FROM_ACC();
    c_ptr[i] = limb;
    EXTRACT_CARRY_FROM_ACC_ADD();
  }

}
