#include "kernels.hpp"

void k_ab_mul(
adf::input_buffer<int32, adf::extents<320>>&__restrict a_in,
adf::input_buffer<int32, adf::extents<320>>&__restrict b_in,
adf::output_buffer<int32, adf::extents<640>>&__restrict c_out
) {
v8int32 chess_storage(DM_bankA) *restrict a_ptr = (v8int32 chess_storage(DM_bankA) *restrict) a_in.data();
v8int32 chess_storage(DM_bankB) *restrict b_ptr = (v8int32 chess_storage(DM_bankB) *restrict) b_in.data();
v8int32 chess_storage(DM_bankC) *restrict c_ptr = (v8int32 chess_storage(DM_bankC) *restrict) c_out.data();

v8acc80 chess_storage(bm0) acc_0;
v8acc80 chess_storage(bm1) acc_1;
v8acc80 chess_storage(bm2) acc_2;
v8acc80 chess_storage(bm3) acc_3;
v16int32 chess_storage(xa) a;
v16int32 chess_storage(xb) b;
v8int32 chess_storage(wd0) carry;

// -- [col 0] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 0
// c_ptr = 0
a_ptr += 0;
b_ptr += 0;
c_ptr += 0;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmul8(a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 10
// c_ptr = 20
a_ptr += 10;
b_ptr += 10;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmul8(a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 20
// c_ptr = 40
a_ptr += 10;
b_ptr += 10;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmul8(a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 30
// c_ptr = 60
a_ptr += 10;
b_ptr += 10;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmul8(a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 1] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 1
a_ptr += -30;
b_ptr += -29;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 0
// c_ptr = 1
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 11
a_ptr += 9;
b_ptr += 11;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 10
// c_ptr = 21
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 21
a_ptr += 9;
b_ptr += 11;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 20
// c_ptr = 41
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 31
a_ptr += 9;
b_ptr += 11;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 30
// c_ptr = 61
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 2] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 2
a_ptr += -31;
b_ptr += -28;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 1
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 0
// c_ptr = 2
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 12
a_ptr += 8;
b_ptr += 12;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 11
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 10
// c_ptr = 22
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 22
a_ptr += 8;
b_ptr += 12;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 21
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 20
// c_ptr = 42
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 32
a_ptr += 8;
b_ptr += 12;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 31
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 30
// c_ptr = 62
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 3] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 3
a_ptr += -32;
b_ptr += -27;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 2
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 1
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 0
// c_ptr = 3
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 13
a_ptr += 7;
b_ptr += 13;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 12
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 11
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 10
// c_ptr = 23
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 23
a_ptr += 7;
b_ptr += 13;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 22
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 21
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 20
// c_ptr = 43
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 33
a_ptr += 7;
b_ptr += 13;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 32
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 31
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 30
// c_ptr = 63
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 4] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 4
a_ptr += -33;
b_ptr += -26;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 3
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 2
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 1
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 0
// c_ptr = 4
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 14
a_ptr += 6;
b_ptr += 14;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 13
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 12
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 11
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 10
// c_ptr = 24
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 24
a_ptr += 6;
b_ptr += 14;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 23
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 22
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 21
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 20
// c_ptr = 44
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 34
a_ptr += 6;
b_ptr += 14;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 33
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 32
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 31
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 30
// c_ptr = 64
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 5] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 5
a_ptr += -34;
b_ptr += -25;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 4
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 3
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 2
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 1
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 0
// c_ptr = 5
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 15
a_ptr += 5;
b_ptr += 15;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 14
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 13
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 12
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 11
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 10
// c_ptr = 25
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 25
a_ptr += 5;
b_ptr += 15;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 24
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 23
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 22
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 21
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 20
// c_ptr = 45
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 35
a_ptr += 5;
b_ptr += 15;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 34
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 33
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 32
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 31
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 30
// c_ptr = 65
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 6] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 6
a_ptr += -35;
b_ptr += -24;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 5
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 4
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 3
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 2
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 1
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 0
// c_ptr = 6
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 16
a_ptr += 4;
b_ptr += 16;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 15
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 14
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 13
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 12
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 11
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 10
// c_ptr = 26
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 26
a_ptr += 4;
b_ptr += 16;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 25
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 24
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 23
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 22
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 21
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 20
// c_ptr = 46
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 36
a_ptr += 4;
b_ptr += 16;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 35
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 34
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 33
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 32
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 31
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 30
// c_ptr = 66
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 7] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 7
a_ptr += -36;
b_ptr += -23;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 6
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 5
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 4
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 3
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 2
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 1
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 0
// c_ptr = 7
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 17
a_ptr += 3;
b_ptr += 17;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 16
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 15
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 14
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 13
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 12
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 11
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 10
// c_ptr = 27
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 27
a_ptr += 3;
b_ptr += 17;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 26
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 25
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 24
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 23
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 22
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 21
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 20
// c_ptr = 47
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 37
a_ptr += 3;
b_ptr += 17;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 36
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 35
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 34
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 33
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 32
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 31
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 30
// c_ptr = 67
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 8] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 8
a_ptr += -37;
b_ptr += -22;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 7
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 6
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 5
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 4
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 3
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 2
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 1
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 0
// c_ptr = 8
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 18
a_ptr += 2;
b_ptr += 18;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 17
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 16
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 15
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 14
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 13
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 12
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 11
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 10
// c_ptr = 28
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 28
a_ptr += 2;
b_ptr += 18;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 27
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 26
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 25
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 24
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 23
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 22
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 21
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 20
// c_ptr = 48
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 38
a_ptr += 2;
b_ptr += 18;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 37
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 36
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 35
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 34
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 33
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 32
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 31
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 30
// c_ptr = 68
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 9] ------------------------------------------------------------------
// a_ptr = 0
// b_ptr = 9
a_ptr += -38;
b_ptr += -21;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 1
// b_ptr = 8
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 7
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 6
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 5
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 4
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 3
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 2
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 1
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 0
// c_ptr = 9
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 10
// b_ptr = 19
a_ptr += 1;
b_ptr += 19;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 11
// b_ptr = 18
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 17
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 16
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 15
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 14
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 13
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 12
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 11
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 10
// c_ptr = 29
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 20
// b_ptr = 29
a_ptr += 1;
b_ptr += 19;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 21
// b_ptr = 28
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 27
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 26
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 25
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 24
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 23
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 22
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 21
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 20
// c_ptr = 49
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 30
// b_ptr = 39
a_ptr += 1;
b_ptr += 19;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 31
// b_ptr = 38
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 37
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 36
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 35
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 34
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 33
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 32
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 31
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 30
// c_ptr = 69
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 10] ------------------------------------------------------------------
// a_ptr = 1
// b_ptr = 9
a_ptr += -38;
b_ptr += -21;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 2
// b_ptr = 8
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 7
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 6
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 5
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 4
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 3
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 2
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 1
// c_ptr = 10
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 11
// b_ptr = 19
a_ptr += 2;
b_ptr += 18;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 12
// b_ptr = 18
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 17
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 16
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 15
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 14
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 13
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 12
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 11
// c_ptr = 30
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 21
// b_ptr = 29
a_ptr += 2;
b_ptr += 18;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 22
// b_ptr = 28
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 27
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 26
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 25
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 24
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 23
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 22
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 21
// c_ptr = 50
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 31
// b_ptr = 39
a_ptr += 2;
b_ptr += 18;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 32
// b_ptr = 38
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 37
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 36
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 35
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 34
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 33
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 32
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 31
// c_ptr = 70
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 11] ------------------------------------------------------------------
// a_ptr = 2
// b_ptr = 9
a_ptr += -37;
b_ptr += -22;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 3
// b_ptr = 8
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 7
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 6
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 5
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 4
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 3
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 2
// c_ptr = 11
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 12
// b_ptr = 19
a_ptr += 3;
b_ptr += 17;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 13
// b_ptr = 18
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 17
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 16
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 15
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 14
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 13
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 12
// c_ptr = 31
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 22
// b_ptr = 29
a_ptr += 3;
b_ptr += 17;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 23
// b_ptr = 28
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 27
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 26
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 25
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 24
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 23
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 22
// c_ptr = 51
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 32
// b_ptr = 39
a_ptr += 3;
b_ptr += 17;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 33
// b_ptr = 38
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 37
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 36
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 35
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 34
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 33
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 32
// c_ptr = 71
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 12] ------------------------------------------------------------------
// a_ptr = 3
// b_ptr = 9
a_ptr += -36;
b_ptr += -23;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 4
// b_ptr = 8
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 7
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 6
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 5
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 4
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 3
// c_ptr = 12
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 13
// b_ptr = 19
a_ptr += 4;
b_ptr += 16;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 14
// b_ptr = 18
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 17
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 16
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 15
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 14
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 13
// c_ptr = 32
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 23
// b_ptr = 29
a_ptr += 4;
b_ptr += 16;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 24
// b_ptr = 28
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 27
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 26
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 25
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 24
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 23
// c_ptr = 52
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 33
// b_ptr = 39
a_ptr += 4;
b_ptr += 16;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 34
// b_ptr = 38
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 37
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 36
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 35
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 34
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 33
// c_ptr = 72
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 13] ------------------------------------------------------------------
// a_ptr = 4
// b_ptr = 9
a_ptr += -35;
b_ptr += -24;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 5
// b_ptr = 8
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 7
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 6
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 5
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 4
// c_ptr = 13
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 14
// b_ptr = 19
a_ptr += 5;
b_ptr += 15;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 15
// b_ptr = 18
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 17
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 16
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 15
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 14
// c_ptr = 33
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 24
// b_ptr = 29
a_ptr += 5;
b_ptr += 15;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 25
// b_ptr = 28
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 27
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 26
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 25
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 24
// c_ptr = 53
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 34
// b_ptr = 39
a_ptr += 5;
b_ptr += 15;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 35
// b_ptr = 38
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 37
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 36
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 35
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 34
// c_ptr = 73
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 14] ------------------------------------------------------------------
// a_ptr = 5
// b_ptr = 9
a_ptr += -34;
b_ptr += -25;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 6
// b_ptr = 8
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 7
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 6
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 5
// c_ptr = 14
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 15
// b_ptr = 19
a_ptr += 6;
b_ptr += 14;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 16
// b_ptr = 18
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 17
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 16
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 15
// c_ptr = 34
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 25
// b_ptr = 29
a_ptr += 6;
b_ptr += 14;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 26
// b_ptr = 28
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 27
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 26
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 25
// c_ptr = 54
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 35
// b_ptr = 39
a_ptr += 6;
b_ptr += 14;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 36
// b_ptr = 38
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 37
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 36
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 35
// c_ptr = 74
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 15] ------------------------------------------------------------------
// a_ptr = 6
// b_ptr = 9
a_ptr += -33;
b_ptr += -26;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 7
// b_ptr = 8
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 7
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 6
// c_ptr = 15
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 16
// b_ptr = 19
a_ptr += 7;
b_ptr += 13;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 17
// b_ptr = 18
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 17
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 16
// c_ptr = 35
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 26
// b_ptr = 29
a_ptr += 7;
b_ptr += 13;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 27
// b_ptr = 28
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 27
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 26
// c_ptr = 55
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 36
// b_ptr = 39
a_ptr += 7;
b_ptr += 13;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 37
// b_ptr = 38
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 37
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 36
// c_ptr = 75
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 16] ------------------------------------------------------------------
// a_ptr = 7
// b_ptr = 9
a_ptr += -32;
b_ptr += -27;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 8
// b_ptr = 8
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 7
// c_ptr = 16
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 17
// b_ptr = 19
a_ptr += 8;
b_ptr += 12;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 18
// b_ptr = 18
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 17
// c_ptr = 36
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 27
// b_ptr = 29
a_ptr += 8;
b_ptr += 12;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 28
// b_ptr = 28
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 27
// c_ptr = 56
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 37
// b_ptr = 39
a_ptr += 8;
b_ptr += 12;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 38
// b_ptr = 38
a_ptr += 1;
b_ptr += -1;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 37
// c_ptr = 76
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 17] ------------------------------------------------------------------
// a_ptr = 8
// b_ptr = 9
a_ptr += -31;
b_ptr += -28;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 9
// b_ptr = 8
// c_ptr = 17
a_ptr += 1;
b_ptr += -1;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_0, 28);
*c_ptr = srs(acc_0, 0);
acc_0 = lups(carry, 0);

