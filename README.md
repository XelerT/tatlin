# Tatlin Tape

External sorting project, simulating work of magnet tapes using files. App can sort large file (2+ Gb), using dynamic memory that can be allocated for this program. If file that have to be sorted is larger than allocated memory, temporary tapes will be used and than they will be merged in output file, using min-heap algorithm.

## Prerequisite:

- Cmake 3.13.4
- Boost 1.40


## Install & Build

        $ git clone https://github.com/XelerT/tatlin.git
        $ cd tatlin
        $ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

To build unit tests use:

        $ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUNITESTS=ON && cmake --build build

## Run

Before running tatlin_tape you need to create input file with some integer data (4 bytes) that you will feed to the program. You can generate some data using my python script (more info later). Also you need to create config file with for fields (read_latency, write_latency, rewind_dur, shift_latency).

Example:

        $ ./tatlin_tape ../tests/e2e/data/02.tape --conf config.txt --output output.tape

Input file path is required as positional argument, other arguments can use default paths. Existing options:

        -h [ --help ]            displays help info and exits
        --clear arg              deletes temporary tapes (default value: false)
        --conf arg               sets config file path (default value: ./conf.txt)
        -o [ --output-file ] arg sets output file name (default value: ./output.tape)
        --input-file arg         sets input file path

Example of config file, ```all latencies sets in ms```:

        read_latency  0
        write_latency 0
        rewind_dur    0
        shift_latency 0


## Tests
Required programs:

- Python 3
- Google tests

### Unit tests

Build and run them:

        $ cmake -S .. -B build -DCMAKE_BUILD_TYPE=Release -DUNITESTS=ON && cmake --build build
        $ cd build
        $ ./unitests


### Data generator

You can use my generator to create some random files with 4 byte integers. From ```tatlin/tests/e2e```, set output file name and number of integers to generate:

        $ python gen_data.py input.tape 16777216

### End to end

In directory ``tests/e2e/data`` must be files with data and answer data. Then you can run from ``build/`` directory e2e tests:

        $ python ../tests/e2e/e2e_tests.py

### Test results

My results for sorting without using temporary tapes are represented on this plot, that situation is possible when application can allocate enough memory to store input tape:

![plot](imgs/gr.png)

Otherwise application have to use temporary tapes, which represented by ordinary files, therefore I have significant decrease in run time because of merging process, which uses all temporary files and min-heap in a time. Results for **1 Gb** file with two temporary tapes:

        552 seconds time elapsed

        257 seconds user time
        290 seconds sys time

In comparison with same file to sort, but without temporary tapes (Results: 30s - wall time, 28s - user time, 1s - system time) we have decrease in 18.4 times.

**2 Gb** file stats:

        1238 seconds time elapsed

        567 seconds user time
        658 seconds sys time