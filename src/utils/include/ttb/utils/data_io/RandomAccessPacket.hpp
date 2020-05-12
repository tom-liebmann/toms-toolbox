#pragma once

#include "Packet.hpp"


namespace ttb
{
    /**
     * Packet class that provides random access to the underlying data.
     */
    class RandomAccessPacket : public Packet
    {
    public:
        virtual size_t size() const = 0;

        virtual size_t write( size_t offset, size_t size, Writer& writer ) const = 0;

        size_t write( size_t offset, size_t size, void* dst, size_t dstSize ) const;

        virtual size_t write( Writer& writer ) const override;
    };
}
