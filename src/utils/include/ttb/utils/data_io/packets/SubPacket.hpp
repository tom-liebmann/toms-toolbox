#pragma once

#include <ttb/utils/data_io/Packet.hpp>


namespace ttb
{
    class SubPacket : public Packet
    {
    public:
        SubPacket( size_t offset, size_t size, Packet const& packet );

        virtual size_t size() const override;
        virtual size_t write( size_t offset, size_t size, Writer& writer ) const override;
        using Packet::write;

    private:
        size_t m_offset;
        size_t m_size;
        Packet const& m_packet;
    };
}
