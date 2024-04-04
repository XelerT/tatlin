#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <filesystem>

#include "tape/itape.hpp"
#include "tape/tapes.hpp"

#include "utils/minheap.hpp"
#include "sorter/sorter.hpp"

namespace tatlin_tape 
{

template <typename T>
class manager_t
{
        public:
                using uptr_itape_t  = std::unique_ptr<itape_t<T>>;
                using shptr_itape_t = std::shared_ptr<itape_t<T>>;

        private:
                std::string input_file_path  {};
                std::string output_file_path {};

                tape_cnfg_t config {};
                min_heap_t<T> min_heap {};

                std::vector<tape_t<T>> tmp_tapes {};
                bool delete_tmps = false;

        public:
                manager_t (std::string &input_file_path_, 
                           std::string &output_file_path_, tape_cnfg_t &config_,
                           bool delete_tmps_ = false):
                        input_file_path(input_file_path_), 
                        output_file_path(output_file_path_), 
                        config(config_), delete_tmps(delete_tmps_) {}
                manager_t (std::string &&input_file_path_, 
                           std::string &&output_file_path_, tape_cnfg_t &config_,
                           bool delete_tmps_ = false):
                        input_file_path(input_file_path_), 
                        output_file_path(output_file_path_),
                        config(config_), delete_tmps(delete_tmps_) {}

                void copy_sort_tape ();

        private:
                std::unique_ptr<T[]> create_chunks ();
                void create_tmp_tapes (size_t n_remaining_bytes, size_t n_chunks, 
                                       size_t chunk_size);

                void delete_tmp_dir () { std::filesystem::remove_all("tmp/"); }
};

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

template <typename T>
inline void manager_t<T>::copy_sort_tape ()
{
        uptr_itape_t input_tape {create_tape<T>(config, input_file_path)};
        tape_sorter_t<T> sorter {input_tape->get_size()};

        size_t n_chunks = sorter.get_n_chunks();
        if (n_chunks != 1)
                create_tmp_tapes(input_tape->get_size(), n_chunks, sorter.get_chunk_size());
        
        uptr_itape_t output_tape {create_tape<T>(config, output_file_path)};
        sorter.sort(*input_tape, *output_tape, tmp_tapes.begin());
        
        if (delete_tmps)
                delete_tmp_dir();
}

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

template <typename T>
inline void manager_t<T>::create_tmp_tapes 
(size_t n_remaining_bytes, size_t n_chunks, size_t chunk_size)
{
        std::filesystem::create_directory("tmp/");
        
        for (size_t i = 0; i < n_chunks - 1; i++) {                
                std::string tmp_tape_name = "tmp/" + std::to_string(i) + ".tape";
                tmp_tapes.push_back(tape_t<T> {config, tmp_tape_name, chunk_size, true});
                n_remaining_bytes -= chunk_size;
        }
        std::string tmp_tape_name = "tmp/" + std::to_string(n_chunks - 1) + ".tape";
        tmp_tapes.push_back(tape_t<T> {config, tmp_tape_name, n_remaining_bytes, true});
}

}
