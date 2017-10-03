#include <ttb/net/packets/SizedIPacket.hpp>

#include <ttb/net/TCPSocket.hpp>

namespace ttb
{
    SizedIPacket::SizedIPacket( std::vector< uint8_t > data ) : m_data( std::move( data ) )
    {
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
