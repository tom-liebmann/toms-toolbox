#pragma once

#include <cstdlib>


namespace ttb
{
    class Writer
    {
    public:
        virtual ~Writer();

        virtual size_t write( void const* data, size_t size ) = 0;
    };
}
