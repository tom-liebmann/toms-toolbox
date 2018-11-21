#include <ttb/net/packets/DataOPacket.hpp>

#include <ttb/net/DataWriter.hpp>

#include <cstring>
#include <sstream>

namespace ttb
{
    DataOPacket::Creator DataOPacket::create( Endianness endianness )
    {
        return { endianness };
    }

    DataOPacket::DataOPacket( Endianness endianness, std::vector< uint8_t > data )
        : m_endianness( endianness ), m_data( std::move( data ) )
    {
    }

    size_t DataOPacket::size() const
    {
        return m_data.size();
    }

    void DataOPacket::write( DataWriter& writer ) const
    {
        writer.write( m_data.data(), m_data.size() );
    }


    DataOPacket::Creator& DataOPacket::Creator::reserve( size_t size )
    {
        m_data.reserve( size );
        return *this;
    }

    template <>
    DataOPacket::Creator& DataOPacket::Creator::write< std::string >( std::string const& value )
    {
        auto len = static_cast< uint32_t >( value.length() );
        append( &len, sizeof( uint32_t ) );
        append( value.c_str(), len, false );

        return *this;
    }

    std::shared_ptr< DataOPacket > DataOPacket::Creator::finish()
    {
        return std::shared_ptr< DataOPacket >(
            new DataOPacket( m_endianness, std::move( m_data ) ) );
    }

    DataOPacket::Creator::Creator( Endianness endianness ) : m_endianness( endianness )
    {
    }

    void DataOPacket::Creator::append( void const* data, size_t size, bool checkEndianness )
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
}
