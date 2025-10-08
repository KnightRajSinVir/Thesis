#include "kernels.hpp"

// This kernel is used to compute c = a +  (2 * p - b)
// a, b, c are 13-limb numbers

#define LIMB_SIZE 31
#define N_LIMBS 13
#define TOP_BIT_POINTER_OFFSET 26
#define CARRY_POINTER_OFFSET 27

alignas(16) static const int32 two_p_limbs[13][8] = {
  0x7FFF5556, 0x7FFF5556, 0x7FFF5556, 0x7FFF5556, 0x7FFF5556, 0x7FFF5556, 0x7FFF5556, 0x7FFF5556, // limb 0  = bits [0..30]
  0x67FBFFFF, 0x67FBFFFF, 0x67FBFFFF, 0x67FBFFFF, 0x67FBFFFF, 0x67FBFFFF, 0x67FBFFFF, 0x67FBFFFF, // limb 1  = bits [31..61]
  0x0A9FFFFD, 0x0A9FFFFD, 0x0A9FFFFD, 0x0A9FFFFD, 0x0A9FFFFD, 0x0A9FFFFD, 0x0A9FFFFD, 0x0A9FFFFD, // limb 2  = bits [62..92]
  0x6ABFFFEB, 0x6ABFFFEB, 0x6ABFFFEB, 0x6ABFFFEB, 0x6ABFFFEB, 0x6ABFFFEB, 0x6ABFFFEB, 0x6ABFFFEB, // limb 3  = bits [93..123]
  0x561EC483, 0x561EC483, 0x561EC483, 0x561EC483, 0x561EC483, 0x561EC483, 0x561EC483, 0x561EC483, // limb 4  = bits [124..154]
  0x4C34A83D, 0x4C34A83D, 0x4C34A83D, 0x4C34A83D, 0x4C34A83D, 0x4C34A83D, 0x4C34A83D, 0x4C34A83D, // limb 5  = bits [155..185]
  0x42895FB3, 0x42895FB3, 0x42895FB3, 0x42895FB3, 0x42895FB3, 0x42895FB3, 0x42895FB3, 0x42895FB3, // limb 6  = bits [186..216]
  0x774B84F3, 0x774B84F3, 0x774B84F3, 0x774B84F3, 0x774B84F3, 0x774B84F3, 0x774B84F3, 0x774B84F3, // limb 7  = bits [217..247]
  0x1759AEC8, 0x1759AEC8, 0x1759AEC8, 0x1759AEC8, 0x1759AEC8, 0x1759AEC8, 0x1759AEC8, 0x1759AEC8, // limb 8  = bits [248..278]
  0x6E9ED90D, 0x6E9ED90D, 0x6E9ED90D, 0x6E9ED90D, 0x6E9ED90D, 0x6E9ED90D, 0x6E9ED90D, 0x6E9ED90D, // limb 9  = bits [279..309]
  0x7F34D258, 0x7F34D258, 0x7F34D258, 0x7F34D258, 0x7F34D258, 0x7F34D258, 0x7F34D258, 0x7F34D258, // limb 10 = bits [310..340]
  0x111EA397, 0x111EA397, 0x111EA397, 0x111EA397, 0x111EA397, 0x111EA397, 0x111EA397, 0x111EA397, // limb 11 = bits [341..371]
  0x00000340, 0x00000340, 0x00000340, 0x00000340, 0x00000340, 0x00000340, 0x00000340, 0x00000340, // limb 12 = bits [372..402]
};

alignas(16) static const int32 all_zeros[8] = {
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

alignas(16) static const int32 all_ones[8] = {
  0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
};

alignas(16) static const int32 all_twos[8] = {
  0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
};

alignas(16) static const int32 all_31s[8] = {
  0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000, 0x40000000,
};

// Macro to get the limb from the accumulator after MAC operations by extracting
// the lower part of the accumulator, shifting it, and storing it in the output
// buffer.
#define GET_LIMB_FROM_ACC(i)                                                    \
  do {                                                                          \
    acc_limb_extraction = ups(c_ptr[i], 0);                                     \
    c_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 31);              \
    acc_limb_extraction = ups(c_ptr[TOP_BIT_POINTER_OFFSET], 31);               \
    c_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 0);               \
    c_ptr[i] = c_ptr[i] + c_ptr[TOP_BIT_POINTER_OFFSET];                        \
  } while (0)

// Macros to extract carries in the case of addition 
// This macro is different as the result of the addition can be stored in just 64 bits 
// and not 128 bits. 
#define EXTRACT_CARRY_FROM_ACC_ADD()                                            \
  do {                                                                          \
    c_ptr[CARRY_POINTER_OFFSET] = srs(acc, LIMB_SIZE);                          \
    acc = lups(c_ptr[CARRY_POINTER_OFFSET], 0);                                 \
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
  v8acc80 chess_storage(bm0) acc;

  // Accumulator and vecroe for limb extraction 
  v8acc48 chess_storage(aml1) acc_limb_extraction;

  // Compute a + (2 * p - b)

  // Algorithm for 2 * p - b
  // We know 2 * p > b
  // Always take a carry from the next limb 

  // 2 * p
  acc = mul(*(v8int32 *) (all_twos), *(v8int32 *) (all_31s));
  acc = mac(acc, *(v8int32 *) (two_p_limbs[0]), *(v8int32 *) (all_ones));
  acc = mac(acc, - b_ptr[0], *(v8int32 *) (all_ones));
  c_ptr[0] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

  for (int i = 1; i < 12; i++) {
    acc = mac(acc, *(v8int32 *) (all_twos), *(v8int32 *) (all_31s));
    acc = mac(acc, *(v8int32 *) (two_p_limbs[i]), *(v8int32 *) (all_ones));
    acc = mac(acc, - *(v8int32 *) (all_ones), *(v8int32 *) (all_ones));
    acc = mac(acc, - b_ptr[i], *(v8int32 *) (all_ones));
    c_ptr[i] = srs(acc, 0);
    EXTRACT_CARRY_FROM_ACC_ADD();
  }

  acc = mac(acc, *(v8int32 *) (two_p_limbs[12]), *(v8int32 *) (all_ones));
  acc = mac(acc, - *(v8int32 *) (all_ones), *(v8int32 *) (all_ones));
  acc = mac(acc, - b_ptr[12], *(v8int32 *) (all_ones));
  c_ptr[12] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_ADD();

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

  // a + (2 * p - b)
  acc = lups(*(v8int32 *) (all_zeros), 0);
  for (int i = 0; i < 13; i++) {
    acc = mac(acc, a_ptr[i], *(v8int32 *) (all_ones));
    acc = mac(acc, c_ptr[i], *(v8int32 *) (all_ones));
    c_ptr[i] = srs(acc, 0);
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
