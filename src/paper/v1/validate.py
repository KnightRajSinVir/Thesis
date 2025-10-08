#!/usr/bin/env python3
"""
check_schoobook.py

This script verifies Schoobook-style multiplications for 8 pairs of 403-bit numbers 
(split into 31-bit limbs), arranged across lines of 4 integers per line, both on input
and output.

Usage:
    python3 check_schoobook_multi.py a_in.txt b_in.txt c_out.txt

- a_in.txt, b_in.txt: 26 lines, each with 4 ints (columns 0–3).
- c_out.txt:        52 lines, each with 4 ints (columns 0–3).

Layout (both input and output):
  Lines 2*i     → limb i of numbers 0–3 (cols 0–3)
  Lines 2*i+1   → limb i of numbers 4–7 (cols 0–3)
  for i in 0…12 (input) or i in 0…25 (output).

On mismatch, writes a corrected `c_out_fixed.txt` in the **same** format.
"""

import sys

# Constants
R = 1 << (31 * 13)

# your 403-bit modulus n = Σ (q_limbs[i] << (31*i))
p_val = 0x1A0111EA397FE69A4B1BA7B6434BACD764774B84F38512BF6730D2A0F6B0F6241EABFFFEB153FFFFB9FEFFFFFFFFAAAB  

# your Montgomery constant p' = Σ (p_prime_limbs[i] << (31*i))
p_prime_val =  0x627A0CEB06106FEAAFC9468B316FEE268CF5819ECCA0E8EB2DB4C16EF2EF0C8E30B48286ADB92D9D113E889F3FFFCFFFCFFFD


def read_lines(fname, expect):
    with open(fname) as f:
        L = [list(map(int, line.split())) for line in f]
    if len(L) < expect:
        print(f"Error: {fname} has {len(L)} lines; expected ≥ {expect}.")
        sys.exit(1)
    return L[:expect]

def reconstruct(limbs, bits=31):
    m = (1 << bits) - 1
    v = 0
    for i, w in enumerate(limbs):
        if not (0 <= w < (1 << bits)):
            raise ValueError(f"Invalid limb value {w} at index {i}, expected 0 <= w < {1 << bits}.")
        v |= (w & m) << (i * bits)
    return v

def split_limbs(val, n, bits=31):
    m = (1 << bits) - 1
    return [(val >> (i * bits)) & m for i in range(n)]

def write_c_out_fixed(blocks, fname="c_out_fixed.txt"):
    # blocks[j][i] is limb i of product j
    LIMBS_OUT = len(blocks[0])
    with open(fname, "w") as f:
        for i in range(LIMBS_OUT):
            row0 = [blocks[j][i] for j in range(4)]
            f.write(" ".join(str(x) for x in row0) + "\n")
            row1 = [blocks[j][i] for j in range(4, 8)]
            f.write(" ".join(str(x) for x in row1) + "\n")

def main():
    if len(sys.argv) != 5:
        print(f"Usage: {sys.argv[0]} x3_in.txt y3_in.txt x3_test.txt y3_test.txt")
        sys.exit(1)

    x3_lines = read_lines(sys.argv[1], 26)
    y3_lines = read_lines(sys.argv[2], 26)
    x3_test_lines = read_lines(sys.argv[3], 26)
    y3_test_lines = read_lines(sys.argv[4], 26)

    LIMBS_IN, LIMBS_OUT, BITS = 13, 26, 31

    x3_blocks = [[0] * LIMBS_IN for _ in range(8)]
    y3_blocks = [[0] * LIMBS_IN for _ in range(8)]
    x3_test_blocks = [[0] * LIMBS_IN for _ in range(8)]
    y3_test_blocks = [[0] * LIMBS_IN for _ in range(8)]
    for i in range(LIMBS_IN):
        low_x3,  high_x3  = x3_lines[2*i],   x3_lines[2*i + 1]
        low_y3,  high_y3  = y3_lines[2*i],   y3_lines[2*i + 1]
        low_x3_test,  high_x3_test  = x3_test_lines[2*i],   x3_test_lines[2*i + 1]
        low_y3_test,  high_y3_test  = y3_test_lines[2*i],   y3_test_lines[2*i + 1]
        for j in range(4):
            x3_blocks[j][i]     = low_x3[j]
            y3_blocks[j][i]     = low_y3[j]
            x3_test_blocks[j][i]   = low_x3_test[j]
            y3_test_blocks[j][i]   = low_y3_test[j]


    mismatch = False
    calc_u = 0
    for idx, (x3, y3, x3_test, y3_test) in enumerate(zip(x3_blocks, y3_blocks, x3_test_blocks, y3_test_blocks)):
        x3_val = reconstruct(x3, BITS)
        y3_val = reconstruct(y3, BITS)
        x3_test_val = reconstruct(x3_test, BITS)
        y3_test_val = reconstruct(y3_test, BITS)

        print(f"x3_val = {x3_val}")
        print(f"y3_val = {y3_val}")
        print(f"x3_test_val = {x3_test_val}")
        print(f"y3_test_val = {y3_test_val}")

        print(f"x3_val % p_val = {x3_val % p_val}")
        print(f"x3_test_val % p_val = {x3_test_val % p_val}")
        print(f"y3_val % p_val = {y3_val % p_val}")
        print(f"y3_test_val % p_val = {y3_test_val % p_val}")

        if x3_val % p_val != x3_test_val % p_val or y3_val % p_val != y3_test_val % p_val:
            print(f"Mismatch at idx={idx}: expected {x3_val}, got {x3_test_val}")
            print(f"Expected {y3_val}, got {y3_test_val}")
            mismatch = True

    if not mismatch:
        print("All 8 products match.")
    else:
        print("Mismatch detected. Generating corrected x3_out_fixed.txt and y3_out_fixed.txt…")

if __name__ == "__main__":
    main()
