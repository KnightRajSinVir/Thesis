#pragma once

#include <adf.h>
#include "kernels/kernels.hpp"

using namespace adf;

class simple_graph: public graph {
private:
    kernel k0;
public:
    input_plio a_in;
    input_plio b_in;
    output_plio c_out;

    simple_graph() {
        k0 = kernel::create(k_dummy);
        source(k0) = "../src/kernels/k_dummy.cpp";

        a_in = input_plio::create("a_in", plio_128_bits, "../test/a_in.txt");
        b_in = input_plio::create("b_in", plio_128_bits, "../test/b_in.txt");
        c_out = output_plio::create("c_out", plio_128_bits, "test/c_out.txt");
        runtime<ratio>(k0) = 1.0;
        connect(a_in.out[0], k0.in[0]);
        connect(b_in.out[0], k0.in[1]);
        connect(k0.out[0], c_out.in[0]);
        dimensions(k0.in[0]) = {BUFFER_DEPTH * WORD_SIZE};
        dimensions(k0.in[1]) = {BUFFER_DEPTH * WORD_SIZE};
        dimensions(k0.out[0]) = {BUFFER_DEPTH * WORD_SIZE};
    }
};
