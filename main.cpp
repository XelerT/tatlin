#include "drive.hpp"

using namespace tatlin_tape;

int main () 
{
        tape_cnfg_t tape_conf {};
        drive_t<int> drive {"input.tape", "output.tape", tape_conf};
        drive.copy_sort_tape();

        return 0;
}
