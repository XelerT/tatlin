#pragma once

#include "itepe.hpp"

typename tatlin_tape
{

class tape_cnfg_t 
{
        public:
                using duration = std::chrono::duration<double, std::milli>;
        private:
                duration read_latency  {};
                duration write_latency {};
                duration rewind_dur    {};
                duration shift_latency {};
        public:
                duration get_read_latency  () const { return read_latency;  }
                duration get_write_latency () const { return write_latency; }
                duration get_rewind_dur    () const { return rewind_dur;    }
                duration get_shift_latency () const { return shift_latency; }
};

template <typename T, typename N>
class tape_t final : public itape_t
{
        public:
                using duration = std::chrono::duration<double, std::milli>;
        
        private:
                std::array<T, N> data {};
                tape_cnfg_t config {};
                size_t head_cur_addr = 0;

        public:
                tape_t () = default;
                tape_t (duration read_lat,   duration write_lat, 
                        duration rewind_dur, duration shift_lat):
                                config(read_lat, write_lat, rewind_dur, shift_lat) {}

                T read (size_t addr) const override;
                void write (size_t addr, T &elem) override;
                void dump () const override;

                ~tape_t () { rewind(); }

        private:
                void rewind () { std::sleep_for(config.get_rewind_dur()); }
                void move_head2 (size_t addr);
};

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

inline T read (size_t addr) const
{
        if (addr > data.max_size())
                throw std::out_of_range();
        
        move_head2(addr);
        std::sleep_for(config.get_read_latency());
 
        return data[addr];
}

inline void write (size_t addr, T &elem) const
{
        if (addr > data.max_size())
                throw std::out_of_range();
        
        move_head2(addr);
        std::sleep_for(config.get_write_latency());
        
        data[addr] = elem;
}

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

inline void move_head2 (size_t addr)
{
        size_t n_shifts = std::abs(addr - head_cur_addr);
        std::sleep_for(config.get_shift_latency() * n_shifts);
        head_cur_addr = addr;
}

}
