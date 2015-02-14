#include "OPacket.hpp"

#include <tgNet/TCPSocket.hpp>

#include <cstring>
#include <sstream>

using namespace tgNet;

OPacket::OPacket( uint32_t size )
    : m_buffer( static_cast< uint8_t* >( operator new( size ) ) )
    , m_size( 0 )
    , m_cap( size )
{ }

OPacket::~OPacket()
{
    delete m_buffer;
}

template<>
void OPacket::write< std::string >( const std::string& value )
{
    uint32_t len = value.length();
    append( reinterpret_cast< const uint8_t* >( &len ), sizeof( uint32_t ) );
    append( reinterpret_cast< const uint8_t* >( value.c_str() ), len );
}

void OPacket::send( const TCPSocket* socket ) const
{
    { // send packet length
        uint32_t done = 0;
        while( done < sizeof( uint32_t ) )
            done += socket->send( reinterpret_cast< const uint8_t* >( &m_size + done ), sizeof( uint32_t ) - done );
    }

    { // send packet data
        uint32_t done = 0;
        while( done < m_size )
            done += socket->send( m_buffer + done, m_size - done );
    }
}

void OPacket::append( const uint8_t* data, uint32_t size )
{
    ensure( size );
    memcpy( static_cast< void* >( m_buffer + m_size ), static_cast< const void* >( data ), size );
    m_size += size;
}

void OPacket::ensure( uint32_t size )
{
    if( m_cap - m_size < size )
    {
        uint8_t* buf = static_cast< uint8_t* >( operator new( m_size + size ) );
        memcpy( static_cast< void* >( buf ), static_cast< void* >( m_buffer ), m_size + size );
        delete m_buffer;
        m_buffer = buf;
        m_cap = m_size + size;
    }
}

std::string OPacket::getContent() const
{
    std::ostringstream os;

    for( uint32_t i = 0; i < m_size; ++i )
        os << static_cast< int >( m_buffer[ i ] ) << " ";

    return os.str();
}
