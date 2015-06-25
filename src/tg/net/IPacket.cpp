#include "IPacket.hpp"

#include <tg/net/TCPSocket.hpp>

#include <cstring>
#include <sstream>

std::unique_ptr< tg::IPacket > tg::IPacket::fromTCPSocket( const TCPSocket& socket )
{
    uint32_t size;
    socket.receive( &size, sizeof( uint32_t ) );

    uint8_t* buffer = static_cast< uint8_t* >( operator new( size ) );
    try
    {
        socket.receive( buffer, size );
    }
    catch( TCPSocket::Error& e )
    {
        delete buffer;
        throw e;
    }

    std::unique_ptr< IPacket > result( new IPacket() );
    result->m_buffer = buffer;
    result->m_size = size;
    result->m_cursor = 0;

    return result;
}

tg::IPacket::~IPacket()
{
    delete m_buffer;
}

template<>
std::string tg::IPacket::read< std::string >()
{
    uint32_t len;
    read( reinterpret_cast< uint8_t* >( &len ), sizeof( uint32_t ) );
    if( len > m_size - m_cursor )
        throw 0;

    std::string value = std::string( reinterpret_cast< char* >( m_buffer + m_cursor ), len );
    m_cursor += len;

    return value;
}

void tg::IPacket::read( uint8_t* data, uint32_t size )
{
    if( size > m_size - m_cursor )
        throw 0;

    memcpy( static_cast< void* >( data ), static_cast< const void* >( m_buffer + m_cursor ), size );
    m_cursor += size;
}

std::string tg::IPacket::getContent() const
{
    std::ostringstream os;

    for( uint32_t i = 0; i < m_size; ++i )
        os << static_cast< int >( m_buffer[ i ] ) << " ";

    return os.str();
}
