from sympy import mod_inverse
from math import log2, floor
from sympy import gcd

# Montgomery Modular Multiplication Algorithm
# Why? Avoids modular division by N which is generally a prime and thus expensive

# Let a, b be integers in the range [0, N-1] and N be a positive integer such that gcd(N, R) = 1 and R > N
# N is a number such that gcd(N, R) = 1 and R > N
# R_prime * R = 1 mod N
# R * R_prime - N * N_prime = 1
# Compute c = (a * b) mod N

def montgomery_multiplication(a, b, N, N_prime, R):
    a_mont = (a * R) % N
    b_mont = (b * R) % N
    product_mont = montgomery_product(a_mont, b_mont, N, N_prime, R)
    product = montgomery_reduction(product_mont, N, N_prime, R)
    return product

def montgomery_product(a_mont, b_mont, N, N_prime, R):
    # Original Algorithm
    t = (a_mont * b_mont) % R
    m = (t * N_prime) % R
    u = (a_mont * b_mont + m * N) // R
    if u >= N:
        u = u - N
    return u

def montgomery_reduction(a_mont, N, N_prime, R):
    # Converts montgomery form to normal form
    return montgomery_product(a_mont, 1, N, N_prime, R)

def montgomery_product_thesis(a_mont, b_mont, N, N_prime, R):
    # Algorithm used in thesis project
    t = (a_mont * b_mont)
    m = ((t % R) * N_prime) % R
    u = (t + m * N) >> floor(log2(R)) # equivalent to u = (t + m * N) / R since R is a power of 2

    # No subtracction of N 
    return u

def verify_parameters(N, N_prime, R):
    # Verify that N and R are valid parameters for the montgomery multiplication algorithm
    # N is a number such that gcd(N, R) = 1 and R > N
    # R_prime * R = 1 mod N
    # R * R_prime - N * N_prime = 1

    R_prime = mod_inverse(R, N)
    if R < N:
        return False
    if gcd(N, R) != 1:
        return False
    if R * R_prime - N * N_prime != 1:
        return False

    print(f"Parameters are valid : N = {N}, N_prime = {N_prime}, R = {R}")
    return True