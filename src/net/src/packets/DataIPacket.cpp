#include <ttb/net/packets/DataIPacket.hpp>

#include <cstdlib>
#include <sstream>

namespace ttb
{
    DataIPacket::DataIPacket( std::shared_ptr< IPacket const > parent, Endianness endianness )
        : m_parent( std::move( parent ) ), m_endianness( endianness )
    {
    }

    template <>
    std::string DataIPacket::read< std::string >()
    {
        auto len = read< uint32_t >();

        std::string result( reinterpret_cast< char const* >(
                                reinterpret_cast< uint8_t const* >( m_parent->data() ) + m_offset ),
                            len );
        m_offset += len;

        return result;
    }

    void const* DataIPacket::data() const
    {
        return reinterpret_cast< uint8_t const* >( m_parent->data() ) + m_offset;
    }

    size_t DataIPacket::size() const
    {
        return m_parent->size() - m_offset;
    }
}
