#pragma once

#include <cstdlib>


namespace ttb
{
    class DataWriter
    {
    public:
        virtual ~DataWriter();

        virtual size_t write( void const* data, size_t size ) = 0;
    };
}
