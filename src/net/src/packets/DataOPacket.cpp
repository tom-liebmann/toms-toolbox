#include <ttb/net/packets/DataOPacket.hpp>

#include <ttb/net/DataWriter.hpp>
#include <ttb/net/TCPSocket.hpp>

#include <cstring>
#include <sstream>

namespace ttb
{
    DataOPacket::DataOPacket( Endianness endianness ) : m_endianness( endianness )
    {
    }

    DataOPacket::DataOPacket( uint32_t size, Endianness endianness )
        : m_endianness( endianness ), m_data( size )
    {
    }

    void DataOPacket::append( void const* data, size_t size, bool checkEndianness )
    {
        size_t oldSize = m_data.size();
        m_data.resize( m_data.size() + size );

        if( !checkEndianness || m_endianness == nativeEndianness() )
        {
            memcpy( m_data.data() + oldSize, data, size );
        }
        else
        {
            for( size_t i = 0; i < size; ++i )
            {
                m_data[ oldSize + i ] =
                    *( reinterpret_cast< uint8_t const* >( data ) + size - 1 - i );
            }
        }
    }

    size_t DataOPacket::size() const
    {
        return m_data.size();
    }

    size_t DataOPacket::write( DataWriter& writer, size_t offset, size_t size ) const
    {
        if( offset < m_data.size() )
        {
            return writer.write( m_data.data() + offset, std::min( size, m_data.size() - offset ) );
        }
        else
        {
            return 0;
        }
    }

    template <>
    void DataOPacket::write< std::string >( std::string const& value )
    {
        uint32_t len = value.length();
        append( &len, sizeof( uint32_t ) );
        append( value.c_str(), len, false );
    }
}
