#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

#define SIMD_SIZE 8

void kmontgomery1(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict t_out
);

void kmontgomery2(
    adf::input_buffer<int32>&__restrict t_in,
    adf::output_buffer<int32>&__restrict m_out
);

void kmontgomery3(
    adf::input_buffer<int32>&__restrict t_in,
    adf::input_buffer<int32>&__restrict m_in,
    adf::output_buffer<int32>&__restrict u_out
);

void kadd(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict a_add_b_out
);

void ksub13_2N(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict a_sub_b_out
);

void ksub13_4N(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict a_sub_b_out
);


void ksub26_N(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict a_sub_b_out
);