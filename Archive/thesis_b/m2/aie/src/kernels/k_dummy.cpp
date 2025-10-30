#include "kernels.hpp"

#define P 17
#define R (1 << 10)   // R = 1024
#define NPRIME 783    // Precomputed: (-inv(P,R)) mod R, since 17*783 ≡ -1 (mod 1024)

int montgomery_reduce(int T) {
    int m = ((T % R) * NPRIME) % R;
    // Since R = 1024 is a power of 2, division by R is equivalent to a right shift of 10 bits.
    int t = (T + m * P) >> 10;  
    if (t >= P)
        t = t - P;
    return t;
}

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
        v8int32 c {};

        for (unsigned j = 0; j < 8; j++) {
            int a_elem = ext_elem(a, j);
            int b_elem = ext_elem(b, j);

            // Convert to montgomery form
            a_elem = (a_elem * R) % P;
            b_elem = (b_elem * R) % P;
            
            // T = a * b
            int T = a_elem * b_elem;
            int product_mont = montgomery_reduce(T);

            // Optionally, convert the result out of Montgomery form.
            // This is done by performing one more Montgomery reduction on the product.
            // To convert the result back we do a reduction on product_mont * 1
            int product = montgomery_reduce(product_mont * 1);

            c_ptr[i] = upd_elem(c_ptr[i], j, product);
        }
    }
}
