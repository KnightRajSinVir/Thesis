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
