import helper_functions
from constants import bls12_381, bls12_381_modulo_inverse


p_limbs = [
    0x7FFFAAAB, 
    0x73FDFFFF, 
    0x454FFFFE, 
    0x755FFFF5, 
    0x6B0F6241, 
    0x661A541E, 
    0x6144AFD9, 
    0x3BA5C279, 
    0x4BACD764, 
    0x374F6C86, 
    0x7F9A692C, 
    0x088F51CB, 
    0x000001A0, 
]

p_prime_limbs = [
    0x7FFCFFFD, 
    0x13E7FFF9, 
    0x67444FA2, 
    0x4356DC96, 
    0x0E30B482, 
    0x5DE5DE19, 
    0x2CB6D305, 
    0x76650747, 
    0x68CF5819, 
    0x662DFDC4, 
    0x2BF251A2, 
    0x030837F5, 
    0x627A0CEB, 
]


p = 0
for i in range(13):
    p = p_limbs[i] * (1 << (31 * i)) + p

print(p == bls12_381)

p_prime = 0
for i in range(13):
    p_prime = p_prime_limbs[i] * (1 << (31 * i)) + p_prime

print(p_prime == bls12_381_modulo_inverse)

R = 1 << (13*31)

print(helper_functions.verify_parameters(p, p_prime, R))  