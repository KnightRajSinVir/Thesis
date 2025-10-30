#include "kernels.hpp"

#define LIMB_SIZE 28
#define N_LIMBS 10


#define TOP_BIT_POINTER_OFFSET 41

// #define GET_LIMB_FROM_ACC(i)                                                    \
//   do {                                                                          \
//     acc_limb_extraction = ups(c_base[i], 2);                                     \
//     c_base[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 31);              \
//     acc_limb_extraction = ups(c_base[TOP_BIT_POINTER_OFFSET], 31);               \
//     c_base[TOP_BIT_POINTER_OFFSET] = lsrs(acc_limb_extraction, 0);               \
//     c_base[i] = c_base[i] + c_base[TOP_BIT_POINTER_OFFSET];                        \
//     acc_limb_extraction = ups(c_base[i], 0);                                     \
//     c_base[i] = lsrs(acc_limb_extraction, 2);                                    \
//   } while (0)

#define GET_LIMB_FROM_ACC(i)                                                    \
  do {                                                                          \
    tmp_80 = lups(*(c_base + i), 4);                                           \
    tmp_48 = as_v16acc48(tmp_80);                                              \
    v16int32_data_1 = lsrs(tmp_48, 4);                                          \
    v16int32_data_2 = shuffle16(v16int32_data_1, 0, 0xECA86420, 0x0);          \
    v8int32_data = ext_w(v16int32_data_2, 0);                                   \
    *(c_base + i) = v8int32_data;                                              \
  } while (0)


static constexpr unsigned kLaneMask = 0x76543210;

// One term: load A[AI], B[BI] and do either lmul8 (first-of-block) or lmac8
template<int AI, int BI, bool First>
inline __attribute__((always_inline)) void mac_term(
    v8acc80 &acc, v16int32 &a, v16int32 &b,
    v8int32 chess_storage(DM_bankA) *restrict a_base,
    v8int32 chess_storage(DM_bankB) *restrict b_base) {
  a = upd_w(a, 0, *(a_base + AI));
  b = upd_w(b, 0, *(b_base + BI));
  if constexpr (First) {
    acc = lmul8(a, 0, kLaneMask, b, 0, kLaneMask);
  } else {
    acc = lmac8(acc, a, 0, kLaneMask, b, 0, kLaneMask);
  }
}

// Carry-out + limb store for C[C_OFF], then reload acc from carry
template<int C_OFF>
inline __attribute__((always_inline)) void carry_store(
    v8acc80 &acc, v8int32 &carry,
    v8int32 chess_storage(DM_bankC) *restrict c_base) {
  carry           = srs(acc, 28);
  *(c_base+C_OFF) = srs(acc, 0);
  acc             = lups(carry, 0);
}

// Inner unrolled row: AI = AI_START..AI_END (compile-time)
// FirstInit determines whether the *very first op of this row* is lmul8.
template<int N, int K, int A0, int B0, int AI, int AI_END, bool FirstInit>
struct InnerRow {
  static inline __attribute__((always_inline)) void run(
      v8acc80 &acc, v16int32 &a, v16int32 &b,
      v8int32 chess_storage(DM_bankA) *restrict a_base,
      v8int32 chess_storage(DM_bankB) *restrict b_base) {
    if constexpr (AI <= AI_END) {
      mac_term<A0 + AI, B0 + (K - AI), FirstInit>(acc, a, b, a_base, b_base);
      // Subsequent ops in the same row are always MACs
      InnerRow<N, K, A0, B0, AI + 1, AI_END, false>::run(acc, a, b, a_base, b_base);
    }
  }
};

// One output limb row k: compute valid AI range and emit all MACs + carry store
template<int N, int K, int A0, int B0, int C0>
struct Row {
  static inline __attribute__((always_inline)) void run(
      v8acc80 &acc, v16int32 &a, v16int32 &b, v8int32 &carry,
      v8int32 chess_storage(DM_bankA) *restrict a_base,
      v8int32 chess_storage(DM_bankB) *restrict b_base,
      v8int32 chess_storage(DM_bankC) *restrict c_base) {
    static_assert(N > 0, "N must be positive");

    // AI ranges over max(0, K-(N-1)) .. min(K, N-1)
    constexpr int AI_START = (K < N) ? 0 : (K - (N - 1));
    constexpr int AI_END   = (K < N) ? K : (N - 1);

    // FIRST OP OF THIS BLOCK: when we are at the very first row (K==0) and
    // the row's first valid AI is 0. This holds for every block independently.
    constexpr bool FirstOfBlock = (K == 0) && (AI_START == 0);

    InnerRow<N, K, A0, B0, AI_START, AI_END, FirstOfBlock>::run(acc, a, b, a_base, b_base);
    carry_store<C0 + K>(acc, carry, c_base);
  }
};

