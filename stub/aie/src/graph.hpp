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
        k0 = kernel::create(vec_add);

        a_in = input_plio::create(plio_128_bits, "../test/a_in.txt");
        b_in = input_plio::create(plio_128_bits, "../test/b_in.txt");
        c_out = output_plio::create(plio_128_bits, "test/c_out.txt");
        connect(a_in.out[0], k0.in[0]);
        connect(b_in.out[0], k0.in[1]);
        connect(k0.out[0], c_out.in[0]);
        dimensions(k0.in[0]) = {128 * 8};
        dimensions(k0.in[1]) = {128 * 8};
        dimensions(k0.out[0]) = {128 * 8};
        source(k0) = "./kernels/vec_add.cpp";
        runtime<ratio>(k0) = 1.0;
    }
};