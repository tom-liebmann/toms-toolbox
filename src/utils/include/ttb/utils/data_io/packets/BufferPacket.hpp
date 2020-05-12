#pragma once

#include "../RandomAccessPacket.hpp"

#include <cstdint>


namespace ttb
{
    class BufferPacket : RandomAccessPacket
    {
    public:
        BufferPacket( void const* data, size_t dataSize );

        virtual size_t size() const override;
        virtual size_t write( size_t offset, size_t size, Writer& writer ) const override;

    private:
        uint8_t const* m_data;
        size_t m_dataSize;
    };
}
