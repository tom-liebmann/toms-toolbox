#include "CombinedOPacket.hpp"

namespace tg
{
    void CombinedOPacket::send( TCPSocket& socket ) const
    {
        m_first.send( socket );
        m_second.send( socket );
    }

    size_t CombinedOPacket::getSize() const
    {
        return m_first.getSize() + m_second.getSize();
    }
}
