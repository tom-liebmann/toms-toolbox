#pragma once

#include "../Packet.hpp"
#include "../PacketWriteHelper.hpp"
#include "../Writer.hpp"

#include <functional>


// declarations
//=================================================================================================

namespace ttb
{
    template < typename TPrefixType >
    class PrefixPacket : public Packet
    {
    public:
        PrefixPacket( TPrefixType prefix, Packet const& packet );

        // Override: ttb::Packet
        virtual size_t size() const override;
        virtual size_t write( size_t offset, size_t size, Writer& writer ) const override;
        using Packet::write;

    private:
        TPrefixType m_prefix;
        std::reference_wrapper< Packet const > m_packet;
    };
}


// definitions
//=================================================================================================

namespace ttb
{
    template < typename TPrefixType >
    PrefixPacket< TPrefixType >::PrefixPacket( TPrefixType prefix, Packet const& packet )
        : m_prefix{ prefix }, m_packet{ packet }
    {
    }

    template < typename TPrefixType >
    size_t PrefixPacket< TPrefixType >::size() const
    {
        return sizeof( TPrefixType ) + m_packet.get().size();
    }

    template < typename TPrefixType >
    size_t PrefixPacket< TPrefixType >::write( size_t offset, size_t size, Writer& writer ) const
    {
        auto writeHelper = PacketWriteHelper{ offset, size, writer };

        writeHelper.writeData( &m_prefix, sizeof( m_prefix ) );
        writeHelper.writePacket( m_packet.get() );

        return writeHelper.written();
    }
}
