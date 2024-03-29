#pragma once

#include <memory>

namespace tatlin_tape
{
        class itape_t
        {
                public:
                        template <typename T>
                        virtual T read (size_t addr) const = 0;
                        template <typename T>
                        virtual void write (size_t addr, T &elem) = 0;
                        virtual void dump  () const               = 0;

                        virtual ~tape_t () {}
        };
}
