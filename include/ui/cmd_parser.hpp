#pragma once

#include <string>

#include "utils/utils.hpp"

struct arguments_t
{
        bool parsed = true;
        bool clear  = false;
        std::string conf_path = "./conf.txt";
        std::string input_file_path  {};
        std::string output_file_path = "output.tape";
};

arguments_t parse_cmd_args (int n_args, const char **args);