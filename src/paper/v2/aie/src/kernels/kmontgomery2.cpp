#include "kernels.hpp"

#define LIMB_SIZE 31
#define N_LIMBS 13
#define TOP_BIT_POINTER_OFFSET 26

alignas(16) static const int32 p_prime_limbs[13][8] = {
  0x7FFCFFFD, 0x7FFCFFFD, 0x7FFCFFFD, 0x7FFCFFFD, 0x7FFCFFFD, 0x7FFCFFFD, 0x7FFCFFFD, 0x7FFCFFFD,// limb  0 = bits [  0.. 30]
  0x13E7FFF9, 0x13E7FFF9, 0x13E7FFF9, 0x13E7FFF9, 0x13E7FFF9, 0x13E7FFF9, 0x13E7FFF9, 0x13E7FFF9,// limb  1 = bits [ 31.. 61]
  0x67444FA2, 0x67444FA2, 0x67444FA2, 0x67444FA2, 0x67444FA2, 0x67444FA2, 0x67444FA2, 0x67444FA2,// limb  2 = bits [ 62.. 92]
  0x4356DC96, 0x4356DC96, 0x4356DC96, 0x4356DC96, 0x4356DC96, 0x4356DC96, 0x4356DC96, 0x4356DC96,// limb  3 = bits [ 93..123]
  0x0E30B482, 0x0E30B482, 0x0E30B482, 0x0E30B482, 0x0E30B482, 0x0E30B482, 0x0E30B482, 0x0E30B482,// limb  4 = bits [124..154]
  0x5DE5DE19, 0x5DE5DE19, 0x5DE5DE19, 0x5DE5DE19, 0x5DE5DE19, 0x5DE5DE19, 0x5DE5DE19, 0x5DE5DE19,// limb  5 = bits [155..185]
  0x2CB6D305, 0x2CB6D305, 0x2CB6D305, 0x2CB6D305, 0x2CB6D305, 0x2CB6D305, 0x2CB6D305, 0x2CB6D305,// limb  6 = bits [186..216]
  0x76650747, 0x76650747, 0x76650747, 0x76650747, 0x76650747, 0x76650747, 0x76650747, 0x76650747,// limb  7 = bits [217..247]
  0x68CF5819, 0x68CF5819, 0x68CF5819, 0x68CF5819, 0x68CF5819, 0x68CF5819, 0x68CF5819, 0x68CF5819, // limb  8 = bits [248..278]
  0x662DFDC4, 0x662DFDC4, 0x662DFDC4, 0x662DFDC4, 0x662DFDC4, 0x662DFDC4, 0x662DFDC4, 0x662DFDC4, // limb  9 = bits [279..309]
  0x2BF251A2, 0x2BF251A2, 0x2BF251A2, 0x2BF251A2, 0x2BF251A2, 0x2BF251A2, 0x2BF251A2, 0x2BF251A2, // limb 10 = bits [310..340]
  0x030837F5, 0x030837F5, 0x030837F5, 0x030837F5, 0x030837F5, 0x030837F5, 0x030837F5, 0x030837F5, // limb 11 = bits [341..371]
  0x627A0CEB, 0x627A0CEB, 0x627A0CEB, 0x627A0CEB, 0x627A0CEB, 0x627A0CEB, 0x627A0CEB, 0x627A0CEB, // limb 12 = bits [372..402]
};

// Macro to extract carry from the accumulator after MAC operations by extracting
// the lower and upper parts of the accumulator, shifting them, and updating
// the accumulator with the shifted values.
#define EXTRACT_CARRY_FROM_ACC()                                                \
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
    acc_limb_extraction = ups(m_ptr[i], 0);                                     \
    m_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 31);              \
    acc_limb_extraction = ups(m_ptr[TOP_BIT_POINTER_OFFSET], 31);               \
    m_ptr[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 0);               \
    m_ptr[i] = m_ptr[i] + m_ptr[TOP_BIT_POINTER_OFFSET];                        \
  } while (0)

void kmontgomery2(
    adf::input_buffer<int32>& __restrict t_in,
    adf::output_buffer<int32>& __restrict m_out
) {
  // pointers to vectors of 8 x int32 in DM banks
  v8int32 chess_storage(DM_bankA) *__restrict t_ptr = (v8int32 chess_storage(DM_bankA) *__restrict)t_in.data();
  v8int32 chess_storage(DM_bankC) *__restrict m_ptr = (v8int32 chess_storage(DM_bankC) *__restrict)m_out.data();

  set_rnd(0); // floor rounding
  clr_sat();  // clear saturation

  // Accumulator used for MAC operations
  v8acc80 chess_storage(bm0) acc;

  // Accumulator and vector for carry extraction  
  v4acc80 chess_storage(aml1) acc_carry;
  v4int64 chess_storage(wr0) carry;

  // Accumulator for limb extraction 
  v8acc48 chess_storage(amh1) acc_limb_extraction;

  // m = ( (t mod R) x p' ) mod R

  // k = 0 : t[0]*p_prime_limbs[0] is the first product
  acc = mul(t_ptr[0], *(v8int32 *) (p_prime_limbs[0]));
  m_ptr[0] = srs(acc, 0);
  EXTRACT_CARRY_FROM_ACC();

  // k = 1 .. (N_LIMBS - 1): accumulate t[i]*p_prime_limbs[k-i] for i=0..k
  for (int k = 1; k < N_LIMBS; k++) {
    for (int i = 0; i <= k; i++) {
      acc = mac(acc, t_ptr[i], *(v8int32 *) (p_prime_limbs[k - i]));
    }
    m_ptr[k] = srs(acc, 0);
    EXTRACT_CARRY_FROM_ACC();
  }

  // Extract limbs
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
