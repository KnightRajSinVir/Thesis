#include "kernels.hpp"

void k_dummy(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict c_out
) {
// get pointers to our banks of data, a and b are inputs, c is the output
// pointers are to a vector of 8 signed int32
// __restrict tells the compiler that the pointers won't overlap
v8int32 chess_storage(DM_bankA) *__restrict a_ptr = (v8int32 chess_storage(DM_bankA) *__restrict) a_in.data();
v8int32 chess_storage(DM_bankB) *__restrict b_ptr = (v8int32 chess_storage(DM_bankB) *__restrict) b_in.data();
v8int32 chess_storage(DM_bankC) *__restrict c_ptr = (v8int32 chess_storage(DM_bankC) *__restrict) c_out.data();

    for (int i = 0; i < BUFFER_DEPTH; i++) {
        v8int32 a = a_ptr[i];
        v8int32 b = b_ptr[i];

        int dot_product = 0;
        for (unsigned j = 0; j < 8; j++) {
            int a_elem = ext_elem(a, j);
            int b_elem = ext_elem(b, j);
            dot_product += a_elem * b_elem;
        }

        c_ptr[i] = upd_elem(c_ptr[i], 0, dot_product);
    }
}
