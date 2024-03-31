#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <filesystem>

#include "tape/itape.hpp"
#include "tape/tapes.hpp"

#include "utils/minheap.hpp"

namespace tatlin_tape 
{

template <typename T>
class drive_t
{
        public:
                using uptr_itape_t  = std::unique_ptr<itape_t<T>>;
                using shptr_itape_t = std::shared_ptr<itape_t<T>>;

        private:
                std::string input_file_path  {};
                std::string output_file_path {};

                tape_cnfg_t config {};
                min_heap_t<T> min_heap {};

                size_t chunk_size = 0;
                size_t n_chunks   = 0;
                size_t tape_size  = 0;

                size_t tape_n_elems = 0;
                size_t chunk_n_elems = 0;

                std::vector<shptr_itape_t> tmp_tapes {};

        public:
                drive_t (std::string &input_file_path_, 
                        std::string &output_file_path_, tape_cnfg_t &config_):
                        input_file_path(input_file_path_), 
                        output_file_path(output_file_path_), config(config_) {}
                drive_t (std::string &&input_file_path_, 
                         std::string &&output_file_path_, tape_cnfg_t &config_):
                        input_file_path(input_file_path_), 
                        output_file_path(output_file_path_), config(config_) {}

                void copy_sort_tape ();

        private:
                void merge_tmp_tapes_in_output ();
                std::unique_ptr<T[]> create_chunks ();
                void create_tmp_tapes (uptr_itape_t &input_tape, 
                                       std::unique_ptr<T[]> &tape_data_chunks);
                void write_output_tape_no_tmps 
                (uptr_itape_t &input_tape, std::unique_ptr<T[]> &tape_data_chunks);
};

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

template <typename T>
inline void drive_t<T>::copy_sort_tape ()
{
        uptr_itape_t input_tape {create_tape<T>(config, input_file_path)};
        tape_size = input_tape->get_size();
        chunk_size = tape_size;

        auto tape_data_chunks = create_chunks();

        if (n_chunks != 1) {
                create_tmp_tapes(input_tape, tape_data_chunks);
                merge_tmp_tapes_in_output();
        } else {
                write_output_tape_no_tmps(input_tape, tape_data_chunks);
        }
}

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

template <typename T>
inline void drive_t<T>::merge_tmp_tapes_in_output ()
{
        uptr_itape_t output {create_tape<T>(config, output_file_path)};

        for (size_t i = 0; i < tape_n_elems; i++) {
                min_heap_node_t root = min_heap.get_min();
                output->write_next(root.elem);

                if (root.next_elem_index < chunk_n_elems) {
                        tmp_tapes[root.arr_index]->read_next(&root.elem);
                        root.next_elem_index++;
                } else {
                        root.elem = INT_MAX;
                }
                min_heap.replace_min(root);
        }
}

template <typename T>
inline std::unique_ptr<T[]> drive_t<T>::create_chunks ()
{
        std::unique_ptr<T[]> tape_data_chunks { new(std::nothrow) T[chunk_size] };
        while (!tape_data_chunks && chunk_size) {
                chunk_size /= 2;
                tape_data_chunks.reset(new(std::nothrow) T[chunk_size]);
        }

        // std::cout << tape_size << " / " << chunk_size << std::endl; 
        if (!chunk_size)
                throw std::bad_alloc();

        n_chunks = tape_size / chunk_size;
        if (tape_size > chunk_size * n_chunks)
                ++n_chunks;
                        
        tape_n_elems  = tape_size / sizeof(T);
        chunk_n_elems = chunk_size / sizeof(T);

        return tape_data_chunks;
}

template <typename T>
inline void drive_t<T>::create_tmp_tapes 
(uptr_itape_t &input_tape, std::unique_ptr<T[]> &tape_data_chunks)
{
        std::filesystem::create_directory("tmp/");
        for (size_t i = 0; i < n_chunks; i++) {
                input_tape->read_next(tape_data_chunks.get(), chunk_n_elems);

                std::sort(tape_data_chunks.get(), tape_data_chunks.get() + chunk_n_elems);
                min_heap.push(min_heap_node_t {tape_data_chunks[0], i, 1});

                std::string tmp_tape_name = "tmp/" + std::to_string(i) + ".tape";
                shptr_itape_t tmp {create_tape<T>(config, tmp_tape_name, true)};

                tmp->write(0, tape_data_chunks.get(), chunk_n_elems);
                tmp_tapes.push_back(tmp);
        }
        min_heap.initial_heapify();
}

template <typename T>
inline void drive_t<T>::write_output_tape_no_tmps 
(uptr_itape_t &input_tape, std::unique_ptr<T[]> &tape_data_chunks)
{
        input_tape->read_next(tape_data_chunks.get(), chunk_n_elems);

        std::sort(tape_data_chunks.get(), tape_data_chunks.get() + chunk_n_elems);

        uptr_itape_t output {create_tape<T>(config, output_file_path)};
        output->write(0, tape_data_chunks.get(), chunk_n_elems);
}

}
