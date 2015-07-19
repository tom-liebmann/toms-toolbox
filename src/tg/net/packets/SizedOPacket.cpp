#include "SizedOPacket.hpp"

#include <tg/net/TCPSocket.hpp>

namespace tg
{
    void SizedOPacket::send( TCPSocket& socket ) const
    {
        uint32_t size = m_packet.getSize();
        socket.send( &size, sizeof( uint32_t ) );

        m_packet.send( socket );
    }

    size_t SizedOPacket::getSize() const
    {
        return sizeof( uint32_t ) + m_packet.getSize();
    }

    std::string SizedOPacket::getContent() const
    {
        return m_packet.getContent();
    }
}
