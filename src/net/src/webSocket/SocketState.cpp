#include <ttb/net/webSocket/SocketState.hpp>


namespace ttb
{
    namespace webSocket
    {
        SocketState::SocketState( WebSocket& socket ) : m_socket( socket )
        {
        }

        SocketState::~SocketState() = default;

        WebSocket& SocketState::socket()
        {
            return m_socket;
        }

        WebSocket const& SocketState::socket() const
        {
            return m_socket;
        }
    }
}
