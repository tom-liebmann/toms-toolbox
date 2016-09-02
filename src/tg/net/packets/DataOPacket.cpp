#include "DataOPacket.hpp"

#include <tg/net/TCPSocket.hpp>

#include <cstring>
#include <sstream>

namespace tg
{
    void DataOPacket::append( const uint8_t* data, uint32_t size, bool checkEndianess )
    {
        size_t oldSize = m_data.size();
        m_data.resize( m_data.size() + size );

        if( !checkEndianess || m_endianess == nativeEndianess() )
            memcpy( m_data.data() + oldSize, data, size );
        else
        {
            for( uint32_t i = 0; i < size; ++i )
                m_data[ oldSize + i ] = *( data + size - 1 - i );
        }
    }

    template <>
    void DataOPacket::write< std::string >( const std::string& value )
    {
        uint32_t len = value.length();
        append( reinterpret_cast< const uint8_t* >( &len ), sizeof( uint32_t ) );
        append( reinterpret_cast< const uint8_t* >( value.c_str() ), len, false );
    }

    void DataOPacket::send( TCPSocket& socket ) const
    {
        socket.send( reinterpret_cast< const uint8_t* >( m_data.data() ), getSize() );
    }

    size_t DataOPacket::getSize() const
    {
        return m_data.size();
    }

    std::string DataOPacket::getContent() const
    {
        std::ostringstream os;
        for( uint8_t value : m_data )
            os << static_cast< uint32_t >( value ) << " ";

        return os.str();
    }
}
