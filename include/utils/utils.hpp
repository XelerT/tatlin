#pragma once

#include <string>

struct CONSOLE_CLRS
{
        static constexpr std::string_view RED     = "\033[91m";
        static constexpr std::string_view GREEN   = "\033[32m";
        static constexpr std::string_view BLUE    = "\033[34m";
        static constexpr std::string_view YELLOW  = "\033[33m";
        static constexpr std::string_view DEFAULT = "\033[0m";
};

template <typename Typeid, typename T>
bool isa (T &lhs)
{
        return typeid(lhs) == typeid(Typeid);
}