#include "templates.hpp"

// This kernel is used to compute c = a +  (2 * p - b)

void ksub13_2N (
        IN_BUFFER a_in,
        IN_BUFFER b_in,
        OUT_BUFFER c_out
    ) 
{
    auto a_base = (BANK_A_PTR) a_in.data();
    auto b_base = (BANK_B_PTR) b_in.data();
    auto c_base = (BANK_C_PTR) c_out.data();
    auto two_p_base = (v8int32 *) (two_p_limbs);

    v8acc80 chess_storage(bm0) acc_0;
    v8acc80 chess_storage(bm1) acc_1;
    v8int32 chess_storage(wd0) carry;

    v8acc80  tmp_80;
    v16acc48 tmp_48;
    v16int32 v16int32_data_1;
    v16int32 v16int32_data_2;
    v8int32  v8int32_data;

    // Compute a + (2 * p - b)

    // 2 * p - b
    blocks_sub_full<10, 2>(a_base, two_p_base, b_base, carry, acc_0, acc_1);
    get_limb_repeat<10>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, c_base);
    get_limb_repeat<10, 20>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, c_base);

    // a + (2 * p - b)
    blocks_add_full<10, 2>(a_base, c_base, c_base, carry, acc_0, acc_1);
    get_limb_repeat<10>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, c_base);
    get_limb_repeat<10, 20>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, c_base);
}
