import random
import os
from constants import bls12_381, bls12_381_modulo_inverse, R
from helper_functions import mod_inverse, montgomery_multiplication 

# Parameters for the graph
SIMD_SIZE = 8
BANK_SIZE = 16
NOF_ROUNDS = 16

def file_writer(fname, data):
    with open(fname, 'w') as fp: 
        for idx, item in enumerate(data):
            fp.write(str(item)+ ' ')
            if idx % 4 == 3:
                fp.write('\n')


x1 = random.randint(0, bls12_381)
y1 = random.randint(0, bls12_381)
x2 = random.randint(0, bls12_381)
y2 = random.randint(0, bls12_381)

if x1 == x2:
    x2 = x2 + 1 % bls12_381

if y1 == y2:
    y2 = y2 + 1 % bls12_381

y2_sub_y1 = (y2 - y1) % bls12_381
x2_sub_x1 = (x2 - x1) % bls12_381
x2_sub_x1_inv = mod_inverse(x2_sub_x1, bls12_381)


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

y2_sub_y1_mont = (y2_sub_y1 * R) % bls12_381
x2_sub_x1_inv_mont = (x2_sub_x1_inv * R) % bls12_381

print(l == (montgomery_multiplication(y2_sub_y1, x2_sub_x1_inv, bls12_381, bls12_381_modulo_inverse, R)))

x1_in = []
y1_in = []
x2_in = []
y2_in = []
y2_sub_y1_in = []
x2_sub_x1_inv_in = []

x3_output = []
y3_output = []

# Split into 31 bit limbs and write to file
for iteration in range(NOF_ROUNDS):
    for j in range(BANK_SIZE):
        if j < 13:
            for i in range(8):
                x1_in.append(x1_mont >> (31 * j) & 0x7FFFFFFF)
                y1_in.append(y1_mont >> (31 * j) & 0x7FFFFFFF)
                x2_in.append(x2_mont >> (31 * j) & 0x7FFFFFFF)
                y2_in.append(y2_mont >> (31 * j) & 0x7FFFFFFF)
                y2_sub_y1_in.append(y2_sub_y1_mont >> (31 * j) & 0x7FFFFFFF)
                x2_sub_x1_inv_in.append(x2_sub_x1_inv_mont >> (31 * j) & 0x7FFFFFFF)
                x3_output.append(x3_mont >> (31 * j) & 0x7FFFFFFF)
                y3_output.append(y3_mont >> (31 * j) & 0x7FFFFFFF)
        else:
            for i in range(8):
                x1_in.append(0)
                y1_in.append(0)
                x2_in.append(0)
                y2_in.append(0)
                y2_sub_y1_in.append(0)
                x2_sub_x1_inv_in.append(0)
                x3_output.append(0)
                y3_output.append(0)


curr_dir = os.path.dirname(os.path.abspath(__file__))
file_writer(os.path.join(curr_dir, "x1_in.txt"), x1_in)
file_writer(os.path.join(curr_dir, "y1_in.txt"), y1_in)
file_writer(os.path.join(curr_dir, "x2_in.txt"), x2_in)
file_writer(os.path.join(curr_dir, "y2_in.txt"), y2_in)
file_writer(os.path.join(curr_dir, "y2_sub_y1_in.txt"), y2_sub_y1_in)
file_writer(os.path.join(curr_dir, "x2_sub_x1_inv_in.txt"), x2_sub_x1_inv_in)

file_writer(os.path.join(curr_dir, "x3_output.txt"), x3_output)
file_writer(os.path.join(curr_dir, "y3_output.txt"), y3_output)