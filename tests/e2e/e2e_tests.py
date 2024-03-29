import subprocess
import argparse
import os
import sys
from   subprocess import Popen, PIPE, STDOUT

# -----------------------------------------------------------------------------------------

class TERMINAL_COLORS:
        PURPLE    = '\033[95m'
        OKBLUE    = '\033[94m'
        OKCYAN    = '\033[96m'
        OKGREEN   = '\033[92m'
        YELLOW    = '\033[33m'
        BLUE      = '\033[36m'
        GREEN     = '\033[32m'

        WARNING   = '\033[93m'
        ERROR     = '\033[91m'
        RED       = '\033[91m'
        DEFAULT   = '\033[0m'
        BOLD      = '\033[1m'
        UNDERLINE = '\033[4m'


data_files_names = []
ans_files_names  = []

path2dir = os.path.dirname(os.path.abspath(__file__))
path2dir_len = len(path2dir)

def get_files_names(files_names, last_dir=str()):
        dir_path = path2dir + last_dir
        data_files_counter = 0

        for path in os.listdir(dir_path):
                if os.path.isfile(os.path.join(dir_path, path)):
                        data_files_counter += 1
                        files_names.append(os.path.join(dir_path, path))

# -----------------------------------------------------------------------------------------

def check_output_data(n_test, stdout_data, correct_res, tested_prog_name, test_file_name):
    try:
        if stdout_data == correct_res:
            print(TERMINAL_COLORS.GREEN                                                          + \
                f"Test {n_test} PASSED (filename: ...{data_files_names[n_test][path2dir_len:]})" + \
                # f"File {data_files_names[n_test]} " + \
            TERMINAL_COLORS.DEFAULT
            )
            return True
        else:
            print(TERMINAL_COLORS.ERROR              + \
                f"Test {n_test} IS FAILED:\n"        + \
                f"Prog output   = {stdout_data}\n"   + \
                f"Expected      = {correct_res}\n"   + \
                f"File:         = {test_file_name}"  + \
            TERMINAL_COLORS.DEFAULT
            )
            return False
    except:
        print(TERMINAL_COLORS.WARNING                   + \
            f"Test {n_test} fall. Prog output:\n"       + \
            stdout_data + "'"                           + \
        TERMINAL_COLORS.DEFAULT
        )
        return False


def show_total_test_stat(n_test, passed_test, failed):
    print('===========TOTAL==============')
    print(TERMINAL_COLORS.OKBLUE                         + \
            f"Total num of tests: {n_test + 1}\n"        + \
            f"Num of passed:      {passed_test}"         + \
    TERMINAL_COLORS.DEFAULT
    )
    if (len(failed)):
        print('FAILED:')
        for failed_test in failed:
            print (TERMINAL_COLORS.ERROR    + \
                   failed_test, "\n"        + \
                   TERMINAL_COLORS.DEFAULT )
    print('==============================')

# -----------------------------------------------------------------------------------------

def parse_test_data(test_case):
    with open(test_case) as dat_file:
        correct_res = 0
        res_is_reached = False
        data = ""
        for item in dat_file:
            data = item

    start = data.find('(')
    end   = data.find(')')

    try:
        correct_res = data[start + 1:end]
        data = data[start + 1:end]
    except:
        correct_res = 0

    correct_numbers = ''.join(c if c.isdigit() else ' ' for c in correct_res).split()
    return correct_numbers

# -----------------------------------------------------------------------------------------

def run_test(tested_prog_name, test_case):
    pipe = Popen([tested_prog_name, test_case], stdout = PIPE)

    stdout_data = pipe.communicate()
    string_data = stdout_data[0].decode()

    # converted_output = string_data.split()
    # converted_output = [int(n) for n in converted_output]

    # return converted_output
    return string_data


def get_data(file_name):
    try:
        with open(file_name) as file:
            return "\n".join([s for s in file.read().split("\n") if s])
    except:
        print(TERMINAL_COLORS.WARNING            + \
                f"File {file_name} is broken.\n" + \
            TERMINAL_COLORS.DEFAULT
             )
        return None 


def run_test_data(tested_prog_name):
    passed_test = 0
    n_test      = 0
    failed = []
    
    print('————————————————————————START OF TESTS———————————————————————')

    for (n_test, file_name, ans_file_name) in zip(range(len(data_files_names)), data_files_names, ans_files_names):
        # data = get_data(file_name)
        ans = get_data(ans_file_name)
        if (ans == "DO NOT RUN"):
            continue

        prog_output = run_test(tested_prog_name, file_name)
        prog_output = "\n".join([s for s in prog_output.split("\n") if s])
        if (check_output_data(n_test, prog_output, ans, tested_prog_name, file_name)):
            passed_test += 1
        else:
            failed.append(file_name)

    print('————————————————————————END OF TESTS—————————————————————————\n\n')
    show_total_test_stat(n_test, passed_test, failed)


# -----------------------------------------------------------------------------------------

if __name__ == "__main__":
    get_files_names(data_files_names, "/my_test_dat/")
    get_files_names(ans_files_names, "/my_test_ans/")

    run_test_data("./paracl")
