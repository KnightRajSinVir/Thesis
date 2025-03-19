#include <ap_int.h>
#include <hls_stream.h>

void mm2s(
    ap_uint<256> *gmem,
    hls::stream<ap_uint<256>> &a,
    hls::stream<ap_uint<256>> &b,
    unsigned nof_samples
) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=m_axi port=gmem
#pragma HLS INTERFACE mode=axis port=a
#pragma HLS INTERFACE mode=axis port=b
#pragma HLS INTERFACE mode=s_axilite port=nof_samples

    for (unsigned i = 0; i < nof_samples; i++) {
#pragma HLS PIPELINE II=2
        a << gmem[i * 2 + 0];
        b << gmem[i * 2 + 1];
    }
}

void s2mm(
    ap_uint<256> *gmem,
    hls::stream<ap_uint<256>> &c,
    unsigned nof_samples
) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=m_axi port=gmem
#pragma HLS INTERFACE mode=axis port=c
#pragma HLS INTERFACE mode=s_axilite port=nof_samples

    for (unsigned i = 0; i < nof_samples; i++) {
#pragma HLS PIPELINE II=1
        gmem[i] = c.read();
    }
}