// a_ptr = 18
// b_ptr = 19
a_ptr += 9;
b_ptr += 11;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 19
// b_ptr = 18
// c_ptr = 37
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_1, 28);
*c_ptr = srs(acc_1, 0);
acc_1 = lups(carry, 0);

// a_ptr = 28
// b_ptr = 29
a_ptr += 9;
b_ptr += 11;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 29
// b_ptr = 28
// c_ptr = 57
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_2, 28);
*c_ptr = srs(acc_2, 0);
acc_2 = lups(carry, 0);

// a_ptr = 38
// b_ptr = 39
a_ptr += 9;
b_ptr += 11;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);

// a_ptr = 39
// b_ptr = 38
// c_ptr = 77
a_ptr += 1;
b_ptr += -1;
c_ptr += 20;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
carry = srs(acc_3, 28);
*c_ptr = srs(acc_3, 0);
acc_3 = lups(carry, 0);

// -- [col 18] ------------------------------------------------------------------
// a_ptr = 9
// b_ptr = 9
// c_ptr = 18
a_ptr += -30;
b_ptr += -29;
c_ptr += -59;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_0 = lmac8(acc_0, a, 0, 0x76543210, b, 0, 0x76543210);
*c_ptr++ = srs(acc_0, 0);
*c_ptr = srs(acc_0, 28);

// a_ptr = 19
// b_ptr = 19
// c_ptr = 38
a_ptr += 10;
b_ptr += 10;
c_ptr += 19;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_1 = lmac8(acc_1, a, 0, 0x76543210, b, 0, 0x76543210);
*c_ptr++ = srs(acc_1, 0);
*c_ptr = srs(acc_1, 28);

// a_ptr = 29
// b_ptr = 29
// c_ptr = 58
a_ptr += 10;
b_ptr += 10;
c_ptr += 19;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_2 = lmac8(acc_2, a, 0, 0x76543210, b, 0, 0x76543210);
*c_ptr++ = srs(acc_2, 0);
*c_ptr = srs(acc_2, 28);

// a_ptr = 39
// b_ptr = 39
// c_ptr = 78
a_ptr += 10;
b_ptr += 10;
c_ptr += 19;
a = upd_w(a, 0, *a_ptr);
b = upd_w(b, 0, *b_ptr);
acc_3 = lmac8(acc_3, a, 0, 0x76543210, b, 0, 0x76543210);
*c_ptr++ = srs(acc_3, 0);
*c_ptr = srs(acc_3, 28);

}
