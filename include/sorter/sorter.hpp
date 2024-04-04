#pragma once

#include "tape/itape.hpp"
#include "tape/tapes.hpp"

#include "utils/minheap.hpp"

namespace tatlin_tape 
{

template <typename T>
class tape_sorter_t 
{
        public:
                using uptr_itape_t  = std::unique_ptr<itape_t<T>>;
                using shptr_itape_t = std::shared_ptr<itape_t<T>>;

        private:
                min_heap_t<T> min_heap {};

                size_t chunk_size = 0;
                size_t n_chunks   = 0;
                size_t tape_size  = 0;

                size_t tape_n_elems = 0;
                size_t chunk_n_elems = 0;

                std::unique_ptr<T[]> tape_data_chunks {};

        public:
                size_t get_n_chunks   () const { return n_chunks;   }    
                size_t get_chunk_size () const { return chunk_size; }     

                tape_sorter_t (size_t input_size):
                        chunk_size(input_size),
                        tape_data_chunks(new(std::nothrow) T[chunk_size])
                {
                        while (!tape_data_chunks && chunk_size) {
                                chunk_size /= 2;
                                tape_data_chunks.reset(new(std::nothrow) T[chunk_size]);
                        }
                        if (!chunk_size)
                                throw std::bad_alloc();

                        n_chunks = input_size / chunk_size;
                        if (input_size > chunk_size * n_chunks)
                                ++n_chunks;

                        tape_n_elems  = input_size / sizeof(T);
                        chunk_n_elems = chunk_size / sizeof(T);
                }

                template <typename It>
                void sort (itape_t<T> &input, itape_t<T> &output, It tmp_tapes);

        private:
                void write_output_tape (itape_t<T> &input, itape_t<T> &output);
                template <typename It>
                void fill_tmp_tapes (itape_t<T> &input, It tmp_tapes);
                template <typename It>
                void fill_tmp_tape (itape_t<T> &input, It tmp_tape, size_t n_tape);
                template <typename It>
                void merge_tapes (itape_t<T> &output, It tapes);
                template <typename It>
                void move_heads2 (size_t addr, It tmp_tapes);
};

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

template <typename T>
template <typename It>
inline void tape_sorter_t<T>::sort (itape_t<T> &input, itape_t<T> &output, It tmp_tapes)
{
        if (n_chunks != 1) {
                fill_tmp_tapes(input, tmp_tapes);
                merge_tapes(output, tmp_tapes);
        } else {
                write_output_tape(input, output);
        }
}

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

template <typename T>
inline void tape_sorter_t<T>::write_output_tape (itape_t<T> &input, itape_t<T> &output)
{
        input.read_next(tape_data_chunks.get(), chunk_n_elems);

        std::sort(tape_data_chunks.get(), tape_data_chunks.get() + chunk_n_elems);
        output.write(0, tape_data_chunks.get(), chunk_n_elems);
}

template <typename T>
template <typename It>
inline void tape_sorter_t<T>::fill_tmp_tapes (itape_t<T> &input, It tmp_tapes)
{
        for (size_t i = 0; i < n_chunks; i++)
                fill_tmp_tape(input, tmp_tapes++, i);
}

template <typename T>
template <typename It>
inline void tape_sorter_t<T>::fill_tmp_tape (itape_t<T> &input, It tmp_tape, size_t n_tape)
{
        size_t n_elems = tmp_tape->get_n_elems();

        input.read_next(tape_data_chunks.get(), n_elems);

        std::sort(tape_data_chunks.get(), tape_data_chunks.get() + n_elems);
        min_heap.push(min_heap_node_t {tape_data_chunks[0], n_tape, 1});

        tmp_tape->write(0, tape_data_chunks.get(), n_elems);
        tmp_tape->rewind();
}

template <typename T>
template <typename It>
inline void tape_sorter_t<T>::merge_tapes (itape_t<T> &output, It tapes)
{
        min_heap.initial_heapify();
        move_heads2(sizeof(T), tapes);

        for (size_t i = 0; i < tape_n_elems; i++) {
                min_heap_node_t root = min_heap.get_min();
                output.write_next(&root.elem);

                if (root.next_elem_index < tapes[root.arr_index].get_n_elems()) {
                        tapes[root.arr_index].read_next(&root.elem);
                        root.next_elem_index++;
                } else {
                        root.elem = INT_MAX;
                }
                min_heap.replace_min(root);
        }
}

template <typename T>
template <typename It>
void tape_sorter_t<T>::move_heads2 (size_t addr, It tmp_tapes)
{
        std::for_each(tmp_tapes, tmp_tapes + n_chunks, 
                      [&](itape_t<T> &tape) { tape.move_head2(addr); });
}

}