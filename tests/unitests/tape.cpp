#include "config.hpp"

using namespace std;
using namespace tatlin_tape;

TEST (tape, init_move_ctor)
{
        tape_cnfg_t config {0, 0, 0, 0};
        std::string name = "filename.tape";
        std::vector<int> data {0, 1, 2};
        std::vector<int> data2(3);

        tape_t<int> tape {config, name};
        tape.write_next(data.data(), data.size());

        tape_t<int> tape2 {std::move(tape)};
        tape2.rewind();
        tape2.read_next(data2.data(), data.size());

        ASSERT_TRUE(data == data2);
}

TEST (tape, init_move_assign)
{
        tape_cnfg_t config {0, 0, 0, 0};
        std::string name = "filename.tape";
        std::vector<int> data {0, 1, 2};
        std::vector<int> data2(3);

        tape_t<int> tape {config, name};
        tape.write_next(data.data(), data.size());

        tape_t<int> tape2 = std::move(tape);
        tape2.rewind();
        tape2.read_next(data2.data(), data.size());

        ASSERT_TRUE(data == data2);
}
