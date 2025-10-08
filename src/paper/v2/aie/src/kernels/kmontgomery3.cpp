#include "kernels.hpp"

#define LIMB_SIZE 31
#define N_LIMBS 13
#define TOP_BIT_POINTER_OFFSET 26
#define CARRY_POINTER_OFFSET 27

alignas(16) static const int32 p_limbs[13][8] = {
  0x7FFFAAAB, 0x7FFFAAAB, 0x7FFFAAAB, 0x7FFFAAAB, 0x7FFFAAAB, 0x7FFFAAAB, 0x7FFFAAAB, 0x7FFFAAAB,// limb  0 = bits [  0.. 30]
  0x73FDFFFF, 0x73FDFFFF, 0x73FDFFFF, 0x73FDFFFF, 0x73FDFFFF, 0x73FDFFFF, 0x73FDFFFF, 0x73FDFFFF,// limb  1 = bits [ 31.. 61]
  0x454FFFFE, 0x454FFFFE, 0x454FFFFE, 0x454FFFFE, 0x454FFFFE, 0x454FFFFE, 0x454FFFFE, 0x454FFFFE,// limb  2 = bits [ 62.. 92]
  0x755FFFF5, 0x755FFFF5, 0x755FFFF5, 0x755FFFF5, 0x755FFFF5, 0x755FFFF5, 0x755FFFF5, 0x755FFFF5,// limb  4 = bits [124..154]
  0x6B0F6241, 0x6B0F6241, 0x6B0F6241, 0x6B0F6241, 0x6B0F6241, 0x6B0F6241, 0x6B0F6241, 0x6B0F6241,// limb  5 = bits [155..185]
  0x661A541E, 0x661A541E, 0x661A541E, 0x661A541E, 0x661A541E, 0x661A541E, 0x661A541E, 0x661A541E,// limb  6 = bits [186..216]
  0x6144AFD9, 0x6144AFD9, 0x6144AFD9, 0x6144AFD9, 0x6144AFD9, 0x6144AFD9, 0x6144AFD9, 0x6144AFD9,// limb  7 = bits [217..247]
  0x3BA5C279, 0x3BA5C279, 0x3BA5C279, 0x3BA5C279, 0x3BA5C279, 0x3BA5C279, 0x3BA5C279, 0x3BA5C279,// limb  8 = bits [248..278]
  0x4BACD764, 0x4BACD764, 0x4BACD764, 0x4BACD764, 0x4BACD764, 0x4BACD764, 0x4BACD764, 0x4BACD764,// limb  8 = bits [248..278]
  0x374F6C86, 0x374F6C86, 0x374F6C86, 0x374F6C86, 0x374F6C86, 0x374F6C86, 0x374F6C86, 0x374F6C86,// limb  9 = bits [279..309]
  0x7F9A692C, 0x7F9A692C, 0x7F9A692C, 0x7F9A692C, 0x7F9A692C, 0x7F9A692C, 0x7F9A692C, 0x7F9A692C,// limb 10 = bits [310..340]
  0x088F51CB, 0x088F51CB, 0x088F51CB, 0x088F51CB, 0x088F51CB, 0x088F51CB, 0x088F51CB, 0x088F51CB,// limb 11 = bits [341..371]
  0x000001A0, 0x000001A0, 0x000001A0, 0x000001A0, 0x000001A0, 0x000001A0, 0x000001A0, 0x000001A0,// limb 12 = bits [372..402]
};

alignas(16) static const int32 all_ones[8] = {
  0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
};
alignas(16) static const int32 all_zeros[8] = {
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

// Macro to extract carry from the accumulator after MAC operations by extracting
// the lower and upper parts of the accumulator, shifting them, and updating
// the accumulator with the shifted values.
#define EXTRACT_CARRY_FROM_ACC_MAC()                                            \
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
    acc_limb_extraction = ups(u_ptr[i], 0);                                     \
    u_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 31);              \
    acc_limb_extraction = ups(u_ptr[TOP_BIT_POINTER_OFFSET], 31);               \
    u_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 0);               \
    u_ptr[i] = u_ptr[i] + u_ptr[TOP_BIT_POINTER_OFFSET];                        \
  } while (0)

