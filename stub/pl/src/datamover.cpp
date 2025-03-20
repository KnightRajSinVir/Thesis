#include <ap_int.h>
#include <hls_stream.h>

void mm2s(
    ap_uint<256> *gmem,
    hls::stream<ap_uint<256>> &a_out,
    hls::stream<ap_uint<256>> &b_out,
    unsigned nof_samples
) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=m_axi port=gmem
#pragma HLS INTERFACE mode=axis port=a_out
#pragma HLS INTERFACE mode=axis port=b_out
#pragma HLS INTERFACE mode=s_axilite port=nof_samples

    for (unsigned i = 0; i < nof_samples; i++) {
#pragma HLS PIPELINE II=2
        a_out << gmem[i * 2 + 0];
        b_out << gmem[i * 2 + 1];
    }
}

void s2mm(
    ap_uint<256> *gmem,
    hls::stream<ap_uint<256>> &c_in,
    unsigned nof_samples
) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=m_axi port=gmem
#pragma HLS INTERFACE mode=axis port=c_in
#pragma HLS INTERFACE mode=s_axilite port=nof_samples

    for (unsigned i = 0; i < nof_samples; i++) {
#pragma HLS PIPELINE II=1
        gmem[i] = c_in.read();
    }
}
