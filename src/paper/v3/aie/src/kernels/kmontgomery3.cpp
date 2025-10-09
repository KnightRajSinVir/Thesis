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
    acc_limb_extraction = ups(mxp_ptr[i], 0);                                     \
    mxp_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 31);              \
    acc_limb_extraction = ups(mxp_ptr[TOP_BIT_POINTER_OFFSET], 31);               \
    mxp_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 0);               \
    mxp_ptr[i] = mxp_ptr[i] + mxp_ptr[TOP_BIT_POINTER_OFFSET];                        \
  } while (0)
  

void kmontgomery3(
    adf::input_buffer<int32>& __restrict m_in,
    adf::output_buffer<int32>& __restrict mxp_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankB) *__restrict m_ptr = (v8int32 chess_storage(DM_bankB) *__restrict)m_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict mxp_ptr = (v8int32 chess_storage(DM_bankC) *__restrict)mxp_out.data();

  set_rnd(0); // floor rounding
  clr_sat();  // clear saturation

  // Accumulator used for MAC operations
  v8acc80 acc;

  // Accumulator and vector for carry extraction  
  v4acc80 acc_carry;
  v4int64 carry;

  // Accumulator and vecroe for limb extraction 
  v8acc48 acc_limb_extraction;

  // mxp = (m x p) / R

  // m x p
  // We will use mxp_ptr as the accumulator for m x p

  acc = mul(m_ptr[0], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[0] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 1 .. (N_LIMBS - 1): accumulate m[i]*q_limbs[k-i] for i=0..k
  // k = 1
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[1] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 2
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[2] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 3
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[3] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 4
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[4] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 5
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[5] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 6
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[6] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 7
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[7] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 8
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[8] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 9
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[9] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 10
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[10] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 11
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[11] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 12
  acc = mac(acc, m_ptr[0], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[1]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[0]));
  mxp_ptr[12] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = N_LIMBS .. (2*N_LIMBS - 2): accumulate m[i]*q_limbs[k-i] for i=(k-(N_LIMBS-1))..(N_LIMBS-1)
  // k = 13
  acc = mac(acc, m_ptr[1], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[2]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[1]));
  mxp_ptr[13] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 14
  acc = mac(acc, m_ptr[2], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[3]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[2]));
  mxp_ptr[14] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 15
  acc = mac(acc, m_ptr[3], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[4]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[3]));
  mxp_ptr[15] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 16
  acc = mac(acc, m_ptr[4], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[5]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[4]));
  mxp_ptr[16] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 17
  acc = mac(acc, m_ptr[5], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[6]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[5]));
  mxp_ptr[17] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 18
  acc = mac(acc, m_ptr[6], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[7]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[6]));
  mxp_ptr[18] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 19
  acc = mac(acc, m_ptr[7], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[8]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[7]));
  mxp_ptr[19] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 20
  acc = mac(acc, m_ptr[8], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[9]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[8]));
  mxp_ptr[20] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 21
  acc = mac(acc, m_ptr[9], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[10]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[9]));
  mxp_ptr[21] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 22
  acc = mac(acc, m_ptr[10], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[11]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[10]));
  mxp_ptr[22] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 23
  acc = mac(acc, m_ptr[11], *(v8int32 *) (p_limbs[12]));
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[11]));
  mxp_ptr[23] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 24
  acc = mac(acc, m_ptr[12], *(v8int32 *) (p_limbs[12]));
  mxp_ptr[24] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC_MAC();

  // k = 2*N_LIMBS - 1: accumulator has the last limb
  mxp_ptr[2 * N_LIMBS - 1] = srs(acc, 0);

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

}
