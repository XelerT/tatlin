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
                        tape_cnfg_t (float read_lat_, float write_lat_,
                                     float rewind_dur_, float shift_lat_):
                                read_latency(read_lat_), write_latency(write_lat_),
                                rewind_dur(rewind_dur_), shift_latency(shift_lat_)
                                {}
                        tape_cnfg_t (duration_t read_lat_, duration_t write_lat_,
                                     duration_t rewind_dur_, duration_t shift_lat_):
                                read_latency(read_lat_), write_latency(write_lat_),
                                rewind_dur(rewind_dur_), shift_latency(shift_lat_)
                                {}
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
                        virtual void write_next (const T *elem, size_t n_elems = 1) = 0;
                        virtual void write (size_t addr, const T *elems, size_t n_elems) = 0;

                        virtual size_t get_size () const = 0;
                        virtual std::string get_name () = 0;
                        virtual void move_head2 (size_t addr) = 0;
                        virtual void rewind () = 0;


                        virtual ~itape_t () {}
        };
}
