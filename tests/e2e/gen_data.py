import time
import random
import sys

def gen_data(output_file_name, n_elems, max_elem):
        file = open(output_file_name, "wb")
        for i in range(n_elems):
                file.write((random.randint(0, max_elem)).to_bytes(4, byteorder='little', signed=False))

if __name__ == "__main__":
        gen_data(sys.argv[1], int(sys.argv[2]), 100000)