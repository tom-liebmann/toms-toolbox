#include "SocketDataWriter.hpp"


namespace ttb
{
    namespace posix
    {
        SocketDataWriter::SocketDataWriter( ttb::posix::Selectable& socket ) : m_socket( socket )
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

            return result;
        }
    }
}
