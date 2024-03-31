#pragma once

#include <string>
#include <chrono>

namespace tatlin_tape
{
        using duration_t = std::chrono::duration<double, std::milli>;

        class tape_cnfg_t 
        {
                private:
                        duration_t read_latency  {};
                        duration_t write_latency {};
                        duration_t rewind_dur    {};
                        duration_t shift_latency {};
                public:
                        duration_t get_read_latency  () const { return read_latency;  }
                        duration_t get_write_latency () const { return write_latency; }
                        duration_t get_rewind_dur    () const { return rewind_dur;    }
                        duration_t get_shift_latency () const { return shift_latency; }
        };

        template <typename T>
        class itape_t
        {
                public:
                        virtual void read (T *output, size_t addr, size_t n_elems = 1) = 0;
                        virtual void read_next (T *output, size_t n_elems = 1) = 0;
                        virtual void write (size_t addr, T &elem) = 0;
                        virtual void write_next (const T &elem) = 0;
                        virtual void write (size_t addr, const T *elems, size_t n_elems) = 0;

                        virtual size_t get_size () const = 0;
                        virtual std::string get_name () = 0;
                        virtual void move_head2 (size_t addr) = 0;

                        virtual ~itape_t () {}
        };
}

        // input_tape->read_next(tape_data_chunks.get(), n_remaining_bytes / sizeof(T));

        // std::sort(tape_data_chunks.get(), tape_data_chunks.get() + n_remaining_bytes / sizeof(T));
        // min_heap.push(min_heap_node_t {tape_data_chunks[0], n_chunks - 1, 1});

        // std::string tmp_tape_name = "tmp/" + std::to_string(n_chunks - 1) + ".tape";
        // shptr_itape_t tmp {create_tape<T>(config, tmp_tape_name, n_remaining_bytes, true)};

        // tmp->write(0, tape_data_chunks.get(), n_remaining_bytes / sizeof(T));
        // tmp_tapes.push_back(tmp);

        // min_heap.initial_heapify();
