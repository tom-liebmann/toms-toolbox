#pragma once

#include "../Writer.hpp"

#include <cstdint>


namespace ttb
{
    class RandomAccessPacket;
}


namespace ttb
{
    class BufferWriter : public Writer
    {
    public:
        BufferWriter( void* buffer, size_t bufferSize );

        size_t capacity() const;

        void clear();

        RandomAccessPacket const& packet() const;

        // Override: Writer
        virtual size_t write( void const* data, size_t size ) override;

    private:
        uint8_t* m_buffer;
        size_t m_bufferSize;

        size_t m_writeHead;
    };
}
