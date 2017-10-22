#include "SocketDataWriter.hpp"

#include <sys/socket.h>

#include <iostream>
#include <sstream>


namespace
{
    std::string toBits( uint8_t value );
}


namespace ttb
{
    namespace posix
    {
        SocketDataWriter::SocketDataWriter( ttb::posix::Selectable const& socket )
            : m_socket( socket )
        {
        }

        size_t SocketDataWriter::write( void const* data, size_t size )
        {
            auto result = ::send(
                m_socket.handle(), reinterpret_cast< uint8_t const* >( data ), size, MSG_NOSIGNAL );

            if( result < 0 )
            {
                if( errno != EAGAIN && errno != EWOULDBLOCK )
                {
                    throw std::runtime_error( "Connection broken" );
                }

                return 0;
            }

            for( size_t i = 0; i < result; ++i )
            {
                std::cout << "Written: " << toBits( reinterpret_cast< uint8_t const* >( data )[ i ] ) << std::endl;
            }

            return result;
        }
    }
}


namespace
{
    std::string toBits( uint8_t value )
    {
        std::ostringstream stream;

        for( size_t i = 0; i < 8; ++i )
        {
            stream << ( ( ( value >> ( 7 - i ) ) & 1 ) ? '1' : '0' );
        }

        return stream.str();
    }
}
