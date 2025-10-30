#pragma once

#include <adf.h>
#include "kernels/kernels.hpp"

#define BANK_SIZE 32

using namespace adf;

class simple_graph: public graph {
private:
    kernel lambda_montgomery1;
    kernel lambda_montgomery2;
    kernel lambda_montgomery3;
    kernel lambda_montgomery4;

    kernel lambda_square_montgomery1;
    kernel lambda_square_montgomery2;
    kernel lambda_square_montgomery3;
    kernel lambda_square_montgomery4;
    kernel x1_add_x2;
    kernel lambda_square_sub_x1_x2;

    kernel x1_sub_x3;
    kernel lambda_x1_x3_montgomery1;
    kernel lambda_x1_x3_montgomery2;
    kernel lambda_x1_x3_montgomery3;
    kernel lambda_x1_x3_montgomery4;
    kernel lambda_x1_x3_sub_y1;
    
public:
    input_plio x1_in;
    input_plio x2_in;
    input_plio y1_in;
    input_plio y2_sub_y1_in;
    input_plio x2_sub_x1_inv_in;

    output_plio x3_out;
    output_plio y3_out;

    simple_graph() {
        lambda_montgomery1 = kernel::create(kmontgomery1);
        lambda_montgomery2 = kernel::create(kmontgomery2);
        lambda_montgomery3 = kernel::create(kmontgomery3);
        lambda_montgomery4 = kernel::create(kmontgomery4);

        lambda_square_montgomery1 = kernel::create(kmontgomery1);
        lambda_square_montgomery2 = kernel::create(kmontgomery2);
        lambda_square_montgomery3 = kernel::create(kmontgomery3);
        lambda_square_montgomery4 = kernel::create(kmontgomery4);
        x1_add_x2 = kernel::create(kadd);
        lambda_square_sub_x1_x2 = kernel::create(ksub13_2N);
        
        x1_sub_x3 = kernel::create(ksub13_4N);
        lambda_x1_x3_montgomery1 = kernel::create(kmontgomery1);
        lambda_x1_x3_montgomery2 = kernel::create(kmontgomery2);
        lambda_x1_x3_montgomery3 = kernel::create(kmontgomery3);
        lambda_x1_x3_montgomery4 = kernel::create(kmontgomery4);
        lambda_x1_x3_sub_y1 = kernel::create(ksub26_N);

        // Kernel sources
        source(lambda_montgomery1) = "../src/kernels/kmontgomery1.cpp";
        source(lambda_montgomery2) = "../src/kernels/kmontgomery2.cpp";
        source(lambda_montgomery3) = "../src/kernels/kmontgomery3.cpp";
        source(lambda_montgomery4) = "../src/kernels/kmontgomery4.cpp";

        source(lambda_square_montgomery1) = "../src/kernels/kmontgomery1.cpp";
        source(lambda_square_montgomery2) = "../src/kernels/kmontgomery2.cpp";
        source(lambda_square_montgomery3) = "../src/kernels/kmontgomery3.cpp";
        source(lambda_square_montgomery4) = "../src/kernels/kmontgomery4.cpp";
        source(x1_add_x2) = "../src/kernels/kadd.cpp";
        source(lambda_square_sub_x1_x2) = "../src/kernels/ksub13_2N.cpp";
        
        source(x1_sub_x3) = "../src/kernels/ksub13_4N.cpp";
        source(lambda_x1_x3_montgomery1) = "../src/kernels/kmontgomery1.cpp";
        source(lambda_x1_x3_montgomery2) = "../src/kernels/kmontgomery2.cpp";
        source(lambda_x1_x3_montgomery3) = "../src/kernels/kmontgomery3.cpp";
        source(lambda_x1_x3_montgomery4) = "../src/kernels/kmontgomery4.cpp";
        source(lambda_x1_x3_sub_y1) = "../src/kernels/ksub26_N.cpp";

        // Input and output PLIOs
        x1_in = input_plio::create("x1_in", plio_128_bits, "../test/x1_in.txt");
        x2_in = input_plio::create("x2_in", plio_128_bits, "../test/x2_in.txt");
        y1_in = input_plio::create("y1_in", plio_128_bits, "../test/y1_in.txt");
        y2_sub_y1_in = input_plio::create("y2_sub_y1_in", plio_128_bits, "../test/y2_sub_y1_in.txt");
        x2_sub_x1_inv_in = input_plio::create("x2_sub_x1_inv_in", plio_128_bits, "../test/x2_sub_x1_inv_in.txt");

        x3_out = output_plio::create("x3_out", plio_128_bits, "test/x3_out.txt");
        y3_out = output_plio::create("y3_out", plio_128_bits, "test/y3_out.txt");

        // Runtime ratios
        runtime<ratio>(lambda_montgomery1) = 1.0;
        runtime<ratio>(lambda_montgomery2) = 1.0;
        runtime<ratio>(lambda_montgomery3) = 1.0;
        runtime<ratio>(lambda_montgomery4) = 1.0;

        runtime<ratio>(lambda_square_montgomery1) = 1.0;
        runtime<ratio>(lambda_square_montgomery2) = 1.0;
        runtime<ratio>(lambda_square_montgomery3) = 1.0;
        runtime<ratio>(lambda_square_montgomery4) = 1.0;
        runtime<ratio>(x1_add_x2) = 1.0;
        runtime<ratio>(lambda_square_sub_x1_x2) = 1.0;

        runtime<ratio>(x1_sub_x3) = 1.0;
        runtime<ratio>(lambda_x1_x3_montgomery1) = 1.0;
        runtime<ratio>(lambda_x1_x3_montgomery2) = 1.0;
        runtime<ratio>(lambda_x1_x3_montgomery3) = 1.0;
        runtime<ratio>(lambda_x1_x3_montgomery4) = 1.0;
        runtime<ratio>(lambda_x1_x3_sub_y1) = 1.0;

        // Calculating lambda
        connect(y2_sub_y1_in.out[0], lambda_montgomery1.in[0]);
        connect(x2_sub_x1_inv_in.out[0], lambda_montgomery1.in[1]);

        connect(lambda_montgomery1.out[0], lambda_montgomery2.in[0]);

        connect(lambda_montgomery2.out[0], lambda_montgomery3.in[0]);
        connect(lambda_montgomery1.out[0], lambda_montgomery4.in[0]);
        connect(lambda_montgomery3.out[0], lambda_montgomery4.in[1]);
        

        // Calculating x3 = lambda^2 - x1 - x2
        // lambda^2
        connect(lambda_montgomery4.out[0], lambda_square_montgomery1.in[0]);
        connect(lambda_montgomery4.out[0], lambda_square_montgomery1.in[1]);

        connect(lambda_square_montgomery1.out[0], lambda_square_montgomery2.in[0]);

        connect(lambda_square_montgomery2.out[0], lambda_square_montgomery3.in[0]);
        connect(lambda_square_montgomery1.out[0], lambda_square_montgomery4.in[0]);
        connect(lambda_square_montgomery3.out[0], lambda_square_montgomery4.in[1]);

        // x1 + x2
        connect(x1_in.out[0], x1_add_x2.in[0]);
        connect(x2_in.out[0], x1_add_x2.in[1]);

        // lambda^2 - x1 - x2
        connect(lambda_square_montgomery4.out[0], lambda_square_sub_x1_x2.in[0]);
        connect(x1_add_x2.out[0], lambda_square_sub_x1_x2.in[1]);

        // connect to output x3
        connect(lambda_square_sub_x1_x2.out[0], x3_out.in[0]);

        // Calculating y3 = lambda * (x1 - x3) - y1
        // x1 - x3
        connect(x1_in.out[0], x1_sub_x3.in[0]);
        connect(lambda_square_sub_x1_x2.out[0], x1_sub_x3.in[1]);

        // lambda * (x1 - x3)
        connect(lambda_montgomery4.out[0], lambda_x1_x3_montgomery1.in[0]);
        connect(x1_sub_x3.out[0], lambda_x1_x3_montgomery1.in[1]);

        connect(lambda_x1_x3_montgomery1.out[0], lambda_x1_x3_montgomery2.in[0]);

        connect(lambda_x1_x3_montgomery2.out[0], lambda_x1_x3_montgomery3.in[0]);
        connect(lambda_x1_x3_montgomery1.out[0], lambda_x1_x3_montgomery4.in[0]);
        connect(lambda_x1_x3_montgomery3.out[0], lambda_x1_x3_montgomery4.in[1]);

        // lambda * (x1 - x3) - y1
        connect(lambda_x1_x3_montgomery4.out[0], lambda_x1_x3_sub_y1.in[0]);
        connect(y1_in.out[0], lambda_x1_x3_sub_y1.in[1]);

        // connect to output y3
        connect(lambda_x1_x3_sub_y1.out[0], y3_out.in[0]);

        dimensions(lambda_montgomery1.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_montgomery1.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_montgomery1.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_montgomery2.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_montgomery2.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_montgomery3.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_montgomery3.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_montgomery4.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_montgomery4.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_montgomery4.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_square_montgomery1.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_square_montgomery1.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_square_montgomery1.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_square_montgomery2.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_square_montgomery2.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_square_montgomery3.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_square_montgomery3.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_square_montgomery4.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_square_montgomery4.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_square_montgomery4.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(x1_add_x2.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(x1_add_x2.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(x1_add_x2.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_square_sub_x1_x2.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_square_sub_x1_x2.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_square_sub_x1_x2.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(x1_sub_x3.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(x1_sub_x3.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(x1_sub_x3.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_x1_x3_montgomery1.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_x1_x3_montgomery1.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_x1_x3_montgomery1.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_x1_x3_montgomery2.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_x1_x3_montgomery2.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_x1_x3_montgomery3.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_x1_x3_montgomery3.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_x1_x3_montgomery4.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_x1_x3_montgomery4.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_x1_x3_montgomery4.out[0]) = {BANK_SIZE * SIMD_SIZE};

        dimensions(lambda_x1_x3_sub_y1.in[0]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_x1_x3_sub_y1.in[1]) = {BANK_SIZE * SIMD_SIZE};
        dimensions(lambda_x1_x3_sub_y1.out[0]) = {BANK_SIZE * SIMD_SIZE};
   
    }
};
