#include "SocketDataReader.hpp"

#include <sys/socket.h>

#include <iostream>


namespace ttb
{
    namespace posix
    {
        SocketDataReader::SocketDataReader( ttb::posix::Selectable& selectable )
            : m_selectable( selectable ), m_begin( 0 ), m_end( 0 )
        {
        }

        bool SocketDataReader::doRead()
        {
            auto handle = m_selectable.handle();

            if( m_end < m_buffer.size() )
            {
                auto result = ::recv(
                    handle, m_buffer.data() + m_end, m_buffer.size() - m_end, MSG_NOSIGNAL );

                if( result < 0 )
                {
                    if( errno != EAGAIN && errno != EWOULDBLOCK )
                    {
                        return false;
                    }
                }
                else if( result == 0 )
                {
                    return false;
                }
                else
                {
                    m_end += result;
                }
            }

            return true;
        }

        size_t SocketDataReader::available() const
        {
            return m_end - m_begin;
        }

        size_t SocketDataReader::read( void* data, size_t size )
        {
            if( size > 0 && m_end - m_begin > 0 )
            {
                auto dataPtr = reinterpret_cast< uint8_t* >( data );
                auto readSize = std::min( size, m_end - m_begin );

                std::copy(
                    m_buffer.data() + m_begin, m_buffer.data() + m_begin + readSize, dataPtr );

                m_begin += readSize;

                if( m_begin == m_end )
                {
                    m_begin = 0;
                    m_end = 0;
                }

                return readSize;
            }
            else
            {
                return 0;
            }
        }
    }
}
