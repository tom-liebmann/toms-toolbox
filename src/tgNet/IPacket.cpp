#include "IPacket.hpp"

#include <tgNet/TCPSocket.hpp>

#include <cstring>

using namespace tgNet;

IPacket* IPacket::fromTCPSocket( const TCPSocket* socket )
{
    uint32_t size;

    uint32_t done = 0;
    while( done < sizeof( uint32_t ) )
        done += socket->receive( reinterpret_cast< uint8_t* >( &size + done ), sizeof( uint32_t ) - done );

    uint8_t* buffer = static_cast< uint8_t* >( operator new( size ) );
    done = 0;
    try
    {
        while( done < size )
            done += socket->receive( buffer + done, size - done );
    }
    catch( TCPSocket::Error& e )
    {
        delete buffer;
        throw e;
    }

    IPacket* result = new IPacket();
    result->m_buffer = buffer;
    result->m_size = size;
    result->m_cursor = 0;

    return result;
}

IPacket::~IPacket()
{
    delete m_buffer;
}

template<>
std::string IPacket::read< std::string >()
{
    uint32_t len;
    read( reinterpret_cast< uint8_t* >( &len ), sizeof( uint32_t ) );
    if( len > m_size - m_cursor )
        throw 0;

    std::string value = std::string( reinterpret_cast< char* >( m_buffer + m_cursor ), len );
    m_cursor += len;

    return std::move( value );
}

void IPacket::read( uint8_t* data, uint32_t size )
{
    if( size > m_size - m_cursor )
        throw 0;

    memcpy( static_cast< void* >( data ), static_cast< const void* >( m_buffer + m_cursor ), size );
    m_cursor += size;
}
