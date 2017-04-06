#pragma once

#ifdef WIN32
#include <winsock2.h>
#endif

#include <memory>

namespace ttb
{
    class TCPSocket;
}

namespace ttb
{
    class Listener
    {
    public:
#ifdef WIN32
        using Handle = SOCKET;
#else
        using Handle = int;
#endif

        Listener( uint16_t port );

        ~Listener();

        std::unique_ptr< TCPSocket > accept();

    private:
        Handle m_handle;
    };
}
