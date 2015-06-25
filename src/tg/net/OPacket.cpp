#include "OPacket.hpp"

#include <tg/net/TCPSocket.hpp>

#include <cstring>
#include <sstream>
#include <iostream>

namespace tg
{
    OPacket::OPacket( uint32_t size )
        : m_buffer( reinterpret_cast< uint8_t* >( operator new( size ) ) )
        , m_size( 0 )
        , m_cap( size )
    { }

    OPacket::OPacket( const OPacket& packet )
        : m_buffer( reinterpret_cast< uint8_t* >( operator new( packet.m_cap ) ) )
        , m_size( packet.m_size )
        , m_cap( packet.m_cap )
    {
        memcpy(
            reinterpret_cast< void* >( m_buffer ),
            reinterpret_cast< const void* >( packet.m_buffer ),
            m_size
        );
    }

    OPacket::~OPacket()
    {
        delete m_buffer;
    }

    OPacket& OPacket::operator=( const OPacket& packet )
    {
        delete m_buffer;
        m_size = packet.m_size;
        m_cap = packet.m_cap;
        m_buffer = reinterpret_cast< uint8_t* >( operator new( m_cap ) );
        memcpy(
            reinterpret_cast< void* >( m_buffer ),
            reinterpret_cast< const void* >( packet.m_buffer ),
            m_size
        );
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
        // send packet length
        socket->send( &m_size, sizeof( uint32_t ) );

        // send packet data
        socket->send( m_buffer, m_size );
    }

    void OPacket::append( const uint8_t* data, uint32_t size )
    {
        ensure( size );
        memcpy(
            reinterpret_cast< void* >( m_buffer + m_size ),
            reinterpret_cast< const void* >( data ),
            size
        );
        m_size += size;
    }

    void OPacket::ensure( uint32_t size )
    {
        if( m_cap - m_size < size )
        {
            uint8_t* buf = reinterpret_cast< uint8_t* >( operator new( m_size + size ) );
            memcpy( reinterpret_cast< void* >( buf ), reinterpret_cast< const void* >( m_buffer ), m_size );
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
}
