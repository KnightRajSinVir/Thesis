#include <iostream>
#include <cassert>
#include <xrt/xrt_device.h>
#include <xrt/xrt_bo.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_graph.h>

static constexpr uint32_t SIMD_LANES = 8;
static constexpr uint32_t WORD_SIZE = SIMD_LANES * sizeof(int);
static constexpr uint32_t TEST_SIZE_IN_WORDS = 512;
static constexpr uint32_t TEST_SIZE_IN_LANES = TEST_SIZE_IN_WORDS * SIMD_LANES;
static constexpr uint32_t TEST_SIZE_IN_BYTES = TEST_SIZE_IN_WORDS * WORD_SIZE;

int main() {
    std::cout << "initializing device" << std::endl;
    auto dev = xrt::device(0);
    std::cout << "loading image" << std::endl;
    auto bin = dev.load_xclbin("hw.xclbin");
    std::cout << "initializing pl kernels" << std::endl;
    auto k_mm2s = xrt::kernel(dev, bin, "mm2s");
    auto k_s2mm = xrt::kernel(dev, bin, "s2mm");

    std::cout << "creating test buffers" << std::endl;
    auto ab_buf = xrt::bo(dev, TEST_SIZE_IN_BYTES * 2, k_mm2s.group_id(0));
    auto c_buf = xrt::bo(dev, TEST_SIZE_IN_BYTES, k_s2mm.group_id(0));
    int *ab = ab_buf.map<int*>();
    int *c = c_buf.map<int*>();

    std::cout << "populating test data" << std::endl;

    int result[TEST_SIZE_IN_BYTES]{0};

    for (int i = 0; i < TEST_SIZE_IN_WORDS; i++) {
        // a_in
        ab[i * 16 + 0] = i + 0;
        ab[i * 16 + 1] = i + 1;
        ab[i * 16 + 2] = i + 2;
        ab[i * 16 + 3] = i + 3;
        ab[i * 16 + 4] = i + 4;
        ab[i * 16 + 5] = i + 5;
        ab[i * 16 + 6] = i + 6;
        ab[i * 16 + 7] = i + 7;
        // b_in
        ab[i * 16 + 8] = i + 8;
        ab[i * 16 + 9] = i + 9;
        ab[i * 16 + 10] = i + 10;
        ab[i * 16 + 11] = i + 11;
        ab[i * 16 + 12] = i + 12;
        ab[i * 16 + 13] = i + 13;
        ab[i * 16 + 14] = i + 14;
        ab[i * 16 + 15] = i + 15;

        for (int j = 0; j < 8; j++) {
            result[i * 8] += (i + j) * (i + j + 8);
        }
    }

    std::cout << "copying test data to device" << std::endl;
    ab_buf.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "running datamovers" << std::endl;
    xrt::run run_mm2s(k_mm2s);
    xrt::run run_s2mm(k_s2mm);
    run_mm2s.set_arg("gmem", ab_buf);
    run_mm2s.set_arg("nof_words", TEST_SIZE_IN_WORDS);
    run_s2mm.set_arg("gmem", c_buf);
    run_s2mm.set_arg("nof_words", TEST_SIZE_IN_WORDS);
    run_mm2s.start();
    run_s2mm.start();
    std::cout << "waiting..." << std::endl;
    run_mm2s.wait();
    run_s2mm.wait();
    std::cout << "execution complete" << std::endl;

    std::cout << "copying results back to host" << std::endl;
    c_buf.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    std::cout << std::endl;
    for (int i = 0; i < TEST_SIZE_IN_WORDS; i = i + 8) {
        assert(c[i] == result[i]);
    }
    std::cout << "test passed!" << std::endl;

    return 0;
}
