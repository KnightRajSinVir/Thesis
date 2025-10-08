#include "kernels.hpp"

#define LIMB_SIZE 31
#define N_LIMBS 13
#define TOP_BIT_POINTER_OFFSET 26
#define CARRY_POINTER_OFFSET 27

alignas(16) static const int32 all_ones[8] = {
  0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
};

// Macros to extract carries in the case of addition 
// This macro is different as the result of the addition can be stored in just 64 bits 
// and not 128 bits. 
#define EXTRACT_CARRY_FROM_ACC_ADD()                                            \
  do {                                                                          \
    a_add_b_ptr[CARRY_POINTER_OFFSET] = srs(acc, LIMB_SIZE);                          \
    acc = lups(a_add_b_ptr[CARRY_POINTER_OFFSET], 0);                                 \
  } while (0)

// Macro to get the limb from the accumulator after MAC operations by extracting
// the lower part of the accumulator, shifting it, and storing it in the output
// buffer.
#define GET_LIMB_FROM_ACC(i)                                                    \
  do {                                                                          \
    acc_limb_extraction = ups(a_add_b_ptr[i], 0);                                     \
    a_add_b_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 31);              \
    acc_limb_extraction = ups(a_add_b_ptr[TOP_BIT_POINTER_OFFSET], 31);               \
    a_add_b_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 0);               \
    a_add_b_ptr[i] = a_add_b_ptr[i] + a_add_b_ptr[TOP_BIT_POINTER_OFFSET];                        \
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
  v8acc80 chess_storage(bm0) acc;

  // Accumulator and vecroe for limb extraction 
  v8acc48 chess_storage(aml1) acc_limb_extraction;

  // Modular addition
  // Calculate a + b

  // Calculate a + b
  acc = mul(a_ptr[0], *(v8int32 *) (all_ones));
  acc = mac(acc, b_ptr[0], *(v8int32 *) (all_ones));
  a_add_b_ptr[0] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  // Calculate a + b
  for (int i = 1; i < N_LIMBS; i++) {
    acc = mac(acc, a_ptr[i], *(v8int32 *) (all_ones));
    acc = mac(acc, b_ptr[i], *(v8int32 *) (all_ones));
    a_add_b_ptr[i] = srs(acc, 0);
    EXTRACT_CARRY_FROM_ACC_ADD();
  }

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
}