// Full schoolbook for one block (base offsets A0,B0 -> output at C0)
template<int N, int A0, int B0, int C0, int K = 0>
struct Schoolbook {
  static inline __attribute__((always_inline)) void run(
      v8acc80 &acc, v16int32 &a, v16int32 &b, v8int32 &carry,
      v8int32 chess_storage(DM_bankA) *restrict a_base,
      v8int32 chess_storage(DM_bankB) *restrict b_base,
      v8int32 chess_storage(DM_bankC) *restrict c_base) {
    Row<N, K, A0, B0, C0>::run(acc, a, b, carry, a_base, b_base, c_base);
    if constexpr (K + 1 < (2 * N - 1)) {
      Schoolbook<N, A0, B0, C0, K + 1>::run(acc, a, b, carry, a_base, b_base, c_base);
    }
  }
};

// -----------------------------------------------------------------------------
// Kernel: two independent 10-limb blocks
//   - Block 0: A[0..9]   * B[0..9]   -> C[0..18]   (acc_0)
//   - Block 1: A[10..19] * B[10..19] -> C[20..38]  (acc_1)
// Each block now starts with one lmul8, then only lmac8.
// -----------------------------------------------------------------------------
void k_dummy(
    adf::input_buffer<int32, adf::extents<320>>& __restrict a_in,
    adf::input_buffer<int32, adf::extents<320>>& __restrict b_in,
    adf::output_buffer<int32, adf::extents<640>>& __restrict t_out) {

  auto a_base = (v8int32 chess_storage(DM_bankA) *restrict) a_in.data();
  auto b_base = (v8int32 chess_storage(DM_bankB) *restrict) b_in.data();
  auto c_base = (v8int32 chess_storage(DM_bankC) *restrict) t_out.data();

  v8acc80  chess_storage(bm0) acc_0;
  v8acc80  chess_storage(bm1) acc_1;
  v16int32 chess_storage(xa)  a;
  v16int32 chess_storage(xb)  b;
  v8int32  chess_storage(wd0) carry;

  Schoolbook<10, 0, 0, 0>::run(acc_0, a, b, carry, a_base, b_base, c_base);
  Schoolbook<10, 10, 10, 20>::run(acc_1, a, b, carry, a_base, b_base, c_base);

  v8acc80 tmp_80;
  v16acc48 tmp_48;
  v16int32 v16int32_data_1, v16int32_data_2;
  v8int32 v8int32_data;
  // for (int i = 0; i < 40; i++) chess_prepare_for_pipelining {
  //   tmp_80 = lups(*(c_base + i), 4);
  //   tmp_48 = as_v16acc48(tmp_80);
  //   v16int32_data_1 = lsrs(tmp_48, 4);
  //   v16int32_data_2 = shuffle16(v16int32_data_1, 0, 0xECA86420, 0x0);
  //   v8int32_data = ext_w(v16int32_data_2, 0);
  //   *(c_base + i) = v8int32_data;
  // }

  // v8acc48 acc_limb_extraction;
  // for (int i = 0; i < 40; i++) chess_prepare_for_pipelining {
  //   acc_limb_extraction = ups(*(c_base + i), 2);
  //   *(c_base + TOP_BIT_POINTER_OFFSET) = lsrs(acc_limb_extraction, 31);
  //   acc_limb_extraction = ups(*(c_base + TOP_BIT_POINTER_OFFSET), 31);
  //   *(c_base + TOP_BIT_POINTER_OFFSET) = lsrs(acc_limb_extraction, 0);
  //   *(c_base + i) = *(c_base + i) + *(c_base + TOP_BIT_POINTER_OFFSET);
  //   acc_limb_extraction = ups(*(c_base + i), 0);
  //   *(c_base + i) = lsrs(acc_limb_extraction, 2);
  // }

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
  GET_LIMB_FROM_ACC(26);
  GET_LIMB_FROM_ACC(27);
  GET_LIMB_FROM_ACC(28);
  GET_LIMB_FROM_ACC(29);
  GET_LIMB_FROM_ACC(30);
  GET_LIMB_FROM_ACC(31);
  GET_LIMB_FROM_ACC(32);
  GET_LIMB_FROM_ACC(33);
  GET_LIMB_FROM_ACC(34);
  GET_LIMB_FROM_ACC(35);
  GET_LIMB_FROM_ACC(36);
  GET_LIMB_FROM_ACC(37);
  GET_LIMB_FROM_ACC(38);
  GET_LIMB_FROM_ACC(39);
}
