#include "drive.hpp"

using namespace tatlin_tape;

int main () 
{
        // arguments_t parsed_args = parse_cmd_args(argc, argv);
        // tape_cnfg_t tape_conf = read_config(parsed_args.conf_path);
        tape_cnfg_t tape_conf = {};
        drive_t<int> drive {"../tests/e2e/data/3.tape", "output.tape", tape_conf};
        drive.copy_sort_tape();

        return 0;
}
