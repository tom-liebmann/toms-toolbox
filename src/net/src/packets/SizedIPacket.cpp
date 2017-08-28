#include <ttb/net/packets/SizedIPacket.hpp>

#include <ttb/net/TCPSocket.hpp>

namespace ttb
{
    SizedIPacket::SizedIPacket( TCPSocket& socket, Endianness endianness )
    {
        uint32_t size;
        socket.receive( &size, sizeof( uint32_t ) );

        if( endianness != nativeEndianness() )
        {
            size = reverse( size );
        }

        m_data.resize( size );
        socket.receive( m_data.data(), size );
    }

    void const* SizedIPacket::data() const
    {
        return m_data.data();
    }

    size_t SizedIPacket::size() const
    {
        return m_data.size();
    }
}
