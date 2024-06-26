#pragma once

#include <chrono>
#include <thread>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>

#include "itape.hpp"

namespace tatlin_tape
{

template <typename T>
class tape_t final : public itape_t<T>
{  
        private:
                tape_cnfg_t config {};
                std::string name {};
                std::fstream tape {};
                size_t head_cur_addr = 0;
                size_t size = 0;
                size_t n_elems = 0;

        public:
                tape_t () = default;
                tape_t (duration_t read_lat,   duration_t write_lat, 
                        duration_t rewind_dur, duration_t shift_lat, 
                        const std::string &file_name):
                        config(read_lat, write_lat, rewind_dur, shift_lat),
                        name(file_name)
                {
                        tape.open(file_name, std::fstream::in | std::fstream::out);

                        size = std::filesystem::file_size(file_name);
                        n_elems = size / sizeof(T);
                }
                tape_t (const tape_cnfg_t &config_, const std::string &file_name):
                        config(config_),
                        name(file_name)
                {
                        if (!std::filesystem::exists(file_name)) {
                                tape.open(file_name, std::fstream::out);
                                tape.close();
                        }
                        tape.open(file_name, std::fstream::in | std::fstream::out);
                        size = std::filesystem::file_size(file_name);                        
                        n_elems = size / sizeof(T);
                }
                tape_t (const tape_cnfg_t &config_, const std::string &file_name, 
                        size_t size_, bool trunc):
                        config(config_),
                        name(file_name),
                        size(size_)
                {
                        n_elems = size / sizeof(T);

                        if (!std::filesystem::exists(file_name)) {
                                tape.open(file_name, std::fstream::out);
                                tape.close();
                        }
                        if (trunc)
                                tape.open(file_name, std::fstream::in  | 
                                                     std::fstream::out | 
                                                     std::fstream::trunc);
                        else
                                tape.open(file_name, std::fstream::in  | 
                                                     std::fstream::out);
                }

                void read_next (T *output, size_t n_elems_ = 1) override;
                void read (T *output, size_t addr, size_t n_elems_ = 1) override;
                void write_next (const T *elems, size_t n_elems_ = 1) override;
                void write (size_t addr, T &elem) override;
                void write (size_t addr, const T *elems, size_t n_elems_) override;

                std::string get_name () noexcept override  { return name; }
                size_t get_size () const noexcept override { return size; }
                size_t get_n_elems () const noexcept override { return n_elems; }
                void move_head2 (size_t addr) override;
                void rewind () { std::this_thread::sleep_for(config.get_rewind_dur()); tape.seekg(0); }
                
                tape_t (tape_t &tape_) = delete;
                tape_t& operator=(tape_t &tape_) = delete;

                tape_t (tape_t &&tape_):
                        config(std::move(tape_.config)),
                        name(std::move(tape_.name)),
                        tape(std::move(tape_.tape)),
                        head_cur_addr(tape_.head_cur_addr),
                        size(tape_.size), n_elems(tape_.n_elems) {}
                tape_t& operator=(tape_t &&tape_)
                {
                        std::swap(config, tape_.config);
                        std::swap(name, tape_.name);
                        std::swap(tape, tape_.tape);
                        std::swap(head_cur_addr, tape_.head_cur_addr);
                        std::swap(size, tape_.size);
                        std::swap(n_elems, tape_.n_elems);
                }

                ~tape_t () { rewind(); tape.close(); }

        private:
                void check_addr (size_t addr);
};

//===================================================~~~DECLARATIONS~~~====================================================================

//---------------------------------------------------~~~~~~Public~~~~~~--------------------------------------------------------------------

template <typename T>
inline void tape_t<T>::read_next (T *output, size_t n_elems_)
{
        std::this_thread::sleep_for(config.get_read_latency());
        
        tape.read(reinterpret_cast<char*>(output), sizeof(T) * n_elems_);
        move_head2(head_cur_addr + sizeof(T) * n_elems_);
}

template <typename T>
inline void tape_t<T>::read (T *output, size_t addr, size_t n_elems_)
{
        check_addr(addr);
        
        move_head2(addr);
        std::this_thread::sleep_for(config.get_read_latency());
        tape.read(reinterpret_cast<char*>(output), sizeof(T) * n_elems_);
}

template <typename T>
inline void tape_t<T>::write_next (const T *elems, size_t n_elems_)
{        
        std::this_thread::sleep_for(config.get_write_latency());
        
        tape.write(reinterpret_cast<const char*>(elems), sizeof(T) * n_elems_);
}

template <typename T>
inline void tape_t<T>::write (size_t addr, T &elem)
{
        check_addr(addr);
        
        move_head2(addr);
        std::this_thread::sleep_for(config.get_write_latency());
        
        tape.write(reinterpret_cast<char*>(&elem), sizeof(T));
}

template <typename T>
inline void tape_t<T>::write (size_t addr, const T *elem, size_t n_elems_)
{
        check_addr(addr);

        move_head2(addr);
        std::this_thread::sleep_for(config.get_write_latency());
        
        tape.write(reinterpret_cast<const char*>(elem), sizeof(T) * n_elems_);
}

template <typename T>
inline void tape_t<T>::move_head2 (size_t addr)
{
        size_t n_shifts = addr > head_cur_addr ? addr - head_cur_addr : head_cur_addr - addr;
        std::this_thread::sleep_for(config.get_shift_latency() * n_shifts);
        head_cur_addr = addr;

        tape.seekg(head_cur_addr);
}

//---------------------------------------------------~~~~~~Private~~~~~~--------------------------------------------------------------------

template <typename T>
inline void tape_t<T>::check_addr (size_t addr)
{
        if (addr > size)
                throw std::out_of_range("Tape is shorter than that address.");
}

}
