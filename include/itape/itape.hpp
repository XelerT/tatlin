#pragma once

#include <memory>

namespace tatlin_tape
{
        template <typename T>
        class itape_t
        {
                public:
                        virtual void read (T *output, size_t addr, size_t n_elems = 1) = 0;
                        virtual void write (size_t addr, T &elem) = 0;
                        // virtual void dump  () const               = 0;

                        virtual ~itape_t () {}
        };
}
