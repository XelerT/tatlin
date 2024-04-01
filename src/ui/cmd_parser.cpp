#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "cmd_parser.hpp"

namespace po = boost::program_options;

arguments_t parse_cmd_args (int n_args, const char **args)
{
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h",   "displays help info and exit")
                ("clear", po::value<bool>(), "deletes temporary tapes (default value: false)")
                ("conf",  po::value<std::string>(), "sets config file path (default value: ./conf.txt)")
                ;

        po::variables_map var_map {};
        po::store(po::parse_command_line(n_args, args, desc), var_map);
        po::notify(var_map);

        arguments_t parsed_args {};

        if (var_map.count("help")) { 
                std::cout << desc << std::endl; 
                parsed_args.parsed = false; 
                
                return parsed_args; 
        }

        if (var_map.count("conf"))  { parsed_args.conf_path = var_map["conf"].as<std::string>(); }
        if (var_map.count("clear")) { parsed_args.clear     = var_map["clear"].as<bool>(); }

        return parsed_args;
}