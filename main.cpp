#include "drive.hpp"

using namespace tatlin_tape;

int main () 
{
        tape_cnfg_t tape_conf {};
        drive_t<int> drive {"../tests/e2e/data/4.tape", "output.tape", tape_conf};
        drive.copy_sort_tape();

        return 0;
}
