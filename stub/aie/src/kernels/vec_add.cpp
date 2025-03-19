#include "kernels.hpp"

void vec_add(
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

    for (int i = 0; i < 1024; i++) {
        v8int32 a = *a_ptr;
        v8int32 b = *b_ptr;
        *c_ptr = a + b;
    }
}

