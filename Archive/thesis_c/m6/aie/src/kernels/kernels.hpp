#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

#define BUFFER_DEPTH 128
#define SIMD_SIZE 8

void k_dummy(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict c_out
);

void k_1(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict t_out
);

void k_2(
    adf::input_buffer<int32>&__restrict t_in,
    adf::output_buffer<int32>&__restrict m_out
);

void k_3(
    adf::input_buffer<int32>&__restrict t_in,
    adf::input_buffer<int32>&__restrict m_in,
    adf::output_buffer<int32>&__restrict u_out
);