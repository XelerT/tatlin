#pragma once

#include <string>

#include "tape/itape.hpp"

struct CONSOLE_CLRS
{
        static constexpr std::string_view RED     = "\033[91m";
        static constexpr std::string_view GREEN   = "\033[32m";
        static constexpr std::string_view BLUE    = "\033[34m";
        static constexpr std::string_view YELLOW  = "\033[33m";
        static constexpr std::string_view DEFAULT = "\033[0m";
};

tatlin_tape::tape_cnfg_t read_config (const std::string &conf_path);