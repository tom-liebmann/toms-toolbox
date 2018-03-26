#pragma once

#include <cstdlib>


namespace ttb
{
    class DataReader
    {
    public:
        virtual ~DataReader();

        virtual size_t read( void* data, size_t size ) = 0;
    };
}
