#include "kernels.hpp"

#define rnd_ceil 1

#define LIMB_SIZE 27

void k_dummy(
    adf::input_buffer<int32>&__restrict a_in,
    adf::input_buffer<int32>&__restrict b_in,
    adf::output_buffer<int32>&__restrict c_out
) {
// get pointers to our banks of data, a_ptr and b_ptr are inputs, c_ptr is the output
// pointers are to a_ptr vector of 8 signed int32
// __restrict tells the compiler that the pointers won't overlap
v8int32 chess_storage(DM_bankA) *__restrict a_ptr = (v8int32 chess_storage(DM_bankA) *__restrict) a_in.data();
v8int32 chess_storage(DM_bankB) *__restrict b_ptr = (v8int32 chess_storage(DM_bankB) *__restrict) b_in.data();
v8int32 chess_storage(DM_bankC) *__restrict c_ptr = (v8int32 chess_storage(DM_bankC) *__restrict) c_out.data();

    set_rnd(0); // set the rounding mode to floor
    clr_sat(); // clear saturation

    v8int32 buffer; // intermediate buffer for getting the carry

    v8acc80 acc = mul(a_ptr[0], b_ptr[0]);
    c_ptr[0] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[1], b_ptr[0]);
    acc = mac(acc, a_ptr[0], b_ptr[1]);
    c_ptr[1] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[2], b_ptr[0]);
    acc = mac(acc, a_ptr[1], b_ptr[1]);
    acc = mac(acc, a_ptr[0], b_ptr[2]);
    c_ptr[2] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
    
    acc = mac(acc, a_ptr[3], b_ptr[0]);
    acc = mac(acc, a_ptr[2], b_ptr[1]);
    acc = mac(acc, a_ptr[1], b_ptr[2]);
    acc = mac(acc, a_ptr[0], b_ptr[3]);
    c_ptr[3] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[4], b_ptr[0]);
    acc = mac(acc, a_ptr[3], b_ptr[1]);
    acc = mac(acc, a_ptr[2], b_ptr[2]);
    acc = mac(acc, a_ptr[1], b_ptr[3]);
    acc = mac(acc, a_ptr[0], b_ptr[4]);
    c_ptr[4] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[5], b_ptr[0]);
    acc = mac(acc, a_ptr[4], b_ptr[1]);
    acc = mac(acc, a_ptr[3], b_ptr[2]);
    acc = mac(acc, a_ptr[2], b_ptr[3]);
    acc = mac(acc, a_ptr[1], b_ptr[4]);
    acc = mac(acc, a_ptr[0], b_ptr[5]);
    c_ptr[5] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[6], b_ptr[0]);
    acc = mac(acc, a_ptr[5], b_ptr[1]);
    acc = mac(acc, a_ptr[4], b_ptr[2]);
    acc = mac(acc, a_ptr[3], b_ptr[3]);
    acc = mac(acc, a_ptr[2], b_ptr[4]);
    acc = mac(acc, a_ptr[1], b_ptr[5]);
    acc = mac(acc, a_ptr[0], b_ptr[6]);
    c_ptr[6] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[7], b_ptr[0]);
    acc = mac(acc, a_ptr[6], b_ptr[1]);
    acc = mac(acc, a_ptr[5], b_ptr[2]);
    acc = mac(acc, a_ptr[4], b_ptr[3]);
    acc = mac(acc, a_ptr[3], b_ptr[4]);
    acc = mac(acc, a_ptr[2], b_ptr[5]);
    acc = mac(acc, a_ptr[1], b_ptr[6]);
    acc = mac(acc, a_ptr[0], b_ptr[7]);
    c_ptr[7] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
    
    acc = mac(acc, a_ptr[8], b_ptr[0]);
    acc = mac(acc, a_ptr[7], b_ptr[1]);
    acc = mac(acc, a_ptr[6], b_ptr[2]);
    acc = mac(acc, a_ptr[5], b_ptr[3]);
    acc = mac(acc, a_ptr[4], b_ptr[4]);
    acc = mac(acc, a_ptr[3], b_ptr[5]);
    acc = mac(acc, a_ptr[2], b_ptr[6]);
    acc = mac(acc, a_ptr[1], b_ptr[7]);
    acc = mac(acc, a_ptr[0], b_ptr[8]);
    c_ptr[8] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
    
    acc = mac(acc, a_ptr[9], b_ptr[0]);
    acc = mac(acc, a_ptr[8], b_ptr[1]);
    acc = mac(acc, a_ptr[7], b_ptr[2]);
    acc = mac(acc, a_ptr[6], b_ptr[3]);
    acc = mac(acc, a_ptr[5], b_ptr[4]);
    acc = mac(acc, a_ptr[4], b_ptr[5]);
    acc = mac(acc, a_ptr[3], b_ptr[6]);
    acc = mac(acc, a_ptr[2], b_ptr[7]);
    acc = mac(acc, a_ptr[1], b_ptr[8]);
    acc = mac(acc, a_ptr[0], b_ptr[9]);
    c_ptr[9] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[10], b_ptr[0]);
    acc = mac(acc, a_ptr[9], b_ptr[1]);
    acc = mac(acc, a_ptr[8], b_ptr[2]);
    acc = mac(acc, a_ptr[7], b_ptr[3]);
    acc = mac(acc, a_ptr[6], b_ptr[4]);
    acc = mac(acc, a_ptr[5], b_ptr[5]);
    acc = mac(acc, a_ptr[4], b_ptr[6]);
    acc = mac(acc, a_ptr[3], b_ptr[7]);
    acc = mac(acc, a_ptr[2], b_ptr[8]);
    acc = mac(acc, a_ptr[1], b_ptr[9]);
    acc = mac(acc, a_ptr[0], b_ptr[10]);
    c_ptr[10] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
   
    acc = mac(acc, a_ptr[11], b_ptr[0]);
    acc = mac(acc, a_ptr[10], b_ptr[1]);    
    acc = mac(acc, a_ptr[9], b_ptr[2]);
    acc = mac(acc, a_ptr[8], b_ptr[3]);
    acc = mac(acc, a_ptr[7], b_ptr[4]);
    acc = mac(acc, a_ptr[6], b_ptr[5]);
    acc = mac(acc, a_ptr[5], b_ptr[6]);
    acc = mac(acc, a_ptr[4], b_ptr[7]);
    acc = mac(acc, a_ptr[3], b_ptr[8]);
    acc = mac(acc, a_ptr[2], b_ptr[9]);
    acc = mac(acc, a_ptr[1], b_ptr[10]);
    acc = mac(acc, a_ptr[0], b_ptr[11]);
    c_ptr[11] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
    
    acc = mac(acc, a_ptr[12], b_ptr[0]);
    acc = mac(acc, a_ptr[11], b_ptr[1]);
    acc = mac(acc, a_ptr[10], b_ptr[2]);
    acc = mac(acc, a_ptr[9], b_ptr[3]);
    acc = mac(acc, a_ptr[8], b_ptr[4]);
    acc = mac(acc, a_ptr[7], b_ptr[5]);
    acc = mac(acc, a_ptr[6], b_ptr[6]);
    acc = mac(acc, a_ptr[5], b_ptr[7]);
    acc = mac(acc, a_ptr[4], b_ptr[8]);
    acc = mac(acc, a_ptr[3], b_ptr[9]);
    acc = mac(acc, a_ptr[2], b_ptr[10]);
    acc = mac(acc, a_ptr[1], b_ptr[11]);
    acc = mac(acc, a_ptr[0], b_ptr[12]);
    c_ptr[12] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[0]);
    acc = mac(acc, a_ptr[12], b_ptr[1]);
    acc = mac(acc, a_ptr[11], b_ptr[2]);
    acc = mac(acc, a_ptr[10], b_ptr[3]);
    acc = mac(acc, a_ptr[9], b_ptr[4]);
    acc = mac(acc, a_ptr[8], b_ptr[5]);
    acc = mac(acc, a_ptr[7], b_ptr[6]);
    acc = mac(acc, a_ptr[6], b_ptr[7]);
    acc = mac(acc, a_ptr[5], b_ptr[8]);
    acc = mac(acc, a_ptr[4], b_ptr[9]);
    acc = mac(acc, a_ptr[3], b_ptr[10]);
    acc = mac(acc, a_ptr[2], b_ptr[11]);
    acc = mac(acc, a_ptr[1], b_ptr[12]);
    acc = mac(acc, a_ptr[0], b_ptr[13]);
    c_ptr[13] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
    
    acc = mac(acc, a_ptr[13], b_ptr[1]);
    acc = mac(acc, a_ptr[12], b_ptr[2]);
    acc = mac(acc, a_ptr[11], b_ptr[3]);
    acc = mac(acc, a_ptr[10], b_ptr[4]);
    acc = mac(acc, a_ptr[9], b_ptr[5]);
    acc = mac(acc, a_ptr[8], b_ptr[6]);
    acc = mac(acc, a_ptr[7], b_ptr[7]);
    acc = mac(acc, a_ptr[6], b_ptr[8]);
    acc = mac(acc, a_ptr[5], b_ptr[9]);
    acc = mac(acc, a_ptr[4], b_ptr[10]);
    acc = mac(acc, a_ptr[3], b_ptr[11]);
    acc = mac(acc, a_ptr[2], b_ptr[12]);
    acc = mac(acc, a_ptr[1], b_ptr[13]);
    c_ptr[14] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[2]);
    acc = mac(acc, a_ptr[12], b_ptr[3]);
    acc = mac(acc, a_ptr[11], b_ptr[4]);
    acc = mac(acc, a_ptr[10], b_ptr[5]);
    acc = mac(acc, a_ptr[9], b_ptr[6]);
    acc = mac(acc, a_ptr[8], b_ptr[7]);
    acc = mac(acc, a_ptr[7], b_ptr[8]);
    acc = mac(acc, a_ptr[6], b_ptr[9]);
    acc = mac(acc, a_ptr[5], b_ptr[10]);
    acc = mac(acc, a_ptr[4], b_ptr[11]);
    acc = mac(acc, a_ptr[3], b_ptr[12]);
    acc = mac(acc, a_ptr[2], b_ptr[13]);
    c_ptr[15] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
    
    acc = mac(acc, a_ptr[13], b_ptr[3]);
    acc = mac(acc, a_ptr[12], b_ptr[4]);
    acc = mac(acc, a_ptr[11], b_ptr[5]);
    acc = mac(acc, a_ptr[10], b_ptr[6]);
    acc = mac(acc, a_ptr[9], b_ptr[7]);
    acc = mac(acc, a_ptr[8], b_ptr[8]);
    acc = mac(acc, a_ptr[7], b_ptr[9]);
    acc = mac(acc, a_ptr[6], b_ptr[10]);
    acc = mac(acc, a_ptr[5], b_ptr[11]);
    acc = mac(acc, a_ptr[4], b_ptr[12]);
    acc = mac(acc, a_ptr[3], b_ptr[13]);
    c_ptr[16] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[4]);
    acc = mac(acc, a_ptr[12], b_ptr[5]);
    acc = mac(acc, a_ptr[11], b_ptr[6]);
    acc = mac(acc, a_ptr[10], b_ptr[7]);
    acc = mac(acc, a_ptr[9], b_ptr[8]);
    acc = mac(acc, a_ptr[8], b_ptr[9]);
    acc = mac(acc, a_ptr[7], b_ptr[10]);
    acc = mac(acc, a_ptr[6], b_ptr[11]);
    acc = mac(acc, a_ptr[5], b_ptr[12]);
    acc = mac(acc, a_ptr[4], b_ptr[13]);
    c_ptr[17] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[5]);
    acc = mac(acc, a_ptr[12], b_ptr[6]);
    acc = mac(acc, a_ptr[11], b_ptr[7]);
    acc = mac(acc, a_ptr[10], b_ptr[8]);
    acc = mac(acc, a_ptr[9], b_ptr[9]);
    acc = mac(acc, a_ptr[8], b_ptr[10]);
    acc = mac(acc, a_ptr[7], b_ptr[11]);
    acc = mac(acc, a_ptr[6], b_ptr[12]);
    acc = mac(acc, a_ptr[5], b_ptr[13]);
    c_ptr[18] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[6]);
    acc = mac(acc, a_ptr[12], b_ptr[7]);
    acc = mac(acc, a_ptr[11], b_ptr[8]);
    acc = mac(acc, a_ptr[10], b_ptr[9]);
    acc = mac(acc, a_ptr[9], b_ptr[10]);
    acc = mac(acc, a_ptr[8], b_ptr[11]);
    acc = mac(acc, a_ptr[7], b_ptr[12]);
    acc = mac(acc, a_ptr[6], b_ptr[13]);
    c_ptr[19] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[7]);
    acc = mac(acc, a_ptr[12], b_ptr[8]);
    acc = mac(acc, a_ptr[11], b_ptr[9]);
    acc = mac(acc, a_ptr[10], b_ptr[10]);
    acc = mac(acc, a_ptr[9], b_ptr[11]);
    acc = mac(acc, a_ptr[8], b_ptr[12]);
    acc = mac(acc, a_ptr[7], b_ptr[13]);
    c_ptr[20] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
    
    acc = mac(acc, a_ptr[13], b_ptr[8]);
    acc = mac(acc, a_ptr[12], b_ptr[9]);
    acc = mac(acc, a_ptr[11], b_ptr[10]);
    acc = mac(acc, a_ptr[10], b_ptr[11]);
    acc = mac(acc, a_ptr[9], b_ptr[12]);
    acc = mac(acc, a_ptr[8], b_ptr[13]);
    c_ptr[21] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[9]);
    acc = mac(acc, a_ptr[12], b_ptr[10]);
    acc = mac(acc, a_ptr[11], b_ptr[11]);
    acc = mac(acc, a_ptr[10], b_ptr[12]);
    acc = mac(acc, a_ptr[9], b_ptr[13]);
    c_ptr[22] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[10]);
    acc = mac(acc, a_ptr[12], b_ptr[11]);
    acc = mac(acc, a_ptr[11], b_ptr[12]);
    acc = mac(acc, a_ptr[10], b_ptr[13]);
    c_ptr[23] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  
    
    acc = mac(acc, a_ptr[13], b_ptr[11]);
    acc = mac(acc, a_ptr[12], b_ptr[12]);
    acc = mac(acc, a_ptr[11], b_ptr[13]);
    c_ptr[24] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[12]);
    acc = mac(acc, a_ptr[12], b_ptr[13]);
    c_ptr[25] = srs(acc, 0); 
    buffer = srs(acc, LIMB_SIZE); // get the carry
    acc = lups(buffer, 0);  

    acc = mac(acc, a_ptr[13], b_ptr[13]);
    c_ptr[26] = srs(acc, 0); 
    c_ptr[27] = srs(acc, LIMB_SIZE); 
}

