#include <ttb/net/Listener.hpp>


namespace ttb
{
    Listener::Listener( uint16_t port ) : m_port( port )
    {
    }

    Listener::~Listener() = default;

    uint16_t Listener::port() const
    {
        return m_port;
    }
}
