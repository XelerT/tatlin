#include "drive.hpp"
#include "utils.hpp"
#include "cmd_parser.hpp"

using namespace tatlin_tape;

int main (int argc, const char *argv[]) 
{
        arguments_t parsed_args = parse_cmd_args(argc, argv);
        if (!parsed_args.parsed)
                return 0;

        try {
                tape_cnfg_t tape_conf = read_config(parsed_args.conf_path);
                std::cout << parsed_args.input_file_path << " " << parsed_args.output_file_path<<std::endl;
                drive_t<int> drive {parsed_args.input_file_path, parsed_args.output_file_path, tape_conf};
                drive.copy_sort_tape();
        } catch (std::bad_alloc &ba) {
                std::cerr << "Can not allocate needed memory.\n"; 
                return -1;
        } catch (std::runtime_error &re) {
                std::cerr << re.what();
                return -2;
        } catch (...) {
                std::cerr << "Unknown error.\n";
                return -3;
        }

        return 0;
}
