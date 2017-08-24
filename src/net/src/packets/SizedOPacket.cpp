#include <ttb/net/packets/SizedOPacket.hpp>

#include <ttb/net/TCPSocket.hpp>

namespace ttb
{
    void SizedOPacket::send( TCPSocket& socket ) const
    {
        uint32_t size = m_packet->getSize();

        if( m_endianness == nativeEndianness() )
            socket.send( &size, sizeof( uint32_t ) );
        else
        {
            uint32_t invSize;
            for( uint8_t i = 0; i < sizeof( size ); ++i )
                *( reinterpret_cast< uint8_t* >( &invSize ) + i ) =
                    *( reinterpret_cast< uint8_t const* >( &size ) + 3 - i );
            socket.send( &invSize, sizeof( uint32_t ) );
        }

        m_packet->send( socket );
    }

    size_t SizedOPacket::getSize() const
    {
        return sizeof( uint32_t ) + m_packet->getSize();
    }

    std::string SizedOPacket::getContent() const
    {
        return m_packet->getContent();
    }
}
