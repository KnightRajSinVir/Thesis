from constants import bls12_381, bls12_381_modulo_inverse, R
import helper_functions
from sympy import mod_inverse
from math import log2
import random

# Elliptical Curve Addition
# A + B = C
# A = (x1, y1)
# B = (x2, y2)
# C = (x3, y3)
# N = modulus of the field
# R = Large power of 2 for the montgomery multiplication algorithm

# Elliptical Curve Addition Formula
# x3 = (lambda^2 - x1 - x2) % N
# y3 = (lambda * (x1 - x3) - y1) % N
# lambda = (y2 - y1) / (x2 - x1) % N
# if A == B, then lambda = (3 * x1^2 + a) / (2 * y1) % N



def elliptical_curve_addition_thesis_model(x1_mont, y1_mont, x2_mont, y2_mont, l_mont, N = bls12_381, N_prime = bls12_381_modulo_inverse, R = R):
    # Enforcing size limitations of intermediate values and inputs
    # x1_mont, y1_mont, x2_mont, y2_mont, l_mont are in the range [0, N - 1]
    # N_prime is in the range [0, N - 1]

    if not(0 <= x1_mont < N and 0 <= y1_mont < N and 0 <= x2_mont < N and 0 <= y2_mont < N and 0 <= l_mont < N):
        print("Input values are not in the range [0, N - 1]")
        exit()


    # l_mont is in the range [0, N - 1]
    # l_square is not reduced so, its range is [0, 2 * N - 1]
    l_square = helper_functions.montgomery_product_thesis(l_mont, l_mont, N, N_prime, R)
    if not(0 <= l_square < R ** 2):
        print("l_square is not in the range [0, 2 * N - 1]")
        exit()
    
    x1_add_x2 = x1_mont + x2_mont
    if not(0 <= x1_add_x2 < R):
        print("x1_add_x2 is not in the range [0, R - 1]")
        exit()

    # x1 + x2 is in the range [0, 2 * N - 1]
    if not(0 <= 2 * N - x1_add_x2 < R):
        print("2 * N - x1_add_x2 is not in the range [0, R - 1]")
        exit()
    x3 = l_square + 2 * N - x1_add_x2
    if not(0 <= x3 < R ** 2):
        print("x3 is not in the range [0, N * N - 1]")
        exit()

    # We add with 4 * N to make sure x1 - x3 is always positive
    if not(0 <= 4 * N - x3 < R):
        print("4 * N - x3 is not in the range [0, R - 1]")
        exit()
    x1_sub_x3 = x1_mont - x3 + 4 * N
    if not(0 <= x1_sub_x3 < R):
        print("x1_sub_x3 is not in the range [0, R - 1]")
        exit()

    l_x1x3 = helper_functions.montgomery_product_thesis(l_mont, x1_sub_x3, N, N_prime, R)
    if not(0 <= l_x1x3 < R ** 2):
        print("l_x1x3 is not in the range [0, R - 1]")
        exit()
    # Add N to make sure N - y1 is always positive
    # y1_mont is in the range [0, N - 1]
    y3 = l_x1x3 + N - y1_mont
    if not(0 <= y3 < R ** 2):
        print("y3 is not in the range [0, R - 1]")
        exit()

    return x3, y3


if __name__ == "__main__":
    if not helper_functions.verify_parameters(bls12_381, bls12_381_modulo_inverse, R):
        print("Parameters are not valid")
        exit()
    
    for iterations in range(100000):
        x1 = random.randint(0, bls12_381)
        y1 = random.randint(0, bls12_381)
        x2 = random.randint(0, bls12_381)
        y2 = random.randint(0, bls12_381)

        if x1 == x2 and y1 == y2:
            l = ((3 * x1**2 + 1) * mod_inverse(2 * y1, bls12_381)) % bls12_381
        else:   
            l = ((y2 - y1) * mod_inverse(x2 - x1, bls12_381)) % bls12_381

        x3 = (l**2 - x1 - x2) % bls12_381
        y3 = (l * (x1 - x3) - y1) % bls12_381
        x3_mont = (x3 * R) % bls12_381
        y3_mont = (y3 * R) % bls12_381

        x1_mont = (x1 * R) % bls12_381
        y1_mont = (y1 * R) % bls12_381
        x2_mont = (x2 * R) % bls12_381
        y2_mont = (y2 * R) % bls12_381
        l_mont = (l * R) % bls12_381

        x3_thesis, y3_thesis = elliptical_curve_addition_thesis_model(x1_mont, y1_mont, x2_mont, y2_mont, l_mont)
        if x3_mont != (x3_thesis % bls12_381) or y3_mont != (y3_thesis % bls12_381):
            print(f"Error in iteration {iterations}")

        x3_thesis_reduced = helper_functions.montgomery_reduction(x3_thesis % bls12_381, bls12_381, bls12_381_modulo_inverse, R)
        y3_thesis_reduced = helper_functions.montgomery_reduction(y3_thesis % bls12_381, bls12_381, bls12_381_modulo_inverse, R)
        if x3_thesis_reduced != x3 or y3_thesis_reduced != y3:
            print(f"Error in iteration {iterations}")



