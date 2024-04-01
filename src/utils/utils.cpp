#include <filesystem> 
#include <fstream>
#include <iostream>
#include "utils.hpp"

using namespace tatlin_tape;    
using namespace std::filesystem;

tape_cnfg_t read_config (const std::string &conf_path)
{
        if (!std::filesystem::exists(conf_path))
                throw std::runtime_error("No such config file.");

        std::fstream config_file {};
        config_file.open(conf_path, std::fstream::in);

        float read_latency  {};
        float write_latency {};
        float rewind_dur    {};
        float shift_latency {};

        std::string field {};
        while (config_file >> field) {
                if (field == "read_latency")
                        config_file >> read_latency;
                else if (field == "write_latency")
                        config_file >> write_latency;
                else if (field == "rewind_dur")
                        config_file >> rewind_dur;
                else if (field == "shift_latency")
                        config_file >> shift_latency;
        }

        if (config_file.fail() && !config_file.eof())
                throw std::runtime_error("Something wrong with config file.");

        config_file.close();

        return tape_cnfg_t {read_latency, write_latency,
                            rewind_dur, shift_latency};
}
