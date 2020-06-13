#pragma once

#include "../Writer.hpp"

#include <cstdint>


namespace ttb
{
    class RandomAccessPacket;
    class BufferPacket;
}


namespace ttb
{
    class BufferWriter : public Writer
    {
    public:
        BufferWriter( void* buffer, size_t bufferSize );

        size_t capacity() const;

        void clear();

        BufferPacket packet() const;

        // Override: Writer
        virtual size_t write( void const* data, size_t size ) override;

    private:
        uint8_t* m_buffer{ nullptr };
        size_t m_bufferSize{ 0 };

        size_t m_writeHead{ 0 };
    };
}
