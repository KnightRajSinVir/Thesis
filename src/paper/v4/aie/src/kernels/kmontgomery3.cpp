// #include "templates.hpp"

// void kmontgomery3 (
//         IN_BUFFER m_in,
//         OUT_BUFFER mxp_out
//     ) 
// {
//     auto m_base = (BANK_A_PTR) m_in.data();
//     auto p_base = (v8int32 *) p_limbs;
//     auto mxp_base = (BANK_C_PTR) mxp_out.data();

//     v8acc80  chess_storage(bm0) acc_0;
//     v8acc80  chess_storage(bm1) acc_1;
//     v16int32 chess_storage(xa)  a;
//     v16int32 chess_storage(xb)  b;
//     v8int32  chess_storage(wd0) carry;

//     // Two layered blocks of size N=10 using two accumulators
//     blocks_full<10, 2>(a, b, carry, m_base, p_base, mxp_base, acc_0, acc_1);

//     v8acc80  tmp_80;
//     v16acc48 tmp_48;
//     v16int32 v16int32_data_1;
//     v16int32 v16int32_data_2;
//     v8int32  v8int32_data;

//     get_limb_repeat<40>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, mxp_base);   
// }

// 46889600
// 8179
// 179


#include "new_templates.hpp"

void kmontgomery3 (
        IN_BUFFER m_in,
        OUT_BUFFER mxp_out
    ) 
{
    auto m_base = (BANK_A_PTR) m_in.data();
    auto p_base = (v8int32 *) p_limbs;
    auto mxp_base = (BANK_C_PTR) mxp_out.data();

    v8acc80  chess_storage(bm0) acc_0;
    v8acc80  chess_storage(bm1) acc_1;
    v16int32 chess_storage(xa)  a;
    v16int32 chess_storage(xb)  b;
    v8int32  chess_storage(wd0) carry;

    // Two layered blocks of size N=10 using two accumulators
    Blocks<10, 2, BANK_A_PTR, v8int32 *, BANK_C_PTR>::run(a, b, carry, m_base, p_base, mxp_base, acc_0, acc_1);

    v8acc80  tmp_80;
    v16acc48 tmp_48;
    v16int32 v16int32_data_1;
    v16int32 v16int32_data_2;
    v8int32  v8int32_data;

    get_limb_repeat<40>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, mxp_base);   
}

// 48739200
// 7427
// 12

