#pragma once

#include "constants.hpp"

#include <adf.h>
#include <aie_api/aie.hpp>

// BANK pointer aliases
using BANK_A_PTR = v8int32 chess_storage(DM_bankA) *restrict;
using BANK_B_PTR = v8int32 chess_storage(DM_bankB) *restrict;
using BANK_C_PTR = v8int32 chess_storage(DM_bankC) *restrict;

using IN_BUFFER  = adf::input_buffer<int32, adf::extents<640>>& __restrict;
using OUT_BUFFER = adf::output_buffer<int32, adf::extents<640>>& __restrict;

// -----------------------------------------------------------------------------
// Limb extract helper
// -----------------------------------------------------------------------------
#define GET_LIMB_FROM_ACC(i)                                                       \
    do {                                                                           \
        tmp_80 = lups(*(c_base + i), 5);                                           \
        tmp_48 = as_v16acc48(tmp_80);                                              \
        v16int32_data_1 = lsrs(tmp_48, 4);                                         \
        v16int32_data_2 = shuffle16(v16int32_data_1, 0, 0xECA86420, 0x0);          \
        v8int32_data = ext_w(v16int32_data_2, 0);                                  \
        tmp_80 = lups(v8int32_data, 0);                                            \
        *(c_base + i) = srs(tmp_80, 1);                                            \
    } while (0)

template<int N, int START = 0, int STRIDE = 1, class CPtr>
inline __attribute__((always_inline)) void get_limb_repeat(
    v8acc80 &tmp_80, v16acc48 &tmp_48,
    v16int32 &v16int32_data_1, v16int32 &v16int32_data_2,
    v8int32 &v8int32_data, CPtr c_base)
{
    if constexpr (N > 0) {
        GET_LIMB_FROM_ACC(START);
        get_limb_repeat<N - 1, START + STRIDE, STRIDE>(
            tmp_80, tmp_48, v16int32_data_1, v16int32_data_2, v8int32_data, c_base);
    }
}

// -----------------------------------------------------------------------------
// Lowest-level multiply-accumulate term (pointer types fully generic)
// -----------------------------------------------------------------------------
template<int AI, int BI, bool First, class APtr, class BPtr>
inline __attribute__((always_inline)) void mac_term(
    v8acc80 &acc, v16int32 &a, v16int32 &b, APtr a_base, BPtr b_base)
{
    a = upd_w(a, 0, *(a_base + AI));
    b = upd_w(b, 0, *(b_base + BI));
    if constexpr (First) {
        acc = lmul8(a, 0, kLaneMask, b, 0, kLaneMask);
    } else {
        acc = lmac8(acc, a, 0, kLaneMask, b, 0, kLaneMask);
    }
}

// -----------------------------------------------------------------------------
// Carry store for intermediate rows (generic c_base type)
// -----------------------------------------------------------------------------
template<int C_OFF, class CPtr>
inline __attribute__((always_inline)) void carry_store(
    v8acc80 &acc, v8int32 &carry, CPtr c_base)
{
    carry             = srs(acc, 27);
    *(c_base + C_OFF) = srs(acc, 0);
    acc               = lups(carry, 0);
}

// -----------------------------------------------------------------------------
// Inner row: fully unrolled A[i] * B[k-i] for fixed k
// -----------------------------------------------------------------------------
template<int N, int K, int A0, int B0, int AI, int AI_END, bool FirstInit, class APtr, class BPtr>
struct InnerRow {
    static inline __attribute__((always_inline)) void run(
        v8acc80 &acc, v16int32 &a, v16int32 &b, APtr a_base, BPtr b_base)
    {
        if constexpr (AI <= AI_END) {
            mac_term<A0 + AI, B0 + (K - AI), FirstInit>(acc, a, b, a_base, b_base);
            InnerRow<N, K, A0, B0, AI + 1, AI_END, false, APtr, BPtr>::run(
                acc, a, b, a_base, b_base);
        }
    }
};

template<int N, int K, int A0, int B0, int C0, class APtr, class BPtr, class CPtr>
struct Row
{
    static inline __attribute__((always_inline)) void run(
        v8acc80 &acc, v16int32 &a, v16int32 &b, v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base)
    {
        static_assert(N > 0, "N must be positive");

        constexpr int AI_START = (K < N) ? 0 : (K - (N - 1));
        constexpr int AI_END   = (K < N) ? K : (N - 1);

        // First op of block if and only if K==0 and AI_START==0
        constexpr bool FirstOfBlock = (K == 0) && (AI_START == 0);

        InnerRow<N, K, A0, B0, AI_START, AI_END, FirstOfBlock, APtr, BPtr>::run(
            acc, a, b, a_base, b_base);
        carry_store<C0 + K>(acc, carry, c_base);
    }
};

