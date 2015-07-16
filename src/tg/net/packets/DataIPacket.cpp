#include "DataIPacket.hpp"

#include <cstdlib>

namespace tg
{
    template<>
    std::string DataIPacket::read< std::string >()
    {
        uint32_t len = read< uint32_t >();

        std::string result( reinterpret_cast< const char* >( m_frame->getData() + m_offset ), len );
        m_offset += len;

        return result;
    }

    const uint8_t* DataIPacket::getData() const
    {
        return m_frame->getData() + m_offset;
    }

    size_t DataIPacket::getSize() const
    {
        return m_frame->getSize() - m_offset;
    }
}
