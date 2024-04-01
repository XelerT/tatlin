import subprocess
import argparse
import time
import random
import os
from subprocess import Popen, PIPE, STDOUT
from pathlib import Path
import filecmp

class TERMINAL_COLORS:
        PURPLE    = '\033[95m'
        OKBLUE    = '\033[94m'
        OKCYAN    = '\033[96m'
        OKGREEN   = '\033[92m'
        WARNING   = '\033[93m'
        ERROR     = '\033[91m'
        DEFAULT   = '\033[0m'
        BOLD      = '\033[1m'
        UNDERLINE = '\033[4m'

class ERRORS:
        ERROR = -1

data_files_names = []
to_data_path = "/../tests/e2e/data/"

def get_data_files_names():
        dir_path = str(Path.cwd()) + to_data_path
        data_files_counter = 0

        for path in os.listdir(dir_path):
                if os.path.isfile(os.path.join(dir_path, path)):
                        data_files_counter += 1
                        data_files_names.append(os.path.join(dir_path, path))


def check_output_data(n_test, file_name, exec_time, correct_output=str()):
        if correct_output == str():
                try:
                        if filecmp.cmp("output.tape", file_name + "_ans"):
                                print(TERMINAL_COLORS.OKGREEN                    + \
                                        f"Test {n_test:3} Passed. "              + \
                                        f"File {data_files_names[n_test]} "      + \
                                        f"Execution time: {exec_time:.03f} sec"  + \
                                TERMINAL_COLORS.DEFAULT
                                )
                        else:
                                print(TERMINAL_COLORS.ERROR                       + \
                                        f"Test {n_test} NOT Passed. "             + \
                                        f"File {data_files_names[n_test]}\n"      + \
                                        f"Execution time: {exec_time:.03f} sec\n" + \
                                TERMINAL_COLORS.DEFAULT
                                )
                except:
                        print(TERMINAL_COLORS.WARNING                                 + \
                                f"Test {n_test} has fallen."                          + \
                        TERMINAL_COLORS.DEFAULT
                        )
        else:
                try:
                        print(TERMINAL_COLORS.OKBLUE                      + \
                                f"Test {n_test} Runned. "                 + \
                                f"File {data_files_names[n_test]}\n"      + \
                                f"Execution time: {exec_time:.03f} sec\n" + \
                        TERMINAL_COLORS.DEFAULT
                        )
                except:
                        print(TERMINAL_COLORS.WARNING                                 + \
                                f"Test {n_test} has fallen."                          + \
                        TERMINAL_COLORS.DEFAULT
                        )


def get_correct_output (n_triangles, data):
        correct_output = data[n_triangles*3*3:]
        return correct_output


def parse_data_file (file_name):
        with open(file_name) as file:
                try:
                        n_nodes = int(next(file))
                        return n_nodes
                except:
                        print(TERMINAL_COLORS.WARNING            + \
                                f"File {file_name} is broken.\n" + \
                                TERMINAL_COLORS.DEFAULT
                             )                        

def run_e2e_test(app2run, file_name):
        print(TERMINAL_COLORS.OKCYAN                    + \
                        f"Run {app2run}."               + \
                TERMINAL_COLORS.DEFAULT
                )
        pipe = Popen([app2run, file_name], stdout=PIPE, stdin=PIPE)
        
        start_time  = time.time()
        stdout_data = pipe.communicate()
        exec_time   = time.time() - start_time

        return stdout_data[0].decode(), exec_time
        

def check_app_output(n_test, file_name, exec_time, correct_output=str()):
        check_output_data(n_test, file_name, exec_time)


def run_e2e_tests(app_name, app2_name=str()):
        for (n_test, file_name) in zip(range(len(data_files_names)), data_files_names):
                if (file_name[-4:] != "_ans"):
                        output_data, exec_time = run_e2e_test(app_name, file_name)
                        check_app_output(n_test, file_name, exec_time)  
                if os.path.exists("output.tape"):
                        os.remove("output.tape")

if __name__ == "__main__":
        get_data_files_names()
        run_e2e_tests("./tatlin_tape")
