#pragma once

#include "../RandomAccessPacket.hpp"

#include <cstdint>


namespace ttb
{
    class BufferPacket : public RandomAccessPacket
    {
    public:
        BufferPacket( void const* data, size_t dataSize );

        virtual size_t size() const override;
        virtual uint8_t const* data() const override;

    private:
        uint8_t const* m_data;
        size_t m_dataSize;
    };
}
