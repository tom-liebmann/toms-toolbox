#pragma once

#include <cstdlib>


namespace ttb
{
    class DataWriter
    {
    public:
        virtual ~DataWriter();

        virtual void write( void const* data, size_t size ) = 0;
    };
}
