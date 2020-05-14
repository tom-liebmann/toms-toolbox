#pragma once

#include "../Packet.hpp"
#include "../Writer.hpp"


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
        virtual size_t write( Writer& writer ) const override;

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
    size_t PrefixPacket< TPrefixType >::write( Writer& writer ) const
    {
        if( auto const written = writer.write( &m_prefix, sizeof( m_prefix ) );
            written < sizeof( m_prefix ) )
        {
            return written;
        }
        else
        {
            return sizeof( m_prefix ) + m_packet.get().write( writer );
        }
    }
}
