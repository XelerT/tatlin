import time
import random
import sys
import bisect
import array

def gen_data(output_file_name, n_elems, max_elem):
        file = open(output_file_name, "wb")
        for i in range(n_elems):
                file.write((random.randint(0, max_elem)).to_bytes(4, byteorder='little', signed=False))

def gen_data_and_ans(output_file_name, n_elems, max_elem):
        data = array.array('i')
        ans = array.array('i')

        for i in range(n_elems):
                elem = random.randint(0, max_elem)
                data.append(elem)
                bisect.insort(ans, elem) 
                
        file_dat = open(output_file_name, "wb")
        file_ans = open(output_file_name + "_ans", "wb")

        file_dat.write((data).tobytes())
        file_ans.write((ans).tobytes())

if __name__ == "__main__":
        gen_data_and_ans(sys.argv[1], int(sys.argv[2]), 10)