template<int N, int A0, int B0, int C0, class APtr, class BPtr, class CPtr, int K = 0>
struct Schoolbook {
    static inline __attribute__((always_inline)) void run(
        v8acc80 &acc, v16int32 &a, v16int32 &b, v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base)
    {
        if constexpr (K == (2 * N - 2)) {
            constexpr int AI_START = (K < N) ? 0 : (K - (N - 1));
            constexpr int AI_END   = (K < N) ? K : (N - 1);
            InnerRow<N, K, A0, B0, AI_START, AI_END, false, APtr, BPtr>::run(
                acc, a, b, a_base, b_base);

            *(c_base + C0 + K)     = srs(acc, 0);
            *(c_base + C0 + K + 1) = srs(acc, 27);
        } else {
            Row<N, K, A0, B0, C0, APtr, BPtr, CPtr>::run(
                acc, a, b, carry, a_base, b_base, c_base);
            Schoolbook<N, A0, B0, C0, APtr, BPtr, CPtr, K + 1>::run(
                acc, a, b, carry, a_base, b_base, c_base);
        }
    }
};

template<int N, int A0, int B0, int C0, class APtr, class BPtr, class CPtr, int K = 0>
struct SchoolbookHalf {
    static inline __attribute__((always_inline)) void run(
        v8acc80 &acc, v16int32 &a, v16int32 &b, v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base)
    {
        if constexpr (K == (N - 1)) {
            constexpr int AI_START = (K < N) ? 0 : (K - (N - 1));
            constexpr int AI_END   = (K < N) ? K : (N - 1);
            InnerRow<N, K, A0, B0, AI_START, AI_END, false, APtr, BPtr>::run(
                acc, a, b, a_base, b_base);

            *(c_base + C0 + K)     = srs(acc, 0);
        } else {
            Row<N, K, A0, B0, C0, APtr, BPtr, CPtr>::run(
                acc, a, b, carry, a_base, b_base, c_base);
            SchoolbookHalf<N, A0, B0, C0, APtr, BPtr, CPtr, K + 1>::run(
                acc, a, b, carry, a_base, b_base, c_base);
        }
    }
};

template<int IDX, typename... Ts>
struct AccAt;

template<typename First, typename... Rest>
struct AccAt<0, First, Rest...>
{
    using type = First;
    static inline __attribute__((always_inline)) First& get(First& f, Rest&...)
    {
        return f;
    }
};

template<int IDX, typename First, typename... Rest>
struct AccAt<IDX, First, Rest...>
{
    using type = typename AccAt<IDX - 1, Rest...>::type;
    static inline __attribute__((always_inline)) type& get(First&, Rest&... r)
    {
        return AccAt<IDX - 1, Rest...>::get(r...);
    }
};

template<int IDX, typename... T>
inline __attribute__((always_inline)) typename AccAt<IDX, T...>::type&
acc_select(T&... t)
{
    return AccAt<IDX, T...>::get(t...);
}

template<int N, int NB, class APtr, class BPtr, class CPtr, int B = 0>
struct Blocks
{
    template<typename... Accs>
    static inline __attribute__((always_inline)) void run(
        v16int32 &a, v16int32 &b, v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base,
        Accs&... accs)
    {
        if constexpr (B < NB) {
            constexpr int A0 = B * N;
            constexpr int B0 = B * N;
            constexpr int C0 = B * (2 * N);

            auto &accB = acc_select<B>(accs...);
            Schoolbook<N, A0, B0, C0, APtr, BPtr, CPtr>::run(
                accB, a, b, carry, a_base, b_base, c_base);

            Blocks<N, NB, APtr, BPtr, CPtr, B + 1>::template run<Accs...>(
                a, b, carry, a_base, b_base, c_base, accs...);
        }
    }
};

template<int N, int NB, class APtr, class BPtr, class CPtr, int B = 0>
struct BlocksHalf
{
    template<typename... Accs>
    static inline __attribute__((always_inline)) void run(
        v16int32 &a, v16int32 &b, v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base,
        Accs&... accs)
    {
        if constexpr (B < NB) {
            constexpr int A0 = B * N;
            constexpr int B0 = B * N;
            constexpr int C0 = B * (2 * N);

            auto &accB = acc_select<B>(accs...);
            SchoolbookHalf<N, A0, B0, C0, APtr, BPtr, CPtr>::run(
                accB, a, b, carry, a_base, b_base, c_base);

            BlocksHalf<N, NB, APtr, BPtr, CPtr, B + 1>::template run<Accs...>(
                a, b, carry, a_base, b_base, c_base, accs...);
        }
    }
};


