#include "Socket.hpp"

#include <mutex>

namespace tg
{
    void Socket::init()
    {
        #ifdef WIN32

            const int vers = 2;
            WSADATA wsaData;
            auto succ = WSAStartup( MAKEWORD( vers, 0 ), &wsaData );

        #endif
    }

    void Socket::destroy()
    {
        #ifdef WIN32

            WSACleanup();

        #endif
    }
}
