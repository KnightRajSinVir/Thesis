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
p_val = 0x1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe5b9385f0b597d

# your Montgomery constant p' = Σ (p_prime_limbs[i] << (31*i))
p_prime_val = 0x5a14a7c315e0861bde90367ae35113a058a0706d580c03d25ef26bada1e44cc4d6d7014531757c3b0bcc8acc03b9582b


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
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} a_in.txt b_in.txt c_out.txt")
        sys.exit(1)

    a_lines = read_lines(sys.argv[1], 26)
    b_lines = read_lines(sys.argv[2], 26)
    c_lines = read_lines(sys.argv[3], 52)

    LIMBS_IN, LIMBS_OUT, BITS = 13, 26, 31

    a_blocks = [[0] * LIMBS_IN for _ in range(8)]
    b_blocks = [[0] * LIMBS_IN for _ in range(8)]
    for i in range(LIMBS_IN):
        low_a,  high_a  = a_lines[2*i],   a_lines[2*i + 1]
        low_b,  high_b  = b_lines[2*i],   b_lines[2*i + 1]
        for j in range(4):
            a_blocks[j][i]     = low_a[j]
            b_blocks[j][i]     = low_b[j]
            a_blocks[j+4][i]   = high_a[j]
            b_blocks[j+4][i]   = high_b[j]

    c_blocks = [[0] * LIMBS_OUT for _ in range(8)]
    for i in range(LIMBS_OUT):
        low_c, high_c = c_lines[2*i], c_lines[2*i + 1]
        for j in range(4):
            c_blocks[j][i]     = low_c[j]
            c_blocks[j+4][i]   = high_c[j]

    mismatch = False
    for idx, (A, B, C) in enumerate(zip(a_blocks, b_blocks, c_blocks)):
        a_val = reconstruct(A, BITS)
        b_val = reconstruct(B, BITS)
        c_val = reconstruct(C, BITS)

        t      = a_val * b_val
        m = ((t % R) * p_prime_val) % R
        calc_u = (t + m * p_val)
        if calc_u != c_val:
            print(f"Mismatch at idx={idx}: expected {a_val*b_val}, got {c_val}")
            mismatch = True

    if not mismatch:
        print("All 8 products match.")
    else:
        print("Mismatch detected. Generating corrected c_out_fixed.txt…")

    fixed = []
    for A, B in zip(a_blocks, b_blocks):
        p = reconstruct(A, BITS) * reconstruct(B, BITS)
        fixed.append(split_limbs(p, LIMBS_OUT, BITS))
    write_c_out_fixed(fixed)
    print("Written c_out_fixed.txt")

if __name__ == "__main__":
    main()
