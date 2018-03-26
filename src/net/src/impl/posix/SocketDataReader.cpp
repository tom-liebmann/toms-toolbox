#include "SocketDataReader.hpp"

#include <sys/socket.h>


namespace ttb
{
    namespace posix
    {
        SocketDataReader::SocketDataReader( ttb::posix::Selectable& selectable )
            : m_selectable( selectable )
        {
        }

        size_t SocketDataReader::read( void* data, size_t size )
        {
            auto result = ::recv(
                m_selectable.handle(), reinterpret_cast< uint8_t* >( data ), size, MSG_NOSIGNAL );

            if( result < 0 )
            {
                if( errno != EAGAIN && errno != EWOULDBLOCK )
                {
                    throw std::runtime_error( "Connection broken" );
                }
                else
                {
                    return 0;
                }
            }

            if( result == 0 )
            {
                throw std::runtime_error( "Connection closed" );
            }

            return result;
        }
    }
}
