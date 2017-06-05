#include <ttb/net/packets/SizedIPacket.hpp>

#include <ttb/net/TCPSocket.hpp>

namespace ttb
{
    SizedIPacket::SizedIPacket( TCPSocket& socket, Endianess endianess )
    {
        uint32_t size;
        socket.receive( reinterpret_cast< void* >( &size ), sizeof( uint32_t ) );

        if( endianess != nativeEndianess() )
            size = reverse( size );

        m_data.resize( size );
        socket.receive( reinterpret_cast< void* >( m_data.data() ), size );
    }

    const uint8_t* SizedIPacket::getData() const
    {
        return m_data.data();
    }

    size_t SizedIPacket::getSize() const
    {
        return m_data.size();
    }

    std::string SizedIPacket::getContent( const std::string& inner ) const
    {
        return inner;
    }
}
