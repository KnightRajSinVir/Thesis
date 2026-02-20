#include "new_templates.hpp"

void kmontgomery2 (
        IN_BUFFER t_in,
        OUT_BUFFER m_out
    ) 
{

    auto t_base = (BANK_A_PTR) t_in.data();
    auto p_prime_base = (v8int32 *) (p_prime_limbs);
    auto m_base = (BANK_C_PTR) m_out.data();

    v8acc80  chess_storage(bm0) acc_0;
    v8acc80  chess_storage(bm1) acc_1;
    v16int32 chess_storage(xa)  a;
    v16int32 chess_storage(xb)  b;
    v8int32  chess_storage(wd0) carry;

    BlocksHalf<10, 2, BANK_A_PTR, v8int32 *, BANK_C_PTR>::run(a, b, carry, t_base, p_prime_base, m_base, acc_0, acc_1);

    v8acc80  tmp_80;
    v16acc48 tmp_48;
    v16int32 v16int32_data_1;
    v16int32 v16int32_data_2;
    v8int32  v8int32_data;
    
    get_limb_repeat<10>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, m_base);
    get_limb_repeat<10, 20>(tmp_80, tmp_48,v16int32_data_1, v16int32_data_2, v8int32_data, m_base);
}
