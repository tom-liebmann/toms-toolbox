#include "SocketState.hpp"


namespace ttb
{
    namespace posix
    {
        namespace webSocket
        {
            SocketState::SocketState( ttb::posix::WebSocket& socket ) : m_socket( socket )
            {
            }

            SocketState::~SocketState()
            {
            }

            ttb::posix::WebSocket& SocketState::socket()
            {
                return m_socket;
            }
        }
    }
}
