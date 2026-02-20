#include "templates.hpp"

void kmontgomery4 (
        IN_BUFFER t_in,
        IN_BUFFER mxp_in,
        OUT_BUFFER u_out
    ) 
{
    auto t_base   = (BANK_A_PTR) t_in.data();
    auto mxp_base = (BANK_B_PTR) mxp_in.data();
    auto u_base   = (BANK_C_PTR) u_out.data();

    v8acc80  chess_storage(bm0) acc_0;
    v8acc80  chess_storage(bm1) acc_1;
    v8int32  chess_storage(wd0) carry;

    v8acc80  tmp_80;
    v16acc48 tmp_48;
    v16int32 v16int32_data_1;
    v16int32 v16int32_data_2;
    v8int32  v8int32_data;

    blocks_add_full<10, 2>(t_base, mxp_base, u_base, carry, acc_0, acc_1);
  
    // Division by R
    u_base[0] = u_base[0 + N_LIMBS];
    u_base[1] = u_base[1 + N_LIMBS];
    u_base[2] = u_base[2 + N_LIMBS];
    u_base[3] = u_base[3 + N_LIMBS];
    u_base[4] = u_base[4 + N_LIMBS];
    u_base[5] = u_base[5 + N_LIMBS];
    u_base[6] = u_base[6 + N_LIMBS];
    u_base[7] = u_base[7 + N_LIMBS];
    u_base[8] = u_base[8 + N_LIMBS];
    u_base[9] = u_base[9 + N_LIMBS];


    get_limb_repeat<10>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, u_base);   

    // Clear top N_LIMBS in mxp_ptr
    u_base[0 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[1 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[2 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[3 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[4 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[5 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[6 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[7 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[8 + N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[9 + N_LIMBS] = *(v8int32 *) (all_zeros);

    // Division by R
    u_base[0 + 2 * N_LIMBS] = u_base[0 + 3 * N_LIMBS];
    u_base[1 + 2 * N_LIMBS] = u_base[1 + 3 * N_LIMBS];
    u_base[2 + 2 * N_LIMBS] = u_base[2 + 3 * N_LIMBS];
    u_base[3 + 2 * N_LIMBS] = u_base[3 + 3 * N_LIMBS];
    u_base[4 + 2 * N_LIMBS] = u_base[4 + 3 * N_LIMBS];
    u_base[5 + 2 * N_LIMBS] = u_base[5 + 3 * N_LIMBS];
    u_base[6 + 2 * N_LIMBS] = u_base[6 + 3 * N_LIMBS];
    u_base[7 + 2 * N_LIMBS] = u_base[7 + 3 * N_LIMBS];
    u_base[8 + 2 * N_LIMBS] = u_base[8 + 3 * N_LIMBS];
    u_base[9 + 2 * N_LIMBS] = u_base[9 + 3 * N_LIMBS];

    get_limb_repeat<10, 20>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, u_base);   

    // Clear top N_LIMBS in mxp_ptr
    u_base[0 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[1 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[2 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[3 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[4 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[5 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[6 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[7 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[8 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
    u_base[9 + 3 * N_LIMBS] = *(v8int32 *) (all_zeros);
}


// 4643
// 48784 ns