#pragma once

#include <cstdint>
#include <cstdlib>
#include <vector>


namespace ttb
{
    class RingBuffer
    {
    public:
        explicit RingBuffer( size_t capacity );

        void clear();

        bool pushAll( void const* data, size_t dataSize );

        size_t push( void const* data, size_t dataSize );

        size_t capacity() const;

        size_t size() const;

        size_t emptySpace() const;

        bool empty() const;

        bool full() const;

        size_t blockSize() const;

        void pop( size_t dataSize );

        bool popAll( void* dst, size_t dstSize );

        size_t pop( void* dst, size_t dstSize );

        uint8_t* data();

        uint8_t const* data() const;

    private:
        size_t m_start{ 0 };
        size_t m_end{ 0 };
        size_t m_used{ 0 };
        std::vector< uint8_t > m_buffer;
    };
}
