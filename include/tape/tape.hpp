#pragma once

#include <chrono>
#include <thread>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>

#include "itape/itape.hpp"

namespace tatlin_tape
{

class tape_cnfg_t 
{
        public:
                using duration_t = std::chrono::duration<double, std::milli>;
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

namespace fs = std::filesystem;   

template <typename T>
class tape_t final : public itape_t<T>
{
        public:
                using duration_t = std::chrono::duration<double, std::milli>;    

        private:
                tape_cnfg_t config {};
                std::string name {};
                std::fstream tape {};
                size_t head_cur_addr = 0;
                size_t size = 0;

        public:
                tape_t () = default;
                tape_t (duration_t read_lat,   duration_t write_lat, 
                        duration_t rewind_dur, duration_t shift_lat, 
                        const std::string &file_name):
                        config(read_lat, write_lat, rewind_dur, shift_lat),
                        name(file_name)
                {
                        // tape.rdbuf()->pubsetbuf(0, 0);
                        tape.open(file_name, std::fstream::in | std::fstream::out);

                        size = fs::file_size(file_name);
                }
                tape_t (const tape_cnfg_t &config_, const std::string &file_name):
                        config(config_),
                        name(file_name)
                {
                        // tape.rdbuf()->pubsetbuf(0, 0);
                        if (!fs::exists(file_name)) {
                                tape.open(file_name, std::fstream::out);
                                tape.close();
                        }
                        tape.open(file_name, std::fstream::in | std::fstream::out);
                        size = fs::file_size(file_name);
                }
                tape_t (const tape_t &tape_):
                        config(tape_.config),
                        name(tape_.name),
                        size(tape_.size)
                {
                        // tape.rdbuf()->pubsetbuf(0, 0);
                        tape.open(name, std::fstream::in | std::fstream::out);
                }
                // tape_t (const tape_cnfg_t &config_, const std::string &file_name):
                //         config(config_),
                //         name(file_name)
                // {
                //         // tape.rdbuf()->pubsetbuf(0, 0);
                //         tape.open(file_name);
                // }

                void read_next (T *output, size_t n_elems = 1);
                void read (T *output, size_t addr, size_t n_elems = 1) override;
                void write_next (const T &elem);
                void write (size_t addr, T &elem) override;
                void write (size_t addr, const T *elems, size_t n_elems);
                // void dump () const override;

                std::string get_name () { return name; }
                size_t get_size () { return size; }

                ~tape_t () { rewind(); tape.close(); }

        private:
                void rewind () { std::this_thread::sleep_for(config.get_rewind_dur()); tape.seekg(0); }
                void move_head2 (size_t addr);
};

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

template <typename T>
inline void tape_t<T>::read_next (T *output, size_t n_elems)
{
        std::this_thread::sleep_for(config.get_read_latency());
        
        tape.read(reinterpret_cast<char*>(output), sizeof(T) * n_elems);
        move_head2(head_cur_addr + sizeof(T) * n_elems);
}

template <typename T>
inline void tape_t<T>::read (T *output, size_t addr, size_t n_elems)
{
        if (addr > size)
                throw std::out_of_range("Tape is shorter than that address.");
        
        move_head2(addr);
        std::this_thread::sleep_for(config.get_read_latency());
        std::cout << name << "\n";
        // tape.flush();
        // tape.seekp(-1, std::ios_base::cur);
        tape.read(reinterpret_cast<char*>(output), sizeof(T) * n_elems);
        // tape.flush();
        std::cout << +*reinterpret_cast<char*>(output) << std::endl;
}

template <typename T>
inline void tape_t<T>::write_next (const T &elem)
{
        // if (addr > data.max_size())
        //         throw std::out_of_range();
        
        std::this_thread::sleep_for(config.get_write_latency());
        
        tape.write(reinterpret_cast<const char*>(&elem), sizeof(T));
}

template <typename T>
inline void tape_t<T>::write (size_t addr, T &elem)
{
        // if (addr > data.max_size())
        //         throw std::out_of_range();
        
        move_head2(addr);
        std::this_thread::sleep_for(config.get_write_latency());
        
        tape.write(reinterpret_cast<char*>(&elem), sizeof(T));
}

template <typename T>
inline void tape_t<T>::write (size_t addr, const T *elem, size_t n_elems)
{
        move_head2(addr);
        std::this_thread::sleep_for(config.get_write_latency());
        
        tape.write(reinterpret_cast<const char*>(elem), sizeof(T) * n_elems);
}

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

template <typename T>
inline void tape_t<T>::move_head2 (size_t addr)
{
        size_t n_shifts = addr > head_cur_addr ? addr - head_cur_addr : head_cur_addr - addr;
        std::this_thread::sleep_for(config.get_shift_latency() * n_shifts);
        head_cur_addr = addr;

        tape.seekg(head_cur_addr);
}

}