// Macros to extract carries in the case of addition 
// This macro is different as the result of the addition can be stored in just 64 bits 
// and not 128 bits. 

#define EXTRACT_CARRY_FROM_ACC_ADD()                                            \
  do {                                                                          \
    u_ptr[CARRY_POINTER_OFFSET] = srs(acc, LIMB_SIZE);                          \
    acc = lups(u_ptr[CARRY_POINTER_OFFSET], 0);                                 \
  } while (0)

void kmontgomery3(
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
  v8acc80 chess_storage(bm0) acc;

  // Accumulator and vector for carry extraction  
  v4acc80 chess_storage(aml1) acc_carry;
  v4int64 chess_storage(wr0) carry;

  // Accumulator and vecroe for limb extraction 
  v8acc48 chess_storage(amh1) acc_limb_extraction;

  // u = (t + m x p) / R

  // m x p
  // We will use u_ptr as the accumulator for m x p

  acc = mul(m_ptr[0], *(v8int32 *) (p_limbs[0]));
  u_ptr[0] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 1 .. (N_LIMBS - 1): accumulate m[i]*q_limbs[k-i] for i=0..k
  for (int k = 1; k < N_LIMBS; k++) {
    for (int i = 0; i <= k; i++) {
      acc = mac(acc, m_ptr[i], *(v8int32 *) (p_limbs[k - i]));
    }
    u_ptr[k] = srs(acc, 0);
    EXTRACT_CARRY_FROM_ACC_MAC();
  }

  // k = N_LIMBS .. (2*N_LIMBS - 2): accumulate m[i]*q_limbs[k-i] for i=(k-(N_LIMBS-1))..(N_LIMBS-1)
  for (int k = N_LIMBS; k < 2 * N_LIMBS - 1; k++) {
    // i runs from (k - (N_LIMBS - 1)) .. (N_LIMBS - 1)
    int start = k - (N_LIMBS - 1);
    for (int i = start; i < N_LIMBS; i++) {
      acc = mac(acc, m_ptr[i], *(v8int32 *) (p_limbs[k - i]));
    }
    u_ptr[k] = srs(acc, 0);
    EXTRACT_CARRY_FROM_ACC_MAC();
  }

  // k = 2*N_LIMBS - 1: accumulator has the last limb
  u_ptr[2 * N_LIMBS - 1] = srs(acc, 0);

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

   // t + m x p

  // (t + m x p) / R
  acc = lups((*(v8int32 *) (all_zeros)), 0);
  for (int i = 0; i < 2 * N_LIMBS; i++) {
    acc = mac(acc, t_ptr[i], *(v8int32 *) (all_ones));
    acc = mac(acc, u_ptr[i], *(v8int32 *) (all_ones));
    u_ptr[i] = srs(acc, 0);
    EXTRACT_CARRY_FROM_ACC_ADD();
  }
  
  u_ptr[0] = u_ptr[0 + N_LIMBS];
  u_ptr[1] = u_ptr[1 + N_LIMBS];
  u_ptr[2] = u_ptr[2 + N_LIMBS];
  u_ptr[3] = u_ptr[3 + N_LIMBS];
  u_ptr[4] = u_ptr[4 + N_LIMBS];
  u_ptr[5] = u_ptr[5 + N_LIMBS];
  u_ptr[6] = u_ptr[6 + N_LIMBS];
  u_ptr[7] = u_ptr[7 + N_LIMBS];
  u_ptr[8] = u_ptr[8 + N_LIMBS];
  u_ptr[9] = u_ptr[9 + N_LIMBS];
  u_ptr[10] = u_ptr[10 + N_LIMBS];
  u_ptr[11] = u_ptr[11 + N_LIMBS];
  u_ptr[12] = u_ptr[12 + N_LIMBS];


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

  // Clear top N_LIMBS in u_ptr
  u_ptr[0 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[1 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[2 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[3 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[4 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[5 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[6 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[7 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[8 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[9 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[10 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[11 + N_LIMBS] = *(v8int32 *) (all_zeros);
  u_ptr[12 + N_LIMBS] = *(v8int32 *) (all_zeros);
}
