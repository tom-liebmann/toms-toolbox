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
        virtual size_t write( Writer& writer ) const override;

        virtual size_t size() const = 0;

        virtual uint8_t const* data() const = 0;
    };
}
