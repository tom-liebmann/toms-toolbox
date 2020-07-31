#pragma once

#include "DataWriter.hpp"

#include <cstdint>
#include <vector>


namespace ttb
{
    class BufferPacket;
}


namespace ttb
{
    class BufferedWriter : public Writer
    {
    public:
        BufferedWriter( size_t capacity = 0 );

        size_t capacity() const;

        void clear();

        BufferPacket packet() const;

        // Override: DataWriter
        virtual size_t write( void const* data, size_t size ) override;

    private:
        std::vector< uint8_t > m_data;
    };
}
