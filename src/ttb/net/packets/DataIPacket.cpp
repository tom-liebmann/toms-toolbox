#include "DataIPacket.hpp"

#include <cstdlib>
#include <sstream>

namespace ttb
{
    template <>
    std::string DataIPacket::read< std::string >()
    {
        uint32_t len = read< uint32_t >();

        std::string result( reinterpret_cast< const char* >( m_frame.getData() + m_offset ), len );
        m_offset += len;

        return result;
    }

    const uint8_t* DataIPacket::getData() const
    {
        return m_frame.getData() + m_offset;
    }

    size_t DataIPacket::getSize() const
    {
        return m_frame.getSize() - m_offset;
    }

    std::string DataIPacket::getContent( const std::string& inner ) const
    {
        std::ostringstream os;

        for( size_t i = 0; i < getSize(); ++i )
            os << static_cast< uint32_t >( getData()[ i ] ) << " ";

        return m_frame.getContent( os.str() );
    }
}
