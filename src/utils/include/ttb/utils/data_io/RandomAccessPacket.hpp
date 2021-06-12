#pragma once

#include "Packet.hpp"

#include <cstdint>


namespace ttb
{
    /**
     * Packet class that provides random access to the underlying data.
     */
    class RandomAccessPacket : public Packet
    {
    public:
        virtual size_t size() const override = 0;

        virtual size_t write( size_t offset, size_t size, Writer& writer ) const override;

        using Packet::write;

        virtual uint8_t const* data() const = 0;
    };
}
