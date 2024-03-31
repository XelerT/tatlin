#include "itape.hpp"
#include "tape.hpp"

namespace tatlin_tape
{

template <typename T>
itape_t<T>* create_tape (const tape_cnfg_t &config, const std::string &file_name)
{
        return new tape_t<T> {config, file_name};
}

template <typename T>
itape_t<T>* create_tape (const tape_cnfg_t &config, const std::string &file_name, 
                         size_t size_, bool trunc)
{
        return new tape_t<T> {config, file_name, size_, trunc};
}

template <typename T>
itape_t<T>* create_tape (const tape_t<T> &tape)
{
        return new tape_t<T> {tape};
}

template <typename T>
itape_t<T>* create_tape (duration_t read_lat,   std::chrono::duration<double, std::milli> write_lat, 
                         std::chrono::duration<double, std::milli> rewind_dur, std::chrono::duration<double, std::milli> shift_lat, 
                         const std::string &file_name)
{
        return new tape_t<T> {read_lat,   write_lat, 
                              rewind_dur, shift_lat, file_name};
}

}