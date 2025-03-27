import random
import os

SIMD_SIZE = 8
BANK_SIZE = 128
NOF_ROUNDS = 2

def file_writer(fname, data):
    with open(fname, 'w') as fp: 
        for idx, item in enumerate(data):
            fp.write(str(item)+ ' ')
            if idx % 4 == 3:
                fp.write('\n')

a_in = []
b_in = []
c_out = []

for i in range(BANK_SIZE * SIMD_SIZE * NOF_ROUNDS):
    a = random.randint(0, 2**30-1)
    b = random.randint(0, 2**30-1)
    c = a + b
    a_in.append(a)
    b_in.append(b)
    c_out.append(c)

curr_dir = os.path.dirname(os.path.abspath(__file__))
file_writer(os.path.join(curr_dir, "a_in.txt"), a_in)
file_writer(os.path.join(curr_dir, "b_in.txt"), b_in)
file_writer(os.path.join(curr_dir, "c_out.txt"), c_out)