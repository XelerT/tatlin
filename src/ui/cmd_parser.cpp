#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "cmd_parser.hpp"

namespace po = boost::program_options;

arguments_t parse_cmd_args (int n_args, const char **args)
{
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "displays help info and exits")
                ("clear",  "deletes temporary tapes")
                ("conf",  po::value<std::string>(), "sets config file path (default value: ./conf.txt)")
                ("output-file,o", po::value<std::string>(), "sets output file name (default value: ./output.tape)")
                ("input-file", po::value<std::string> (), "sets input file path")
                ;

        po::positional_options_description pos_opts;
        pos_opts.add("input-file", -1);

        po::variables_map var_map {};
        arguments_t parsed_args {};
        try {
                po::store(po::command_line_parser(n_args, args).
                          options(desc).positional(pos_opts).run(), var_map);
        } catch (boost::wrapexcept<boost::program_options::unknown_option> &unknown) {
                std::cerr << unknown.what();
                parsed_args.parsed = false;                 
                return parsed_args; 
        } catch (boost::wrapexcept<boost::program_options::multiple_occurrences> &mul_occ) {
                std::cerr << mul_occ.what();
                parsed_args.parsed = false;                 
                return parsed_args; 
        }
        po::notify(var_map);

        if (var_map.count("help")) { 
                std::cout << desc << std::endl; 
                parsed_args.parsed = false; 
                
                return parsed_args; 
        }

        if (var_map.count("conf"))  { parsed_args.conf_path = var_map["conf"].as<std::string>(); }
        if (var_map.count("clear")) { parsed_args.clear     = true; }
        if (var_map.count("input-file"))  { parsed_args.input_file_path  = var_map["input-file"].as<std::string>(); }
        if (var_map.count("output-file")) { parsed_args.output_file_path = var_map["output-file"].as<std::string>(); }

        return parsed_args;
}