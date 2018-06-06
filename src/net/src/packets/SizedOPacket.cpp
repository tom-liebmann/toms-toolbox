// TODO: handle endianness


#include <ttb/net/packets/SizedOPacket.hpp>

#include <iostream>


namespace ttb
{
    SizedOPacket::SizedOPacket( std::shared_ptr< OPacket const > packet, Endianness endianness )
        : m_packet( std::move( packet ) ), m_endianness( endianness )
    {
    }

    size_t SizedOPacket::size() const
    {
        return sizeof( uint32_t ) + m_packet->size();
    }

    void SizedOPacket::send( ttb::PushOutput< std::vector< uint8_t > >& output ) const
    {
        uint32_t packetSize = m_packet->size();

        output.push( std::vector< uint8_t >( reinterpret_cast< uint8_t const* >( &packetSize ),
                                             reinterpret_cast< uint8_t const* >( &packetSize ) +
                                                 sizeof( uint32_t ) ) );

        m_packet->send( output );
    }
}
