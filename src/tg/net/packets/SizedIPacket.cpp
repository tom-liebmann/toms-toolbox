#include "SizedIPacket.hpp"

#include <tg/net/TCPSocket.hpp>

namespace tg
{
    SizedIPacket::SizedIPacket( TCPSocket& socket )
    {
        uint32_t size;
        socket.receive( reinterpret_cast< void* >( &size ), sizeof( uint32_t ) );

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
