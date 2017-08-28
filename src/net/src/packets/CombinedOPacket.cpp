#include <ttb/net/packets/CombinedOPacket.hpp>

namespace ttb
{
    size_t CombinedOPacket::size() const
    {
        size_t result = 0;

        for( auto const& packet : m_packets )
        {
            result += packet->size();
        }

        return result;
    }

    void CombinedOPacket::send( TCPSocket& socket ) const
    {
        for( auto const& packet : m_packets )
        {
            packet->send( socket );
        }
    }
}