// -----------------------------------------------------------------------------
// Addition
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Assumes:
//  - carry_store<C_OFF>(v8acc80&, v8int32&, CPtr) is defined
//  - all_28s and all_ones exist as v8int32-compatible constants for SubFull
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Single-block, full N-limb addition
// -----------------------------------------------------------------------------
template<
    int N, int A0, int B0, int C0,
    class APtr, class BPtr, class CPtr,
    int K = 0
>
struct AddFull
{
    static inline __attribute__((always_inline)) void run(
        v8acc80 &acc,
        v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base)
    {
        static_assert(N > 0, "N must be positive");

        if constexpr (K < N) {
            const v8int32 a_vec = *(a_base + (A0 + K));
            const v8int32 b_vec = *(b_base + (B0 + K));

            const v8int32 sum_vec =
                (K == 0)
                    ? (a_vec + b_vec)
                    : (a_vec + b_vec + carry);

            acc = lups(sum_vec, 0);
            carry_store<C0 + K>(acc, carry, c_base);

            AddFull<N, A0, B0, C0, APtr, BPtr, CPtr, K + 1>::run(
                acc, carry, a_base, b_base, c_base);
        }
    }
};

// -----------------------------------------------------------------------------
// Multi-block, full N-limb addition
// - One shared accumulator (acc) and carry across all NB blocks
// - Block b uses offsets:
//      A0 = b*N, B0 = b*N, C0 = b*N
// -----------------------------------------------------------------------------
template<
    int N, int NB,
    class APtr, class BPtr, class CPtr,
    int B = 0
>
struct BlocksAddFull
{
    static inline __attribute__((always_inline)) void run(
        v8acc80 &acc,
        v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base)
    {
        static_assert(N > 0,  "N must be positive");
        static_assert(NB > 0, "NB must be positive");

        if constexpr (B < NB) {
            constexpr int A0 = B * N;
            constexpr int B0 = B * N;
            constexpr int C0 = B * N;

            AddFull<N, A0, B0, C0, APtr, BPtr, CPtr>::run(
                acc, carry, a_base, b_base, c_base);

            BlocksAddFull<N, NB, APtr, BPtr, CPtr, B + 1>::run(
                acc, carry, a_base, b_base, c_base);
        }
    }
};


// -----------------------------------------------------------------------------
// Single-block, full N-limb subtraction
// Two's-complement style with 29-bit base (matches your original SubUpto logic)
// -----------------------------------------------------------------------------
template<
    int N, int A0, int B0, int C0,
    class APtr, class BPtr, class CPtr,
    int K = 0
>
struct SubFull
{
    static inline __attribute__((always_inline)) void run(
        v8acc80 &acc,
        v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base)
    {
        static_assert(N > 0, "N must be positive");

        if constexpr (K < N) {
            const v8int32 a_vec = *(a_base + (A0 + K));
            const v8int32 b_vec = *(b_base + (B0 + K));

            v8int32 sum_vec;
            if constexpr (K == 0) {
                // Initial biased limb
                sum_vec = *(const v8int32*)(all_28s) + a_vec - b_vec;
            } else if constexpr (K == N - 1) {
                // Final limb with -1 bias
                sum_vec = a_vec - b_vec - *(const v8int32*)(all_ones);
            } else {
                // Middle limbs: biased plus borrow
                sum_vec = *(const v8int32*)(all_28s) + a_vec - b_vec - carry;
            }

            acc = lups(sum_vec, 0);
            carry_store<C0 + K>(acc, carry, c_base);

            SubFull<N, A0, B0, C0, APtr, BPtr, CPtr, K + 1>::run(
                acc, carry, a_base, b_base, c_base);
        }
    }
};

// -----------------------------------------------------------------------------
// Multi-block, full N-limb subtraction
// - One shared accumulator and carry across NB blocks
// - Same C0 layout as addition: C0 = b*N
// -----------------------------------------------------------------------------
template<
    int N, int NB,
    class APtr, class BPtr, class CPtr,
    int B = 0
>
struct BlocksSubFull
{
    static inline __attribute__((always_inline)) void run(
        v8acc80 &acc,
        v8int32 &carry,
        APtr a_base,
        BPtr b_base,
        CPtr c_base)
    {
        static_assert(N > 0,  "N must be positive");
        static_assert(NB > 0, "NB must be positive");

        if constexpr (B < NB) {
            constexpr int A0 = B * N;
            constexpr int B0 = B * N;
            constexpr int C0 = B * N;

            SubFull<N, A0, B0, C0, APtr, BPtr, CPtr>::run(
                acc, carry, a_base, b_base, c_base);

            BlocksSubFull<N, NB, APtr, BPtr, CPtr, B + 1>::run(
                acc, carry, a_base, b_base, c_base);
        }
    }
};
