#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <filesystem>

#include "tape/itape.hpp"
#include "tape/tapes.hpp"

namespace tatlin_tape 
{

template <typename T>
struct min_heap_node_t
{
        T elem {};
        size_t arr_index = 0;
        size_t next_elem_index = 0;
};

template <typename T>
class min_heap_t
{
        private:
                std::vector<min_heap_node_t<T>> nodes {};

        public:
                min_heap_t () = default;
                min_heap_t (std::vector<min_heap_node_t<T>> &&nodes_):
                        nodes(nodes_)
                {
                        initial_heapify();        
                }

                void initial_heapify ()
                { 
                        for (size_t i = (nodes.size() - 1) / 2; i >= 0; --i)
                                heapify(i);
                }

                size_t get_left_child_ind (size_t index)  { return 2 * index + 1; }
                size_t get_right_child_ind (size_t index) { return 2 * index + 2; }

                void push (min_heap_node_t<T> &&node) { nodes.push_back(node); }

                min_heap_node_t<T> get_min () const { return nodes[0]; }

                void replace_min (min_heap_node_t<T> &node) 
                { 
                        nodes[0] = node; 
                        heapify(0);
                }
        private:
                void heapify (size_t root_index);
};

template <typename T>
inline void min_heap_t<T>::heapify (size_t root_index)
{
        size_t left  = get_left_child_ind(root_index);
        size_t right = get_right_child_ind(root_index);
        size_t smallest = root_index;

        if (left < nodes.size() && nodes[left].elem < nodes[root_index].elem)
                smallest = left;

        if (right < nodes.size() && nodes[right].elem < nodes[smallest].elem)
                smallest = right;

        if (smallest != root_index) {
                std::swap(nodes[root_index], nodes[smallest]);
                heapify(smallest);
        }
}

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

                void copy_sort_tape ()
                {
                        uptr_itape_t input_tape {create_tape<T>(config, input_file_path)};
                        tape_size = input_tape->get_size();
                        chunk_size = tape_size;

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

                        if (n_chunks != 1) {
                                std::filesystem::create_directory("tmp/");
                                for (size_t i = 0; i < n_chunks; i++) {
                                        input_tape->read_next(tape_data_chunks.get(), chunk_n_elems);

                                        std::sort(tape_data_chunks.get(), tape_data_chunks.get() + chunk_n_elems);
                                        min_heap.push(min_heap_node_t {tape_data_chunks[0], i, 1});

                                        shptr_itape_t temp {create_tape<T>(config, "tmp/" + std::to_string(i) + ".tape", true)};

                                        temp->write(0, tape_data_chunks.get(), chunk_n_elems);
                                        tmp_tapes.push_back(temp);
                                }
                        } else {
                                input_tape->read_next(tape_data_chunks.get(), chunk_n_elems);

                                std::sort(tape_data_chunks.get(), tape_data_chunks.get() + chunk_n_elems);

                                uptr_itape_t output {create_tape<T>(config, output_file_path)};
                                output->write(0, tape_data_chunks.get(), chunk_n_elems);
                                return;
                        }

                        merge_tmp_tapes(tape_data_chunks.get());
                }

                void merge_tmp_tapes (T* tape_data_chunks)
                {
                        uptr_itape_t output {create_tape<T>(config, output_file_path)};

                        for (size_t i = 0; i < tape_n_elems; i++) {
                                min_heap_node_t root = min_heap.get_min();
                                output->write_next(root.elem);

                                if (root.next_elem_index < chunk_n_elems) {
                                        tmp_tapes[root.arr_index]->read_next(&root.elem);
                                } else {
                                        root.elem = INT_MAX;
                                }
                                min_heap.replace_min(root);
                        }
                }
};

}
