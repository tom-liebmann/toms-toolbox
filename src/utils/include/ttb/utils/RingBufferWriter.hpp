#pragma once

#include "RingBuffer.hpp"

#include <ttb/utils/data_io/Writer.hpp>

#include <functional>


namespace ttb
{
    class RingBufferWriter : public ttb::Writer
    {
    public:
        RingBufferWriter( RingBuffer& buffer );

        virtual size_t write( void const* data, size_t size ) override;

    private:
        std::reference_wrapper< RingBuffer > m_buffer;
    };
}
