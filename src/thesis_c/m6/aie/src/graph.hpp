#pragma once

#include <adf.h>
#include "kernels/kernels.hpp"

using namespace adf;

class simple_graph: public graph {
private:
    kernel k1;
    kernel k2;
    kernel k3;
public:
    input_plio a_in;
    input_plio b_in;
    output_plio c_out;

    simple_graph() {
        k1 = kernel::create(k_1);
        k2 = kernel::create(k_2);
        k3 = kernel::create(k_3);

        source(k1) = "../src/kernels/k1.cpp";
        source(k2) = "../src/kernels/k2.cpp";
        source(k3) = "../src/kernels/k3.cpp";


        a_in = input_plio::create("a_in", plio_128_bits, "../test/a_in.txt");
        b_in = input_plio::create("b_in", plio_128_bits, "../test/b_in.txt");
        c_out = output_plio::create("c_out", plio_128_bits, "test/c_out.txt");
        runtime<ratio>(k1) = 1.0;
        runtime<ratio>(k2) = 1.0;
        runtime<ratio>(k3) = 1.0;

        connect(a_in.out[0], k1.in[0]);
        connect(b_in.out[0], k1.in[1]);

        connect(k1.out[0], k2.in[0]);

        connect(k1.out[0], k3.in[0]);
        connect(k2.out[0], k3.in[1]);
        
        connect(k3.out[0], c_out.in[0]);
        dimensions(k1.in[0]) = {128 * SIMD_SIZE};
        dimensions(k1.in[1]) = {128 * SIMD_SIZE};
        dimensions(k1.out[0]) = {128 * SIMD_SIZE};
        dimensions(k2.in[0]) = {128 * SIMD_SIZE};
        dimensions(k2.out[0]) = {128 * SIMD_SIZE};
        dimensions(k3.in[0]) = {128 * SIMD_SIZE};
        dimensions(k3.in[1]) = {128 * SIMD_SIZE};
        dimensions(k3.out[0]) = {128 * SIMD_SIZE};
    }
};
